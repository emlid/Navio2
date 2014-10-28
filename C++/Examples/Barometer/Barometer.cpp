/*
Provided to you by Emlid Ltd (c) 2014.
twitter.com/emlidtech || www.emlid.com || info@emlid.com

Example: Get pressure from MS5611 barometer onboard of Navio shield for Raspberry Pi

To run this example navigate to the directory containing it and run following commands:
make
./Barometer
*/

#include "Navio/MS5611.h"
#include <unistd.h>
#include <stdio.h>

int main()
{
    MS5611 baro(RASPBERRY_PI_MODEL_B_I2C, MS5611_DEFAULT_ADDRESS);
    // MS5611 baro(RASPBERRY_PI_MODEL_A_I2C, MS5611_DEFAULT_ADDRESS);

    baro.initialize();

    while (true) {
        baro.refreshPressure();
        usleep(10000); // Waiting for pressure data ready
        baro.readPressure();

        baro.refreshTemperature();
        usleep(10000); // Waiting for temperature data ready
        baro.readTemperature();

        baro.calculatePressureAndTemperature();

        printf("Temperature(C): %f Pressure(millibar): %f\n", baro.getTemperature(), baro.getPressure());
        sleep(1);
    }

    return 0;
}
