/*
Provided to you by Emlid Ltd (c) 2014.
twitter.com/emlidtech || www.emlid.com || info@emlid.com

Example: Receive position information with GPS driver onboard of Navio shield for Raspberry Pi.

Ublox GPS receiver is connected as an SPI device 0.0(/dev/spidev0.0).
The receiver sends information over SPI in an endless stream,
this program is intended to show, how to capture ubx protocol messages
from the stream and extract useful information from them.

To run this example navigate to the directory containing it and run following commands:
make
./ublox
*/

//#define _XOPEN_SOURCE 600
#include "Navio/Ublox.h" 

using namespace std;

int main(int argc, char *argv[]){

    // This vector is used to store location data, decoded from ubx messages.
    // After you decode at least one message successfully, the information is stored in vector 
    // in a way described in function decodeMessage(vector<double>& data) of class UBXParser(see ublox.h)

    std::vector<double> pos_data;


    // create ublox class instance
    Ublox gps;

    // Here we test connection with the receiver. Function testConnection() waits for a ubx protocol message and checks it.
    // If there's at least one correct message in the first 300 symbols the test is passed
    if(gps.testConnection())
    {
        printf("Ublox test OK\n");

        // gps.decodeMessages();
        // You can use this function to decode all messages, incoming from the GPS receiver. The function starts an infinite loop. 
        // In this example we can only decode NAV-POSLLH messages, the others are simply ignored. 
        // You can add new message types in function decodeMessage() of class UBXParser(see ublox.h)


        // Here, however we use a different approach. Instead of trying to extract info from every message(as done in decodeMessages()),
        // this function waits for a message of a specified type and gets you just the information you need
        // In this example we decode NAV-POSLLH messages, adding new types, however is quite easy

        while (true) 
        {

            if (gps.decodeSingleMessage(Ublox::NAV_POSLLH, pos_data) == 1)
            {
                // after desired message is successfully decoded, we can use the information stored in pos_data vector
                // right here, or we can do something with it from inside decodeSingleMessage() function(see ublox.h).
                // the way, data is stored in pos_data vector is specified in decodeMessage() function of class UBXParser(see ublox.h)
                printf("Current location data:\n");
                printf("iTOW: %lf\n", pos_data[0]/1000);
                printf("Latitude: %lf\n", pos_data[2]/10000000);
                printf("Longitude: %lf\n", pos_data[1]/10000000);
                printf("Height: %lf\n", pos_data[3]/100);

            } else {
                // printf("Message not captured\n");
                // use this to see, how often you get the right messages
                // to increase the frequency you can turn off the undesired messages or tweak ublox settings
                // to increase internal receiver frequency
            }
            usleep(200);
        }

    } else {

        printf("Ublox test not passed\nAbort program!\n");

    }

    return 0;
}
