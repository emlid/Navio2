/*
Provided to you by Emlid Ltd (c) 2015.
twitter.com/emlidtech || www.emlid.com || info@emlid.com

Example: Read accelerometer and gyroscope values from FIFO of
MPU9250 inertial measurement unit over SPI on Raspberry Pi + Navio.

Navio's onboard MPU9250 is connected to the SPI bus on Raspberry Pi
and can be read through /dev/spidev0.1

Because of high rate of output, it's recommented write values to the text file,
using > [name of file]. With increasing time delay FIFO buffer may overfill,
because of which it will be reset before a conclusion.

To run this example navigate to the directory containing it and run following commands:
make
./FIFO > [name of file]
*/

#include "Navio/MPU9250.h"

//=============================================================================

int main()
{
    //-------------------------------------------------------------------------
    MPU9250 imu;
    int i = 0, count = 0;
    float acc[256], gyro[256];

    imu.initialize();
    //-------------------------------------------------------------------------
    imu.reset_FIFO();
    imu.enable_FIFO();

    while(1) {
        count = imu.read_FIFO(acc, gyro);
        printf("Number of samples: %d\n", count);

        for (i = 0; i < count; i++) {
            printf("Acc: %+7.3f %+7.3f %+7.3f ", acc[3*i], acc[3*i+1], acc[3*i+2]);
            printf("Gyro: %+7.3f %+7.3f %+7.3f\n", gyro[3*i], gyro[3*i+1], gyro[3*i+2]);
        }
        usleep(1000);
    }
}
