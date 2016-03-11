/*
Provided to you by Emlid Ltd (c) 2015.
twitter.com/emlidtech || www.emlid.com || info@emlid.com

Example: Read accelerometer, gyroscope and magnetometer values from
inertial measurement unit: MPU9250 or LSM9DS1 over SPI on Raspberry Pi + Navio.

Navio's onboard sensors are connected to the SPI bus on Raspberry Pi
and can be read through /dev/spidev0.1 (MPU9250), /dev/spidev0.3 (acc/gyro LSM9DS1)
and /dev/spidev0.2 (mag LSM9DS1).

To run this example navigate to the directory containing it and run following commands:
make
./AccelGyroMag -i [sensor name]
Sensors names: mpu is MPU9250, lsm is LSM9DS1.
For print help:
./AccelGyroMag -h
*/

#include "Navio/MPU9250.h"
#include "Navio/LSM9DS1.h"
#include "Navio/Util.h"
#include <unistd.h>

InertialSensor* create_inertial_sensor(char *sensor_name)
{
    InertialSensor *imu;

    if (!strcmp(sensor_name, "mpu")) {
        printf("Selected: MPU9250\n");
        imu = new MPU9250();
    }
    else if (!strcmp(sensor_name, "lsm")) {
        printf("Selected: LSM9DS1\n");
        imu = new LSM9DS1();
    }
    else {
        return NULL;
    }

    return imu;
}

void print_help()
{
    printf("Possible parameters:\nSensor selection: -i [sensor name]\n");
    printf("Sensors names: mpu is MPU9250, lsm is LSM9DS1\nFor help: -h\n");
}

//=============================================================================
int main(int argc, char *argv[])
{
    int parameter;
    char *sensor_name;

    if (check_apm()) {
        return 1;
    }

    if (argc < 2) {
        printf("Enter parameter\n");
        print_help();
        return EXIT_FAILURE;
    }

    // prevent the error message
    opterr = 0;

    while ((parameter = getopt(argc, argv, "i:h")) != -1) {
        switch (parameter) {
        case 'i': sensor_name = optarg; break;
        case 'h': print_help(); return EXIT_FAILURE;
        case '?': printf("Wrong parameter.\n");
                  print_help();
                  return EXIT_FAILURE;
        }
    }

    InertialSensor *sensor = create_inertial_sensor(sensor_name);

    if (!sensor) {
        printf("Wrong sensor name. Select: mpu or lsm\n");
        return EXIT_FAILURE;
    }

    if (!sensor->probe()) {
        printf("Sensor not enabled\n");
        return EXIT_FAILURE;
    }
    sensor->initialize();

    float ax, ay, az;
    float gx, gy, gz;
    float mx, my, mz;
//-------------------------------------------------------------------------

    while(1) {
        sensor->update();
        sensor->read_accelerometer(&ax, &ay, &az);
        sensor->read_gyroscope(&gx, &gy, &gz);
        sensor->read_magnetometer(&mx, &my, &mz);
        printf("Acc: %+7.3f %+7.3f %+7.3f  ", ax, ay, az);
        printf("Gyr: %+8.3f %+8.3f %+8.3f  ", gx, gy, gz);
        printf("Mag: %+7.3f %+7.3f %+7.3f\n", mx, my, mz);

       usleep(500000);
    }
}
