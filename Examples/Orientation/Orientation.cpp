/*
Provided to you by Emlid Ltd (c) 2014.
twitter.com/emlidtech || www.emlid.com || info@emlid.com

Example: Get accelerometer, gyroscope and magnetometer data from MPU9150 sensor
onboard of Navio shield for Raspberry Pi.

To run this example navigate to the directory containing it and run following commands:
make
sudo ./Orientation
*/

#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include "Navio/MPU9150.h"

int main()
{
    int16_t ax, ay, az, gx, gy, gz, mx, my, mz;

    MPU9150 accelgyro;

    accelgyro.initialize();
    accelgyro.setI2CBypassEnabled(1);

    while (true) {
        accelgyro.getMotion9(&ax, &ay, &az, &gx, &gy, &gz, &mx, &my, &mz);
        printf("Acc: %6hd %6hd %6hd Gyr: %6hd %6hd %6hd Mag: %6hd %6hd %6hd\n", ax, ay, az, gx, gy, gz, mx, my, mz);
        sleep(1);
    }
}
