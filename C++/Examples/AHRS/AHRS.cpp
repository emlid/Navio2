/*
This code is provided under the BSD license.
Copyright (c) 2014, Emlid Limited. All rights reserved.
Written by Igor Vereninov and Mikhail Avkhimenia
twitter.com/emlidtech || www.emlid.com || info@emlid.com

Application: Mahory AHRS algorithm supplied with data from MPU9250 and LSM9DS1.
Outputs roll, pitch and yaw in the console and sends quaternion
over the network - it can be used with 3D IMU visualizer located in
Navio/Applications/3D IMU visualizer.

To run this app navigate to the directory containing it and run following commands:
make
sudo ./AHRS -i [sensor name] ipaddress portnumber
Sensors names: mpu is MPU9250, lsm is LSM9DS1.
If you want to visualize IMU data on another machine pass it's address and port
For print help:
./AHRS -h

To achieve stable loop you need to run this application with a high priority
on a linux kernel with real-time patch. Raspbian distribution with real-time
kernel is available at emlid.com and priority can be set with chrt command:
chrt -f -p 99 PID
*/

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/time.h>
#include "Navio/MPU9250.h"
#include "Navio/LSM9DS1.h"
#include "Navio/Util.h"
#include "AHRS.hpp"

#define G_SI 9.80665
#define PI   3.14159

// Objects

InertialSensor *imu;
AHRS    ahrs;   // Mahony AHRS

// Sensor data

float ax, ay, az;
float gx, gy, gz;
float mx, my, mz;

// Orientation data

float roll, pitch, yaw;

// Timing data

float offset[3];
struct timeval tv;
float dt, maxdt;
float mindt = 0.01;
unsigned long previoustime, currenttime;
float dtsumm = 0;
int isFirst = 1;

// Network data

int sockfd;
struct sockaddr_in servaddr = {0};
char sendline[80];

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
    printf("If you want to visualize IMU data on another machine,\n");
    printf("add IP address and port number (by default 7000):\n");
    printf("-i [sensor name] ipaddress portnumber\n");

}

//============================= Initial setup =================================

void imuSetup()
{
    //----------------------- MPU initialization ------------------------------

    imu->initialize();

    //-------------------------------------------------------------------------

	printf("Beginning Gyro calibration...\n");
	for(int i = 0; i<100; i++)
	{
		imu->update();
    imu->read_gyroscope(&gx, &gy, &gz);

    gx *= 180 / PI;
    gy *= 180 / PI;
    gz *= 180 / PI;

		offset[0] += (-gx*0.0175);
		offset[1] += (-gy*0.0175);
		offset[2] += (-gz*0.0175);
		usleep(10000);
	}
	offset[0]/=100.0;
	offset[1]/=100.0;
	offset[2]/=100.0;

	printf("Offsets are: %f %f %f\n", offset[0], offset[1], offset[2]);
	ahrs.setGyroOffset(offset[0], offset[1], offset[2]);
}

//============================== Main loop ====================================

void imuLoop()
{
    //----------------------- Calculate delta time ----------------------------

	gettimeofday(&tv,NULL);
	previoustime = currenttime;
	currenttime = 1000000 * tv.tv_sec + tv.tv_usec;
	dt = (currenttime - previoustime) / 1000000.0;
	if(dt < 1/1300.0) usleep((1/1300.0-dt)*1000000);
        gettimeofday(&tv,NULL);
        currenttime = 1000000 * tv.tv_sec + tv.tv_usec;
	dt = (currenttime - previoustime) / 1000000.0;

    //-------- Read raw measurements from the MPU and update AHRS --------------

    // Accel + gyro.
    imu->update();
    imu->read_accelerometer(&ax, &ay, &az);
    imu->read_gyroscope(&gx, &gy, &gz);

    ax /= G_SI;
    ay /= G_SI;
    az /= G_SI;
    gx *= 180 / PI;
    gy *= 180 / PI;
    gz *= 180 / PI;

    ahrs.updateIMU(ax, ay, az, gx*0.0175, gy*0.0175, gz*0.0175, dt);

    // Accel + gyro + mag.
    // Soft and hard iron calibration required for proper function.
    /*
    imu->update();
    imu->read_accelerometer(&ax, &ay, &az);
    imu->read_gyroscope(&gx, &gy, &gz);
    imu->read_magnetometer(&mx, &my, &mz);

    ax /= G_SI;
    ay /= G_SI;
    az /= G_SI;
    gx *= 180 / PI;
    gy *= 180 / PI;
    gz *= 180 / PI;

    ahrs.update(ax, ay, az, gx*0.0175, gy*0.0175, gz*0.0175, my, mx, -mz, dt);
    */

    //------------------------ Read Euler angles ------------------------------

    ahrs.getEuler(&roll, &pitch, &yaw);

    //------------------- Discard the time of the first cycle -----------------

    if (!isFirst)
    {
    	if (dt > maxdt) maxdt = dt;
    	if (dt < mindt) mindt = dt;
    }
    isFirst = 0;

    //------------- Console and network output with a lowered rate ------------

    dtsumm += dt;
    if(dtsumm > 0.05)
    {
        // Console output
        printf("ROLL: %+05.2f PITCH: %+05.2f YAW: %+05.2f PERIOD %.4fs RATE %dHz \n", roll, pitch, yaw * -1, dt, int(1/dt));

        // Network output
        sprintf(sendline,"%10f %10f %10f %10f %dHz\n", ahrs.getW(), ahrs.getX(), ahrs.getY(), ahrs.getZ(), int(1/dt));
        sendto(sockfd, sendline, strlen(sendline), 0, (struct sockaddr *)&servaddr, sizeof(servaddr));

        dtsumm = 0;
    }
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
    opterr=0;

    while ((parameter = getopt(argc, argv, "i:h")) != -1) {
        switch (parameter) {
        case 'i': sensor_name = optarg; break;
        case 'h': print_help(); return EXIT_FAILURE;
        case '?': printf("Wrong parameter.\n");
                  print_help();
                  return EXIT_FAILURE;
        }
    }

    imu = create_inertial_sensor(sensor_name);

    if (!imu) {
        printf("Wrong sensor name. Select: mpu or lsm\n");
        return EXIT_FAILURE;
    }

    if (!imu->probe()) {
        printf("Sensor not enable\n");
        return EXIT_FAILURE;
    }
    //--------------------------- Network setup -------------------------------

    sockfd = socket(AF_INET,SOCK_DGRAM,0);
    servaddr.sin_family = AF_INET;

    if (argc == 5)  {
        servaddr.sin_addr.s_addr = inet_addr(argv[3]);
        servaddr.sin_port = htons(atoi(argv[4]));
    } else {
        servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
        servaddr.sin_port = htons(7000);
    }

    //-------------------- IMU setup and main loop ----------------------------

    imuSetup();

    while(1)
        imuLoop();
}
