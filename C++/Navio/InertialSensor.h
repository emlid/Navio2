#ifndef _INERTIAL_SENSOR_H
#define _INERTIAL_SENSOR_H

#include "SPIdev.h"

class InertialSensor {
public:
    virtual bool initialize() = 0;
    virtual bool probe() = 0;

    virtual void read_temp() = 0;
    virtual void read_acc() = 0;
    virtual void read_gyro() = 0;
    virtual void read_mag() = 0;

    float temperature;
    float accelerometer_data[3];
    float gyroscope_data[3];
    float magnetometer_data[3];

    virtual void getMotion6(float *ax, float *ay, float *az, float *gx, float *gy, float *gz) = 0;
    virtual void getMotion9(float *ax, float *ay, float *az, float *gx, float *gy, float *gz, float *mx, float *my, float *mz) = 0;
};

#endif //_INERTIAL_SENSOR_H
