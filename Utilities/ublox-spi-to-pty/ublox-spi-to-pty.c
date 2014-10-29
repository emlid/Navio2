/*
This code is provided under the BSD license.
Copyright (c) 2014, Emlid Limited. All rights reserved.
Written by Mikhail Avkhimenia (mikhail.avkhimenia@emlid.com)
twitter.com/emlidtech || www.emlid.com || info@emlid.com

Application: Ublox SPI to PTY bridge. Allows to work with Ublox GPS receiver 
over SPI like over a typical serial connection.

To run this app navigate to the directory containing it and run following commands:
make

And run it:
./ublox-spi-to-pty

You will see a name of the created virtual serial port, like this:
/dev/pts/1

You can connect GPSd to the created virtual serial port:
sudo gpsd /dev/pts/1

You can use gpsmon to check if GPS is working:
gpsmon
*/

#define _XOPEN_SOURCE 600
#include <stropts.h>
#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>

//==================== Helper function for error handling =====================

void error(const char *error_message)
{
	fprintf(stderr, "%s\n", error_message);
	exit(1);
}

//=================================== Main ====================================

int main(void)
{
	int res = 0;

	//=============================== Open SPI ================================

	int spi_fd;

	spi_fd = open("/dev/spidev0.0", O_RDWR);
	if (spi_fd < 0)
		error("posix_openpt() error");

	//=============================== Open PTS ================================

	int fdm;
	int fdm_bytes_available;

	fdm = posix_openpt(O_RDWR);

	if (fdm < 0)
		error("posix_openpt() error");

	res = grantpt(fdm);
	if (res != 0)
		error("grantpt() error");

	res = unlockpt(fdm);
	if (res != 0)
		error("unlockpt() error");

	printf("%s\n", ptsname(fdm));

	//========================== Transfering data =============================

	// SPI transfer buffers and settings

	uint8_t txdata;
	uint8_t rxdata;

	struct spi_ioc_transfer tr = {0};

	tr.tx_buf = (unsigned long)&txdata;
	tr.rx_buf = (unsigned long)&rxdata;
	tr.len = 1;
	tr.speed_hz = 245000;
	tr.bits_per_word = 8;

	// UBX + NMEA scanner 

	enum State 
	{
		Message_Start, 

		UBX_Sync2, 
		UBX_Class, 
		UBX_ID, 
		UBX_Length_LSB, 
		UBX_Length_MSB, 
		UBX_Payload, 
		UBX_CK_A, 
		UBX_CK_B,	

		NMEA_Message
	};

	int state = Message_Start;
	unsigned char message[1024];
	unsigned int position = 0;
	unsigned short ubx_payload_length;

	// Scanning and routing GPS messages

	while (1)
	{	
		// Reading data from the PTY
		ioctl(fdm, FIONREAD, &fdm_bytes_available);	
		if (fdm_bytes_available > 0)
			res = read(fdm, &txdata, 1);
		else
			txdata = 0x00;		

		// Performing SPI transfer			
		
		ioctl(spi_fd, SPI_IOC_MESSAGE(1), &tr);

		// Scanning incoming SPI data for messages

		message[position++] = rxdata;

		switch (state) 
		{
		case Message_Start:
			if (rxdata == 0xb5) 
				state = UBX_Sync2;
			else 
				if (rxdata == '$')
					state = NMEA_Message;
				else
				{
					state = Message_Start;
					position = 0;
				}
			break;

		case UBX_Sync2:
			if (rxdata == 0x62) 
				state = UBX_Class;
			else
				if (rxdata == 0xb5)
				{
					state = Message_Start;
					position = 1;
				}
				else
				{
					state = Message_Start;
					position = 0;
				}
			break;

		case UBX_Class:	
			state = UBX_ID;
			break;

		case UBX_ID:
			state = UBX_Length_LSB;
			break;

		case UBX_Length_LSB:
			ubx_payload_length = rxdata;
			state = UBX_Length_MSB;
			break;

		case UBX_Length_MSB:
			ubx_payload_length += rxdata << 8;
			state = UBX_Payload;
			break;

		case UBX_Payload:
			if (position == ubx_payload_length + 6)
				state = UBX_CK_A;
			if (position >= 1022)
			{
				state = Message_Start;
				position = 0;
			}
			break;

		case UBX_CK_A:
			state = UBX_CK_B;
			break;
		
		case UBX_CK_B:
			// Sending the received message after getting the last symbol
			write(fdm, message, 6 + ubx_payload_length + 2);
			state = Message_Start;
			position = 0;
			break;

		case NMEA_Message:
			// Sending the message after receiveng the end symbol (LF) 
			if (rxdata == '\n')
				write(fdm, message, position);

			if (rxdata == '\n' || position >= 80)
			{
				state = Message_Start;
				position = 0;
			}
			break;
		}
	}

	return 0;
}
