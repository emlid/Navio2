/*
SPIDev driver code is placed under the BSD license.
Written by Mikhail Avkhimenia (mikhail.avkhimenia@emlid.com)
Copyright (c) 2014, Emlid Limited
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the Emlid Limited nor the names of its contributors
      may be used to endorse or promote products derived from this software
      without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL EMLID LIMITED BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef _SPIDEV_H_
#define _SPIDEV_H_

//#define _XOPEN_SOURCE 600
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>
#include <sys/socket.h>
#include <netdb.h>
#include <cstdlib>
#include <cstring>
#include <cstdio>

class SPIdev {
public:
    SPIdev()
    {
    }

	static int transfer(const char *spidev,
                        unsigned char *tx,
                        unsigned char *rx,
                        unsigned int length,
                        unsigned int speed_hz = 1000000,
                        unsigned char bits_per_word = 8,
                        unsigned short delay_usecs = 0)
    {
		spi_ioc_transfer spi_transfer;
		
		memset(&spi_transfer, 0, sizeof(spi_ioc_transfer));

		spi_transfer.tx_buf = (unsigned long)tx;
		spi_transfer.rx_buf = (unsigned long)rx;
		spi_transfer.len = length;
		spi_transfer.speed_hz = speed_hz;
		spi_transfer.bits_per_word = bits_per_word;
		spi_transfer.delay_usecs = delay_usecs;

		int spi_fd = ::open(spidev, O_RDWR);

		if (spi_fd < 0 ) {
			printf("Error: Can not open SPI device\n");
            
            return -1;
		}

		int status = ioctl(spi_fd, SPI_IOC_MESSAGE(1), &spi_transfer);

		::close(spi_fd);

        // Debug information
		/*
		printf("Tx: ");
		for (int i = 0; i < length; i++)
			printf("%x ", tx[i]);
		printf("\n");

		printf("Rx: ");
		for (int i = 0; i < length; i++)
			printf("%x ", rx[i]);
		printf("\n");
		*/

        return status;
	}
};

#endif //_SPIDEV_H_
