/*
GPS driver dcode is placed under the BSD license.
Written by Egor Fedorov (egor.fedorov@emlid.com)
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

#include "Ublox.h"

// class UBXScanner

UBXScanner::UBXScanner()
{
    reset();
}

unsigned char* UBXScanner::getMessage()
{
    return message;
}

unsigned int UBXScanner::getMessageLength()
{
    return message_length;
}

unsigned int UBXScanner::getPosition()
{
    return position;
}

void UBXScanner::reset()
{
    message_length = 0;
    position = 0;
    state = Sync1;
}

int UBXScanner::update(unsigned char data)
{
    if (state != Done)
        message[position++] = data;

    switch (state)
    {
    case Sync1:
        if (data == 0xb5)
            state = Sync2;
        else
            reset();
        break;

    case Sync2:
        if (data == 0x62)
            state = Class;
        else
            if (data == 0xb5)
                state = Sync1;
            else
                reset();
        break;

    case Class:
        state = ID;
        break;

    case ID:
        state = Length1;
        break;

    case Length1:
        payload_length = data;
        state = Length2;
        break;

    case Length2:
        payload_length += data << 8;
        state = Payload;
        break;

    case Payload:
        if (position == payload_length + 6)
            state = CK_A;
        if (position >= 1022)
            reset();
        break;

    case CK_A:
        state = CK_B;
        break;

    case CK_B:
        message_length = 6 + payload_length + 2;
        state = Done;
        break;

    case Done:
    default:
        break;
    }

    return state;
}

// class UBXParser


UBXParser::UBXParser(UBXScanner* ubxsc) : scanner(ubxsc), message(ubxsc->getMessage())
{

}

// This function updates message length and end position in the scanner's buffer

void UBXParser::updateMessageData(){
    length = scanner->getMessageLength();
    position = scanner->getPosition();
}

// Function decodeMessage() returns 1 in case of a successful message verification.
// It looks through the buffer, checks 2 ubx protocol sync chars (0xb5 and 0x62),
// counts the checksum and if everything is alright, defines the message type by id.
// In this example we only decode two messages: Nav-Status and Nav-Posllh. It is possible to add more
// id cases

int UBXParser::decodeMessage(std::vector<double>& data)
{
    int flag=1;
    int pos;
    uint16_t id;
    uint8_t CK_A=0, CK_B=0;

    updateMessageData(); // get the length and end message coordinate from UBX scanner

    pos = position-length; // count the message start position

    // All UBX messages start with 2 sync chars: 0xb5 and 0x62

    if (*(message+pos)!=0xb5) flag=0;
    if (*(message+pos+1)!=0x62) flag=0;

    // Count the checksum

    for (unsigned int i=2;i<(length-2);i++){
        CK_A += *(message+pos+i);
        CK_B += CK_A;
    }

    if (CK_A != *(message+pos+length-2)) flag=0;
    if (CK_B != *(message+pos+length-1)) flag=0;

    // If the sync chars, or the checksum is wrong, we should not be doing this anymore

    if (flag==0) return 0;

    // If we got everything right, then it's time to decide, what type of message this is

    id = (*(message+pos+2)) << 8 | (*(message+pos+3)); // ID is a two-byte number with little endianness

    flag = id; // will return message id, if we got the info decoded

    switch(id){
        case 258:
                // ID for Nav-Posllh messages is 0x0102 == 258
                // In this example we extract 7 variables - longitude, latitude,
                // height above ellipsoid and mean sea level, horizontal and vertical
                // accuracy estimate and iTOW - GPS Millisecond Time of Week

                // All the needed parameters are 4-byte numbers with little endianness.
                // We know the current message and we want to update the info in the data vector.
                // First we clear the old data:

                data.clear();

                // Second, we extract the needed data from the message buffer and save it to the vector.

                //iTOW
                data.push_back ((unsigned)((*(message+pos+9) << 24) | (*(message+pos+8) << 16) | (*(message+pos+7) << 8) | (*(message+pos+6))));
                //Longitude
                data.push_back ((*(message+pos+13) << 24) | (*(message+pos+12) << 16) | (*(message+pos+11) << 8) | (*(message+pos+10)));
                //Latitude
                data.push_back ((*(message+pos+17) << 24) | (*(message+pos+16) << 16) | (*(message+pos+15) << 8) | (*(message+pos+14)));
                //Height above Ellipsoid
                data.push_back ((*(message+pos+21) << 24) | (*(message+pos+20) << 16) | (*(message+pos+19) << 8) | (*(message+pos+18)));
                //Height above mean sea level
                data.push_back ((*(message+pos+25) << 24) | (*(message+pos+24) << 16) | (*(message+pos+23) << 8) | (*(message+pos+22)));
                //Horizontal Accuracy Estateimate
                data.push_back ((unsigned)((*(message+pos+29) << 24) | (*(message+pos+28) << 16) | (*(message+pos+27) << 8) | (*(message+pos+26))));
                //Vertical Accuracy Estateimate
                data.push_back ((unsigned)((*(message+pos+33) << 24) | (*(message+pos+32) << 16) | (*(message+pos+31) << 8) | (*(message+pos+30))));
                break;

        case 259:
                // ID for Nav-Status messages is 0x0103 == 259
                // This message contains a lot of information, but the reason we use it the GPS fix status
                // This status is a one byte flag variable, with the offset of 10: first 6 bytes of the captured
                // message contain message header, id, payload length, next 4 are the first payload variable - iTOW.
                // We are not interested in it, so we just proceed to the fifth byte - gpsFix flag.
                // We are also interested in checking the gpsFixOk flag in the next byte/
                data.clear();
                // gpsFix
                data.push_back (*(message+pos+10));
                // flags, which contain gpsFixOk
                data.push_back (*(message+pos+11));

                break;

        default:
                // In case we don't want to decode the received message
                flag = 0;

                break;
    }

    return flag;
}

// Function checkMessage() returns 1 if the message, currently stored in the buffer is valid.
// Validity means, that the necessary sync chars are present and the checksum test is passed

int UBXParser::checkMessage()
{
    int flag=1;
    int pos;
    uint8_t CK_A=0, CK_B=0;

    updateMessageData(); // get the length and end message coordinate from UBX scanner

    pos = position-length; // count the message start position

    // All UBX messages start with 2 sync chars: 0xb5 and 0x62

    if (*(message+pos)!=0xb5) flag=0;
    if (*(message+pos+1)!=0x62) flag=0;

    // Count the checksum

    for (unsigned int i=2;i<(length-2);i++){
        CK_A += *(message+pos+i);
        CK_B += CK_A;
    }

    if (CK_A != *(message+pos+length-2)) flag=0;
    if (CK_B != *(message+pos+length-1)) flag=0;

    return flag;
}

// class Ublox

Ublox::Ublox(std::string name) : spi_device_name(name), scanner(new UBXScanner()), parser(new UBXParser(scanner))
{

}

Ublox::Ublox(std::string name, UBXScanner* scan, UBXParser* pars) : spi_device_name(name), scanner(scan), parser(pars)
{

}

int Ublox::enableNAV_POSLLH()
{
    unsigned char gps_nav_posllh[] = {0xb5, 0x62, 0x06, 0x01, 0x03, 0x00, 0x01, 0x02, 0x01, 0x0E, 0x47};
    int gps_nav_posllh_length = (sizeof(gps_nav_posllh)/sizeof(*gps_nav_posllh));
    unsigned char from_gps_data_nav[gps_nav_posllh_length];

    return SPIdev::transfer(spi_device_name.c_str(), gps_nav_posllh, from_gps_data_nav, gps_nav_posllh_length, 200000);
}

int Ublox::enableNAV_STATUS()
{
    unsigned char gps_nav_status[] = {0xb5, 0x62, 0x06, 0x01, 0x03, 0x00, 0x01, 0x03, 0x01, 0x0F, 0x49};
    int gps_nav_status_length = (sizeof(gps_nav_status)/sizeof(*gps_nav_status));
    unsigned char from_gps_data_nav[gps_nav_status_length];

    return SPIdev::transfer(spi_device_name.c_str(), gps_nav_status, from_gps_data_nav, gps_nav_status_length, 200000);
}

int Ublox::testConnection()
{
    int status;
    int count = 0;
    unsigned char to_gps_data = 0x00, from_gps_data = 0x00;

    // we do this, so that at least one ubx message is enabled

    if (enableNAV_POSLLH()<0)
    {
        std::cerr << "Could not configure ublox over SPI\n";
    }

    if (enableNAV_STATUS()<0)
    {
        std::cerr << "Could not configure ublox over SPI\n";
    }

    while (count < buffer_length/2)
    {
        // From now on, we will send zeroes to the receiver, which it will ignore
        // However, we are simultaneously getting useful information from it
        SPIdev::transfer(spi_device_name.c_str(), &to_gps_data, &from_gps_data, 1, 200000);

        // Scanner checks the message structure with every byte received
        status = scanner->update(from_gps_data);

        if (status == UBXScanner::Done)
        {
            // Once we have a full message we decode it and reset the scanner, making it look for another message
            // in the data stream, coming over SPI

            // If we find at least one valid message in the buffer, we consider connection to be established
            if(parser->checkMessage()==1)
            {
                scanner->reset();
                return 1;
            }

            scanner->reset();
        }

        count++;
    }

    return 0;
}

int Ublox::decodeMessages()
{
    int status;
    unsigned char to_gps_data = 0x00, from_gps_data = 0x00;
    std::vector<double> position_data;

    if (enableNAV_POSLLH()<0)
    {
        std::cerr << "Could not configure ublox over SPI\n";
    }

    if (enableNAV_STATUS()<0)
    {
        std::cerr << "Could not configure ublox over SPI\n";
    }

    while (true)
    {
        // From now on, we will send zeroes to the receiver, which it will ignore
        // However, we are simultaneously getting useful information from it
        SPIdev::transfer(spi_device_name.c_str(), &to_gps_data, &from_gps_data, 1, 200000);

        // Scanner checks the message structure with every byte received
        status = scanner->update(from_gps_data);

        if (status == UBXScanner::Done)
        {
            // Once we have a full message we decode it and reset the scanner, making it look for another message
            // in the data stream, coming over SPI
            if (parser->decodeMessage(position_data) > 0)
            {
                // do something with the obtained data
                //
                // in case if NAV-POSLLH messages we can do this:
                // printf("decodeMessages(): \nCurrent location data:\n");
                // printf("GPS Millisecond Time of Week: %lf\n", position_data[0]/1000);
                // printf("Longitude: %lf\n", position_data[1]/10000000);
                // printf("Latitude: %lf\n", position_data[2]/10000000);
                // printf("Height above Ellipsoid: %.3lf m\n", pos_data[3]/1000);
                // printf("Height above mean sea level: %.3lf m\n", pos_data[4]/1000);
                // printf("Horizontal Accuracy Estateimate: %.3lf m\n", pos_data[5]/1000);
                // printf("Vertical Accuracy Estateimate: %.3lf m\n", pos_data[6]/1000);
                //
                // in case of NAV-STATUS messages we can do this:
                //
                // printf("Current GPS status:\n");
                // printf("gpsFixOk: %d\n", ((int)pos_data[1] & 0x01));
                //
                // printf("gps Fix status: ");
                // switch((int)pos_data[0]){
                //     case 0x00:
                //         printf("no fix\n");
                //         break;
                //
                //     case 0x01:
                //         printf("dead reckoning only\n");
                //         break;
                //
                //     case 0x02:
                //         printf("2D-fix\n");
                //         break;
                //
                //     case 0x03:
                //         printf("3D-fix\n");
                //         break;
                //
                //     case 0x04:
                //         printf("GPS + dead reckoning combined\n");
                //         break;
                //
                //     case 0x05:
                //         printf("Time only fix\n");
                //         break;
                //
                //     default:
                //         printf("Reserved value. Current state unknown\n");
                //         break;
                //
                // }
                //
                // printf("\n");

            }

            scanner->reset();
        }

        usleep(200);

    }

    return 0 ;
}

int Ublox::decodeSingleMessage(message_t msg, std::vector<double>& position_data)
{
    switch(msg){
        case NAV_POSLLH:
            {
                uint16_t id = 0x0102;
                int status;
                int count = 0;
                unsigned char to_gps_data = 0x00, from_gps_data = 0x00;

                while (count < buffer_length/2)
                {
                    // From now on, we will send zeroes to the receiver, which it will ignore
                    // However, we are simultaneously getting useful information from it
                    SPIdev::transfer(spi_device_name.c_str(), &to_gps_data, &from_gps_data, 1, 200000);

                    // Scanner checks the message structure with every byte received
                    status = scanner->update(from_gps_data);

                    if (status == UBXScanner::Done)
                    {
                        // Once we have a full message we decode it and reset the scanner, making it look for another message
                        // in the data stream, coming over SPI
                        if(parser->decodeMessage(position_data) == id)
                        {
                            // Now let's do something with the extracted information
                            // in case of NAV-POSLLH messages we can print the information like this:
                            // printf("GPS Millisecond Time of Week: %lf\n", position_data[0]/1000);
                            // printf("Longitude: %lf\n", position_data[1]/10000000);
                            // printf("Latitude: %lf\n", position_data[2]/10000000);
                            // printf("Height above Ellipsoid: %.3lf m\n", pos_data[3]/1000);
                            // printf("Height above mean sea level: %.3lf m\n", pos_data[4]/1000);
                            // printf("Horizontal Accuracy Estateimate: %.3lf m\n", pos_data[5]/1000);
                            // printf("Vertical Accuracy Estateimate: %.3lf m\n", pos_data[6]/1000);


                            // You can see ubx message structure in ublox reference manual

                            scanner->reset();

                            return 1;
                        }

                        scanner->reset();
                    }

                    count++;
                }

                return 0;
            }

        break;

        case NAV_STATUS:
            {
                uint16_t id = 0x0103;
                int status;
                int count = 0;
                unsigned char to_gps_data = 0x00, from_gps_data = 0x00;

                while (count < buffer_length/2)
                {
                    // From now on, we will send zeroes to the receiver, which it will ignore
                    // However, we are simultaneously getting useful information from it
                    SPIdev::transfer(spi_device_name.c_str(), &to_gps_data, &from_gps_data, 1, 200000);

                    // Scanner checks the message structure with every byte received
                    status = scanner->update(from_gps_data);

                    if (status == UBXScanner::Done)
                    {
                        // Once we have a full message we decode it and reset the scanner, making it look for another message
                        // in the data stream, coming over SPI
                        if(parser->decodeMessage(position_data) == id)
                        {
                            // Now let's do something with the extracted information
                            // in case of NAV-STATUS messages we can do this:
                            //
                            // printf("Current GPS status:\n");
                            // printf("gpsFixOk: %d\n", ((int)pos_data[1] & 0x01));
                            //
                            // printf("gps Fix status: ");
                            // switch((int)pos_data[0]){
                            //     case 0x00:
                            //         printf("no fix\n");
                            //         break;
                            //
                            //     case 0x01:
                            //         printf("dead reckoning only\n");
                            //         break;
                            //
                            //     case 0x02:
                            //         printf("2D-fix\n");
                            //         break;
                            //
                            //     case 0x03:
                            //         printf("3D-fix\n");
                            //         break;
                            //
                            //     case 0x04:
                            //         printf("GPS + dead reckoning combined\n");
                            //         break;
                            //
                            //     case 0x05:
                            //         printf("Time only fix\n");
                            //         break;
                            //
                            //     default:
                            //         printf("Reserved value. Current state unknown\n");
                            //         break;
                            //
                            // }
                            //
                            // printf("\n");

                            // You can see ubx message structure in ublox reference manual

                            scanner->reset();

                            return 1;
                        }

                        scanner->reset();
                    }

                    count++;
                }

                return 0;
            }

        break;


        // add your ubx message type here!

        default:
            return 0;

        break;
    }
}
