/*
  Written by Alexey Bulatov (alexey.bulatov@emlid.com) for Raspberry Pi
*/

#include "LSM9DS1.h"

#define DEV_ACC_GYRO "/dev/spidev0.3"
#define DEV_MAG      "/dev/spidev0.2"

#define READ_FLAG     0x80
#define MULTIPLE_READ 0x40

#define G_SI          9.80665
#define PI            3.14159

LSM9DS1::LSM9DS1()
{
}

/*-----------------------------------------------------------------------------------------------
                                    REGISTER READ & WRITE
usage: use these methods to read and write LSM9DS1 registers over SPI
-----------------------------------------------------------------------------------------------*/

unsigned int LSM9DS1::WriteReg(const char *dev, uint8_t WriteAddr, uint8_t WriteData )
{
    unsigned int temp_val;
    unsigned char tx[2] = {WriteAddr, WriteData};
    unsigned char rx[2] = {0};
    SPIdev::transfer(dev, tx, rx, 2);
    return rx[1];
}

unsigned int  LSM9DS1::ReadReg(const char *dev, uint8_t ReadAddr)
{
    return WriteReg(dev, ReadAddr | READ_FLAG, 0x00);
}

void LSM9DS1::ReadRegs(const char *dev, uint8_t ReadAddr, uint8_t *ReadBuf, unsigned int Bytes )
{
    unsigned char tx[255] = {0};
    unsigned char rx[255] = {0};

    tx[0] = ReadAddr | READ_FLAG;
    if (dev == DEV_MAG) tx[0] |= MULTIPLE_READ;

    SPIdev::transfer(dev, tx, rx, Bytes + 1);

    for (int i=0; i<Bytes; i++)
        ReadBuf[i] = rx[i + 1];

    usleep(50);
}

/*-----------------------------------------------------------------------------------------------
                                TEST CONNECTION
usage: call this function to know if SPI and LSM9DS1 are working correctly.
returns true if Accel/Gyro and Magnetometer answers
-----------------------------------------------------------------------------------------------*/

bool LSM9DS1::probe()
{
    uint8_t responseXG,responseM;
    responseXG = ReadReg(DEV_ACC_GYRO, LSM9DS1XG_WHO_AM_I);
    responseM = ReadReg(DEV_MAG, LSM9DS1M_WHO_AM_I);
    if (responseXG == WHO_AM_I_ACC_GYRO || responseM == WHO_AM_I_MAG)
        return true;
    else
        return false;
}

bool LSM9DS1::initialize()
{
    //--------Accelerometer and Gyroscope---------
    // enable the 3-axes of the gyroscope
    WriteReg(DEV_ACC_GYRO, LSM9DS1XG_CTRL_REG4, BITS_XEN_G |
                                                BITS_YEN_G |
                                                BITS_ZEN_G);
    // configure the gyroscope
    WriteReg(DEV_ACC_GYRO, LSM9DS1XG_CTRL_REG1_G, BITS_ODR_G_952HZ |
                                                  BITS_FS_G_2000DPS);
    usleep(200);

    // enable the three axes of the accelerometer
    WriteReg(DEV_ACC_GYRO, LSM9DS1XG_CTRL_REG5_XL, BITS_XEN_XL |
                                                   BITS_YEN_XL |
                                                   BITS_ZEN_XL);
    // configure the accelerometer-specify bandwidth selection with Abw
    WriteReg(DEV_ACC_GYRO, LSM9DS1XG_CTRL_REG6_XL, BITS_ODR_XL_952HZ |
                                                   BITS_FS_XL_16G);
    usleep(200);
    //WriteReg(DEV_ACC_GYRO,LSM9DS1XG_CTRL_REG8, 0x44);
    //------------Magnetometer----------------
    WriteReg(DEV_MAG, LSM9DS1M_CTRL_REG1_M, BITS_TEMP_COMP |
                                            BITS_OM_HIGH |
                                            BITS_ODR_M_80HZ);
    WriteReg(DEV_MAG, LSM9DS1M_CTRL_REG2_M, BITS_FS_M_16Gs);
    WriteReg(DEV_MAG, LSM9DS1M_CTRL_REG3_M, BITS_MD_CONTINUOUS); // continuous conversion mode
    WriteReg(DEV_MAG, LSM9DS1M_CTRL_REG4_M, BITS_OMZ_HIGH);
    WriteReg(DEV_MAG, LSM9DS1M_CTRL_REG5_M, 0x00 );
    usleep(200);

    set_gyro_scale(BITS_FS_G_2000DPS);
    set_acc_scale(BITS_FS_XL_16G);
    set_mag_scale(BITS_FS_M_16Gs);
    return true;
}

