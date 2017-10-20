/*
  Written by Alexey Bulatov (alexey.bulatov@emlid.com) for Raspberry Pi
*/

#ifndef _LSM9DS1_H
#define _LSM9DS1_H

#include <stdint.h>
#include <Common/SPIdev.h>
#include <Common/InertialSensor.h>

class LSM9DS1 : public InertialSensor
{
public:
    LSM9DS1();

    bool initialize();
    bool probe();
    void update();

private:
    unsigned int WriteReg(const char *dev, uint8_t WriteAddr, uint8_t WriteData);
    unsigned int ReadReg(const char *dev, uint8_t ReadAddr);
    void ReadRegs(const char *dev, uint8_t ReadAddr, uint8_t *ReadBuf, unsigned int Bytes);

    void set_gyro_scale(int scale);
    void set_acc_scale(int scale);
    void set_mag_scale(int scale);

    void rotate();

    float gyro_scale;
    float acc_scale;
    float mag_scale;
};

#endif //_LSM9DS1_H

// who am I values
#define WHO_AM_I_ACC_GYRO           0x68
#define WHO_AM_I_MAG                0x3D

// Accelerometer and Gyroscope registers
#define LSM9DS1XG_ACT_THS           0x04
#define LSM9DS1XG_ACT_DUR           0x05
#define LSM9DS1XG_INT_GEN_CFG_XL    0x06
#define LSM9DS1XG_INT_GEN_THS_X_XL  0x07
#define LSM9DS1XG_INT_GEN_THS_Y_XL  0x08
#define LSM9DS1XG_INT_GEN_THS_Z_XL  0x09
#define LSM9DS1XG_INT_GEN_DUR_XL    0x0A
#define LSM9DS1XG_REFERENCE_G       0x0B
#define LSM9DS1XG_INT1_CTRL         0x0C
#define LSM9DS1XG_INT2_CTRL         0x0D
#define LSM9DS1XG_WHO_AM_I          0x0F  // should return 0x68
#define LSM9DS1XG_CTRL_REG1_G       0x10
#define LSM9DS1XG_CTRL_REG2_G       0x11
#define LSM9DS1XG_CTRL_REG3_G       0x12
#define LSM9DS1XG_ORIENT_CFG_G      0x13
#define LSM9DS1XG_INT_GEN_SRC_G     0x14
#define LSM9DS1XG_OUT_TEMP_L        0x15
#define LSM9DS1XG_OUT_TEMP_H        0x16
#define LSM9DS1XG_STATUS_REG        0x17
#define LSM9DS1XG_OUT_X_L_G         0x18
#define LSM9DS1XG_OUT_X_H_G         0x19
#define LSM9DS1XG_OUT_Y_L_G         0x1A
#define LSM9DS1XG_OUT_Y_H_G         0x1B
#define LSM9DS1XG_OUT_Z_L_G         0x1C
#define LSM9DS1XG_OUT_Z_H_G         0x1D
#define LSM9DS1XG_CTRL_REG4         0x1E
#define LSM9DS1XG_CTRL_REG5_XL      0x1F
#define LSM9DS1XG_CTRL_REG6_XL      0x20
#define LSM9DS1XG_CTRL_REG7_XL      0x21
#define LSM9DS1XG_CTRL_REG8         0x22
#define LSM9DS1XG_CTRL_REG9         0x23
#define LSM9DS1XG_CTRL_REG10        0x24
#define LSM9DS1XG_INT_GEN_SRC_XL    0x26
#define LSM9DS1XG_OUT_X_L_XL        0x28
#define LSM9DS1XG_OUT_X_H_XL        0x29
#define LSM9DS1XG_OUT_Y_L_XL        0x2A
#define LSM9DS1XG_OUT_Y_H_XL        0x2B
#define LSM9DS1XG_OUT_Z_L_XL        0x2C
#define LSM9DS1XG_OUT_Z_H_XL        0x2D
#define LSM9DS1XG_FIFO_CTRL         0x2E
#define LSM9DS1XG_FIFO_SRC          0x2F
#define LSM9DS1XG_INT_GEN_CFG_G     0x30
#define LSM9DS1XG_INT_GEN_THS_XH_G  0x31
#define LSM9DS1XG_INT_GEN_THS_XL_G  0x32
#define LSM9DS1XG_INT_GEN_THS_YH_G  0x33
#define LSM9DS1XG_INT_GEN_THS_YL_G  0x34
#define LSM9DS1XG_INT_GEN_THS_ZH_G  0x35
#define LSM9DS1XG_INT_GEN_THS_ZL_G  0x36
#define LSM9DS1XG_INT_GEN_DUR_G     0x37

