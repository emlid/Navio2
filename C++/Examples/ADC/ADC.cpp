/*
Provided to you by Emlid Ltd (c) 2014.
twitter.com/emlidtech || www.emlid.com || info@emlid.com

Example: Control ADS1115 connected to PCA9685 driver onboard of Navio shield for Rapspberry Pi.

Connect a wire to P1 and the ADC will measure its voltage and display it.
Be careful, do not connect high-voltage circuits, for acceptable range check the datasheet for ADS1115.

To run this example navigate to the directory containing it and run following commands:
make
./ADC
*/

#include <unistd.h>
#include <err.h>
#include <cstdint>
#include <cstdio>

#include <Navio/Util.h>

int main() {

    const char *channel_path = "/sys/kernel/rcio/adc/ch0";

    while (true) {
        uint32_t conversion;

        if (read_file(channel_path, "%u", &conversion) < 0) {
            warn("Unable to get voltage");
        }

        printf("%.4fV ", static_cast<float> (conversion / 1000.0f));

        printf("\n");
    }

    return 0;
}