void LSM9DS1::set_gyro_scale(int scale)
{
    uint8_t reg;
    reg = BITS_FS_G_MASK & ReadReg(DEV_ACC_GYRO, LSM9DS1XG_CTRL_REG1_G);
    WriteReg(DEV_ACC_GYRO, LSM9DS1XG_CTRL_REG1_G,reg | scale);
    switch (scale) {
    case BITS_FS_G_245DPS:
        gyro_scale = 0.00875;
        break;
    case BITS_FS_G_500DPS:
        gyro_scale = 0.0175;
        break;
    case BITS_FS_G_2000DPS:
        gyro_scale = 0.07;
        break;
    }
}

void LSM9DS1::set_acc_scale(int scale)
{
    uint8_t reg;
    reg = BITS_FS_XL_MASK & ReadReg(DEV_ACC_GYRO, LSM9DS1XG_CTRL_REG6_XL);
    WriteReg(DEV_ACC_GYRO, LSM9DS1XG_CTRL_REG6_XL, reg | scale);
    switch (scale) {
    case BITS_FS_XL_2G:
        acc_scale = 0.000061;
        break;
    case BITS_FS_XL_4G:
        acc_scale = 0.000122;
        break;
    case BITS_FS_XL_8G:
        acc_scale = 0.000244;
        break;
    case BITS_FS_XL_16G:
        acc_scale = 0.000732;
        break;
    }
}

void LSM9DS1::set_mag_scale(int scale)
{
    uint8_t reg;
    reg = BITS_FS_M_MASK & ReadReg(DEV_MAG, LSM9DS1M_CTRL_REG2_M);
    WriteReg(DEV_MAG, LSM9DS1M_CTRL_REG2_M, reg | scale);
    switch (scale) {
    case BITS_FS_M_4Gs:
        mag_scale = 0.00014;
        break;
    case BITS_FS_M_8Gs:
        mag_scale = 0.00029;
        break;
    case BITS_FS_M_12Gs:
        mag_scale = 0.00043;
        break;
    case BITS_FS_M_16Gs:
        mag_scale = 0.00058;
        break;
    }
}

void LSM9DS1::read_temp()
{
    uint8_t response[2];
    ReadRegs(DEV_ACC_GYRO, LSM9DS1XG_OUT_TEMP_L, &response[0], 2);
    temperature = (float)(((int16_t)response[1] << 8) | response[0]) / 256. + 25.;
}

void LSM9DS1::read_acc()
{
    uint8_t response[6];
    int16_t bit_data;
    float accelBias[3]={0,0,0};
    ReadRegs(DEV_ACC_GYRO, LSM9DS1XG_OUT_X_L_XL, &response[0], 6);
    for (int i=0; i<3; i++) {
        bit_data = ((int16_t)response[2*i+1] << 8) | response[2*i] ;
        accelerometer_data[i] = G_SI * ((float)bit_data * acc_scale + accelBias[i]);
    }
}

void LSM9DS1::read_gyro()
{
    uint8_t response[6];
    int16_t bit_data;
    float gyroBias[3]={0,0,0};
    ReadRegs(DEV_ACC_GYRO, LSM9DS1XG_OUT_X_L_G, &response[0], 6);
    for (int i=0; i<3; i++) {
        bit_data = ((int16_t)response[2*i+1] << 8) | response[2*i] ;
        gyroscope_data[i] = (PI/180) * ((float)bit_data * gyro_scale + gyroBias[i]);
    }
}

void LSM9DS1::read_mag()
{
    uint8_t response[6];
    int16_t bit_data;
    float magBias[3]={0,0,0};
    ReadRegs(DEV_MAG, LSM9DS1M_OUT_X_L_M, &response[0], 6);
    for (int i=0; i<3; i++) {
        bit_data = ((int16_t)response[2*i+1] << 8) | response[2*i] ;
        magnetometer_data[i] = 1000.0 * ((float)bit_data * mag_scale + magBias[i]);
    }
}

void LSM9DS1::getMotion6(float *ax,float *ay,float *az,float *gx,float *gy,float *gz)
{
    read_acc();
    read_gyro();
    *ax = accelerometer_data[0];
    *ay = accelerometer_data[1];
    *az = accelerometer_data[2];
    *gx = gyroscope_data[0];
    *gy = gyroscope_data[1];
    *gz = gyroscope_data[2];
}

void LSM9DS1::getMotion9(float *ax,float *ay,float *az,float *gx,float *gy,float *gz,float *mx,float *my,float *mz)
{
    read_acc();
    read_gyro();
    read_mag();
    *ax = accelerometer_data[0];
    *ay = accelerometer_data[1];
    *az = accelerometer_data[2];
    *gx = gyroscope_data[0];
    *gy = gyroscope_data[1];
    *gz = gyroscope_data[2];
    *mx = magnetometer_data[0];
    *my = magnetometer_data[1];
    *mz = magnetometer_data[2];
}
