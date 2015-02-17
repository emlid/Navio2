/*
This code is provided under the BSD license.
Copyright (c) 2014, Emlid Limited. All rights reserved.
Written by Mikhail Avkhimenia (mikhail.avkhimenia@emlid.com)
twitter.com/emlidtech || www.emlid.com || info@emlid.com

Application: Ublox SPI to TCP bridge. Allows to work with Ublox GPS receiver 
over SPI like over a typical network TCP connection.

To run this app navigate to the directory containing it and run following commands:
make

Run it with the spicified port argument:
./ublox-spi-to-tcp 5002

You can connect to receiver from u-center by choosing  menu Receiver - Port - 
Network connection - New And enter "tcp://x.x.x.x:5002" where x.x.x.x is the
IP-address of your Raspberry Pi with Navio.
*/

#define _XOPEN_SOURCE 600
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

/*==================== Helper function for error handling ===================== */

void error(const char *error_message)
{
    fprintf(stderr, "%s\n", error_message);
    exit(1);
}

/*=================================== Main ==================================== */
int main(int argc, char *argv[])
{
    int status;

    /*=================== Checking command line arguments  ==================== */

    if (argc < 2) {
        printf("%s [tcp port number]", argv[0]);
        exit(1);
    }

    /*============================= Open SPI ================================== */

    int spi_fd;

    spi_fd = open("/dev/spidev0.0", O_RDWR);
    if (spi_fd < 0) error("open() SPI error");

    /*============================= TCP setup ================================= */

    int server_socket;

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == 0) error("socket() error");

    int yes = 1;

    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) < 0 ) {
        perror("setsockopt");
    }

    struct sockaddr_in server_address = {0};
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(atoi(argv[1]));

    status = bind(server_socket, (struct sockaddr *) &server_address, sizeof(server_address));
    if (status == -1) error("bind() error");

    /*===================== Waiting for TCP connection ======================== */

    int client_socket;

    struct sockaddr_in client_address;
    socklen_t address_size = sizeof(client_address);

    printf("Waiting for connection.\n");

    status = listen(server_socket, 5);
    if (status == -1) error("listen() error");

    client_socket = accept(server_socket, (struct sockaddr *) &client_address, &address_size);
    if (client_socket == -1) printf("Acception error.");
    else printf("Connection accepted.\n");

    /*========================== Transfering data ============================= */

    /* SPI transfer buffers and settings */

    uint8_t txdata;
    uint8_t rxdata;

    struct spi_ioc_transfer tr = {0};

    tr.tx_buf = (unsigned long)&txdata;
    tr.rx_buf = (unsigned long)&rxdata;
    tr.len = 1;
    tr.speed_hz = 245000;
    tr.bits_per_word = 8;

    /* UBX + NMEA scanner */

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

    /* Scanning and routing GPS messagesa */

    while (1) {
        /* Reading data from a TCP client */

        status = recv(client_socket, &txdata, 1, MSG_DONTWAIT);
        if (status == 0) {
            printf("Client disconnected.\n");
            printf("Waiting for connection.\n");
            listen(server_socket, 5); 
            client_socket = accept(server_socket, (struct sockaddr *) &client_address, &address_size);
            if (client_socket == -1) fprintf(stderr, "accept() error.");
            else printf("Connection accepted.\n");
        }
        if (status < 0) txdata = 0x00;

        /* Performing SPI transfer */

        ioctl(spi_fd, SPI_IOC_MESSAGE(1), &tr);

        /* Scanning incoming SPI data for messages */

        message[position++] = rxdata;

        switch (state) 
        {
        case Message_Start:
            if (rxdata == 0xb5) {
                state = UBX_Sync2;
            }
            else 
                if (rxdata == '$'){
                    state = NMEA_Message;
                }
                else {
                    state = Message_Start;
                    position = 0;
                }
            break;

        case UBX_Sync2:
            if (rxdata == 0x62) 
                state = UBX_Class;
            else
                if (rxdata == 0xb5) {
                    state = Message_Start;
                    position = 1;
                } else {
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
            if (position >= 1022) {
                state = Message_Start;
                position = 0;
            }
            break;

        case UBX_CK_A:
            state = UBX_CK_B;
            break;

        case UBX_CK_B:
            /* Sending the received message after getting the last symbol */
            send(client_socket, message, 6 + ubx_payload_length + 2, 0);
            state = Message_Start;
            position = 0;
            break;

        case NMEA_Message:
            /* Sending the message after receiveng the end symbol (LF)  */
            if (rxdata == '\n')
                send(client_socket, message, position, 0);

            if (rxdata == '\n' || position >= 80) {
                state = Message_Start;
                position = 0;
            }
            break;
        }
    }

    return 0 ;
}