// Magnetometer registers
#define LSM9DS1M_OFFSET_X_REG_L_M   0x05
#define LSM9DS1M_OFFSET_X_REG_H_M   0x06
#define LSM9DS1M_OFFSET_Y_REG_L_M   0x07
#define LSM9DS1M_OFFSET_Y_REG_H_M   0x08
#define LSM9DS1M_OFFSET_Z_REG_L_M   0x09
#define LSM9DS1M_OFFSET_Z_REG_H_M   0x0A
#define LSM9DS1M_WHO_AM_I           0x0F  // should return 0x3D
#define LSM9DS1M_CTRL_REG1_M        0x20
#define LSM9DS1M_CTRL_REG2_M        0x21
#define LSM9DS1M_CTRL_REG3_M        0x22
#define LSM9DS1M_CTRL_REG4_M        0x23
#define LSM9DS1M_CTRL_REG5_M        0x24
#define LSM9DS1M_STATUS_REG_M       0x27
#define LSM9DS1M_OUT_X_L_M          0x28
#define LSM9DS1M_OUT_X_H_M          0x29
#define LSM9DS1M_OUT_Y_L_M          0x2A
#define LSM9DS1M_OUT_Y_H_M          0x2B
#define LSM9DS1M_OUT_Z_L_M          0x2C
#define LSM9DS1M_OUT_Z_H_M          0x2D
#define LSM9DS1M_INT_CFG_M          0x30
#define LSM9DS1M_INT_SRC_M          0x31
#define LSM9DS1M_INT_THS_L_M        0x32
#define LSM9DS1M_INT_THS_H_M        0x33

// Configuration bits Accelerometer and Gyroscope
#define BITS_XEN_G                  0x08
#define BITS_YEN_G                  0x10
#define BITS_ZEN_G                  0x20
#define BITS_XEN_XL                 0x08
#define BITS_YEN_XL                 0x10
#define BITS_ZEN_XL                 0x20
#define BITS_ODR_G_14900mHZ         0x20
#define BITS_ODR_G_59500mHZ         0x40
#define BITS_ODR_G_119HZ            0x60
#define BITS_ODR_G_238HZ            0x80
#define BITS_ODR_G_476HZ            0xA0
#define BITS_ODR_G_952HZ            0xC0
#define BITS_ODR_XL_10HZ            0x20
#define BITS_ODR_XL_50HZ            0x40
#define BITS_ODR_XL_119HZ           0x60
#define BITS_ODR_XL_238HZ           0x80
#define BITS_ODR_XL_476HZ           0xA0
#define BITS_ODR_XL_952HZ           0xC0
#define BITS_FS_G_MASK              0xE3
#define BITS_FS_G_245DPS            0x00
#define BITS_FS_G_500DPS            0x08
#define BITS_FS_G_2000DPS           0x18
#define BITS_FS_XL_MASK             0xE7
#define BITS_FS_XL_2G               0x00
#define BITS_FS_XL_4G               0x10
#define BITS_FS_XL_8G               0x18
#define BITS_FS_XL_16G              0x08

// Configuration bits Magnetometer
#define BITS_TEMP_COMP              0x80
#define BITS_OM_LOW                 0x00
#define BITS_OM_MEDIUM              0x20
#define BITS_OM_HIGH                0x40
#define BITS_OM_ULTRA_HIGH          0x60
#define BITS_ODR_M_625mHZ           0x00
#define BITS_ODR_M_1250mHZ          0x04
#define BITS_ODR_M_250mHZ           0x08
#define BITS_ODR_M_5HZ              0x0C
#define BITS_ODR_M_10HZ             0x10
#define BITS_ODR_M_20HZ             0x14
#define BITS_ODR_M_40HZ             0x18
#define BITS_ODR_M_80HZ             0x1C
#define BITS_FS_M_MASK              0x0C
#define BITS_FS_M_4Gs               0x00
#define BITS_FS_M_8Gs               0x20
#define BITS_FS_M_12Gs              0x40
#define BITS_FS_M_16Gs              0x60
#define BITS_MD_CONTINUOUS          0x00
#define BITS_MD_SINGLE              0x01
#define BITS_MD_POWERDOWN           0x02
#define BITS_OMZ_LOW                0x00
#define BITS_OMZ_MEDIUM             0x04
#define BITS_OMZ_HIGH               0x08
#define BITS_OMZ_ULTRA_HIGH         0x0C
