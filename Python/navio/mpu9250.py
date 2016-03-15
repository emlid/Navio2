"""
MPU9250 driver code is placed under the BSD license.
Copyright (c) 2014, Emlid Limited, www.emlid.com
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.
    * Neither the name of the Emlid Limited nor the names of its contributors
    may be used to endorse or promote products derived from this software
    without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL EMLID LIMITED BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
"""

import spidev
import time
import struct
import array

class MPU9250:

    G_SI = 9.80665
    PI = 3.14159

    # MPU9250 registers
    __MPUREG_XG_OFFS_TC = 0x00
    __MPUREG_YG_OFFS_TC = 0x01
    __MPUREG_ZG_OFFS_TC = 0x02
    __MPUREG_X_FINE_GAIN = 0x03
    __MPUREG_Y_FINE_GAIN = 0x04
    __MPUREG_Z_FINE_GAIN = 0x05
    __MPUREG_XA_OFFS_H = 0x06
    __MPUREG_XA_OFFS_L = 0x07
    __MPUREG_YA_OFFS_H = 0x08
    __MPUREG_YA_OFFS_L = 0x09
    __MPUREG_ZA_OFFS_H = 0x0A
    __MPUREG_ZA_OFFS_L = 0x0B
    __MPUREG_PRODUCT_ID = 0x0C
    __MPUREG_SELF_TEST_X = 0x0D
    __MPUREG_SELF_TEST_Y = 0x0E
    __MPUREG_SELF_TEST_Z = 0x0F
    __MPUREG_SELF_TEST_A = 0x10
    __MPUREG_XG_OFFS_USRH = 0x13
    __MPUREG_XG_OFFS_USRL = 0x14
    __MPUREG_YG_OFFS_USRH = 0x15
    __MPUREG_YG_OFFS_USRL = 0x16
    __MPUREG_ZG_OFFS_USRH = 0x17
    __MPUREG_ZG_OFFS_USRL = 0x18
    __MPUREG_SMPLRT_DIV = 0x19
    __MPUREG_CONFIG = 0x1A
    __MPUREG_GYRO_CONFIG = 0x1B
    __MPUREG_ACCEL_CONFIG = 0x1C
    __MPUREG_ACCEL_CONFIG_2      = 0x1D
    __MPUREG_LP_ACCEL_ODR        = 0x1E
    __MPUREG_MOT_THR             = 0x1F
    __MPUREG_FIFO_EN             = 0x23
    __MPUREG_I2C_MST_CTRL        = 0x24
    __MPUREG_I2C_SLV0_ADDR       = 0x25
    __MPUREG_I2C_SLV0_REG        = 0x26
    __MPUREG_I2C_SLV0_CTRL       = 0x27
    __MPUREG_I2C_SLV1_ADDR       = 0x28
    __MPUREG_I2C_SLV1_REG        = 0x29
    __MPUREG_I2C_SLV1_CTRL       = 0x2A
    __MPUREG_I2C_SLV2_ADDR       = 0x2B
    __MPUREG_I2C_SLV2_REG        = 0x2C
    __MPUREG_I2C_SLV2_CTRL       = 0x2D
    __MPUREG_I2C_SLV3_ADDR       = 0x2E
    __MPUREG_I2C_SLV3_REG        = 0x2F
    __MPUREG_I2C_SLV3_CTRL       = 0x30
    __MPUREG_I2C_SLV4_ADDR       = 0x31
    __MPUREG_I2C_SLV4_REG        = 0x32
    __MPUREG_I2C_SLV4_DO         = 0x33
    __MPUREG_I2C_SLV4_CTRL       = 0x34
    __MPUREG_I2C_SLV4_DI         = 0x35
    __MPUREG_I2C_MST_STATUS      = 0x36
    __MPUREG_INT_PIN_CFG = 0x37
    __MPUREG_INT_ENABLE = 0x38
    __MPUREG_ACCEL_XOUT_H = 0x3B
    __MPUREG_ACCEL_XOUT_L = 0x3C
    __MPUREG_ACCEL_YOUT_H = 0x3D
    __MPUREG_ACCEL_YOUT_L = 0x3E
    __MPUREG_ACCEL_ZOUT_H = 0x3F
    __MPUREG_ACCEL_ZOUT_L = 0x40
    __MPUREG_TEMP_OUT_H = 0x41
    __MPUREG_TEMP_OUT_L = 0x42
    __MPUREG_GYRO_XOUT_H = 0x43
    __MPUREG_GYRO_XOUT_L = 0x44
    __MPUREG_GYRO_YOUT_H = 0x45
    __MPUREG_GYRO_YOUT_L = 0x46
    __MPUREG_GYRO_ZOUT_H = 0x47
    __MPUREG_GYRO_ZOUT_L = 0x48
    __MPUREG_EXT_SENS_DATA_00    = 0x49
    __MPUREG_EXT_SENS_DATA_01    = 0x4A
    __MPUREG_EXT_SENS_DATA_02    = 0x4B
    __MPUREG_EXT_SENS_DATA_03    = 0x4C
    __MPUREG_EXT_SENS_DATA_04    = 0x4D
    __MPUREG_EXT_SENS_DATA_05    = 0x4E
    __MPUREG_EXT_SENS_DATA_06    = 0x4F
    __MPUREG_EXT_SENS_DATA_07    = 0x50
    __MPUREG_EXT_SENS_DATA_08    = 0x51
    __MPUREG_EXT_SENS_DATA_09    = 0x52
    __MPUREG_EXT_SENS_DATA_10    = 0x53
    __MPUREG_EXT_SENS_DATA_11    = 0x54
    __MPUREG_EXT_SENS_DATA_12    = 0x55
    __MPUREG_EXT_SENS_DATA_13    = 0x56
    __MPUREG_EXT_SENS_DATA_14    = 0x57
    __MPUREG_EXT_SENS_DATA_15    = 0x58
    __MPUREG_EXT_SENS_DATA_16    = 0x59
    __MPUREG_EXT_SENS_DATA_17    = 0x5A
    __MPUREG_EXT_SENS_DATA_18    = 0x5B
    __MPUREG_EXT_SENS_DATA_19    = 0x5C
    __MPUREG_EXT_SENS_DATA_20    = 0x5D
    __MPUREG_EXT_SENS_DATA_21    = 0x5E
    __MPUREG_EXT_SENS_DATA_22    = 0x5F
    __MPUREG_EXT_SENS_DATA_23    = 0x60
    __MPUREG_I2C_SLV0_DO         = 0x63
    __MPUREG_I2C_SLV1_DO         = 0x64
    __MPUREG_I2C_SLV2_DO         = 0x65
    __MPUREG_I2C_SLV3_DO         = 0x66
    __MPUREG_I2C_MST_DELAY_CTRL  = 0x67
    __MPUREG_SIGNAL_PATH_RESET   = 0x68
    __MPUREG_MOT_DETECT_CTRL     = 0x69
    __MPUREG_USER_CTRL = 0x6A
    __MPUREG_PWR_MGMT_1 = 0x6B
    __MPUREG_PWR_MGMT_2 = 0x6C
    __MPUREG_BANK_SEL = 0x6D
    __MPUREG_MEM_START_ADDR = 0x6E
    __MPUREG_MEM_R_W = 0x6F
    __MPUREG_DMP_CFG_1 = 0x70
    __MPUREG_DMP_CFG_2 = 0x71
    __MPUREG_FIFO_COUNTH = 0x72
    __MPUREG_FIFO_COUNTL = 0x73
    __MPUREG_FIFO_R_W = 0x74
    __MPUREG_WHOAMI = 0x75
    __MPUREG_XA_OFFSET_H         = 0x77
    __MPUREG_XA_OFFSET_L         = 0x78
    __MPUREG_YA_OFFSET_H         = 0x7A
    __MPUREG_YA_OFFSET_L         = 0x7B
    __MPUREG_ZA_OFFSET_H         = 0x7D
    __MPUREG_ZA_OFFSET_L         = 0x7E

    # ---- AK8963 Reg In MPU9250 -----------------------------------------------

    __AK8963_I2C_ADDR             = 0x0c #0x18
    __AK8963_Device_ID            = 0x48

    # Read-only Reg
    __AK8963_WIA                  = 0x00
    __AK8963_INFO                 = 0x01
    __AK8963_ST1                  = 0x02
    __AK8963_HXL                  = 0x03
    __AK8963_HXH                  = 0x04
    __AK8963_HYL                  = 0x05
    __AK8963_HYH                  = 0x06
    __AK8963_HZL                  = 0x07
    __AK8963_HZH                  = 0x08
    __AK8963_ST2                  = 0x09

    # Write/Read Reg
    __AK8963_CNTL1                = 0x0A
    __AK8963_CNTL2                = 0x0B
    __AK8963_ASTC                 = 0x0C
    __AK8963_TS1                  = 0x0D
    __AK8963_TS2                  = 0x0E
    __AK8963_I2CDIS               = 0x0F

    # Read-only Reg ( ROM )
    __AK8963_ASAX                 = 0x10
    __AK8963_ASAY                 = 0x11
    __AK8963_ASAZ                 = 0x12

    # Configuration bits MPU9250
    __BIT_SLEEP = 0x40
    __BIT_H_RESET = 0x80
    __BITS_CLKSEL = 0x07
    __MPU_CLK_SEL_PLLGYROX = 0x01
    __MPU_CLK_SEL_PLLGYROZ = 0x03
    __MPU_EXT_SYNC_GYROX = 0x02
    __BITS_FS_250DPS              = 0x00
    __BITS_FS_500DPS              = 0x08
    __BITS_FS_1000DPS             = 0x10
    __BITS_FS_2000DPS             = 0x18
    __BITS_FS_2G                  = 0x00
    __BITS_FS_4G                  = 0x08
    __BITS_FS_8G                  = 0x10
    __BITS_FS_16G                 = 0x18
    __BITS_FS_MASK                = 0x18
    __BITS_DLPF_CFG_256HZ_NOLPF2  = 0x00
    __BITS_DLPF_CFG_188HZ         = 0x01
    __BITS_DLPF_CFG_98HZ          = 0x02
    __BITS_DLPF_CFG_42HZ          = 0x03
    __BITS_DLPF_CFG_20HZ          = 0x04
    __BITS_DLPF_CFG_10HZ          = 0x05
    __BITS_DLPF_CFG_5HZ           = 0x06
    __BITS_DLPF_CFG_2100HZ_NOLPF  = 0x07
    __BITS_DLPF_CFG_MASK          = 0x07
    __BIT_INT_ANYRD_2CLEAR        = 0x10
    __BIT_RAW_RDY_EN              = 0x01
    __BIT_I2C_IF_DIS              = 0x10

    __READ_FLAG   = 0x80

    # ---- Sensitivity ---------------------------------------------------------

    __MPU9250A_2g       = (0.000061035156) # 0.000061035156 g/LSB
    __MPU9250A_4g       = (0.000122070312) # 0.000122070312 g/LSB
    __MPU9250A_8g       = (0.000244140625) # 0.000244140625 g/LSB
    __MPU9250A_16g      = (0.000488281250) # 0.000488281250 g/LSB

    __MPU9250G_250dps   = (0.007633587786) # 0.007633587786 dps/LSB
    __MPU9250G_500dps   = (0.015267175572) # 0.015267175572 dps/LSB
    __MPU9250G_1000dps  = (0.030487804878) # 0.030487804878 dps/LSB
    __MPU9250G_2000dps  = (0.060975609756) # 0.060975609756 dps/LSB

    __MPU9250M_4800uT   = (0.6)            # 0.6 uT/LSB

    __MPU9250T_85degC   = (0.002995177763) # 0.002995177763 degC/LSB

    __Magnetometer_Sensitivity_Scale_Factor = (0.15)

    def __init__(self, spi_bus_number = 0, spi_dev_number = 1):
        self.bus = spidev.SpiDev()
        self.spi_bus_number = spi_bus_number
        self.spi_dev_number = spi_dev_number
        self.gyro_divider = 0.0
        self.acc_divider = 0.0
        self.calib_data = [0.0, 0.0, 0.0]
        self.magnetometer_ASA = [0.0, 0.0, 0.0]
        self.temperature = 0.0
        self.gyroscope_data = [0.0, 0.0, 0.0]
        self.accelerometer_data = [0.0, 0.0, 0.0]
        self.magnetometer_data = [0.0, 0.0, 0.0]

# -----------------------------------------------------------------------------------------------
#                                     REGISTER READ & WRITE
# usage: use these methods to read and write MPU9250 registers over SPI
# -----------------------------------------------------------------------------------------------

    def WriteReg(self, reg_address, data):
        self.bus.open(self.spi_bus_number, self.spi_dev_number)
        tx = [reg_address, data]
        rx = self.bus.xfer2(tx)
        self.bus.close()
        return rx

# -----------------------------------------------------------------------------------------------

    def ReadReg(self, reg_address):
        self.bus.open(self.spi_bus_number, self.spi_dev_number)
        tx = [reg_address | self.__READ_FLAG, 0x00]
        rx = self.bus.xfer2(tx)
        self.bus.close()
        return rx[1]

# -----------------------------------------------------------------------------------------------

    def ReadRegs(self, reg_address, length):
        self.bus.open(self.spi_bus_number, self.spi_dev_number)
        tx = [0] * (length + 1)
        tx[0] = reg_address | self.__READ_FLAG

        rx = self.bus.xfer2(tx)

        self.bus.close()
        return rx[1:len(rx)]

# -----------------------------------------------------------------------------------------------
#                                 TEST CONNECTION
# usage: call this function to know if SPI and MPU9250 are working correctly.
# returns true if mpu9250 answers
# -----------------------------------------------------------------------------------------------

    def testConnection(self):
        response = self.ReadReg(self.__MPUREG_WHOAMI)
        if (response == 0x71):
            return True
        else:
            return False

# -----------------------------------------------------------------------------------------------
#                                     INITIALIZATION
# usage: call this function at startup, giving the sample rate divider (raging from 0 to 255) and
# low pass filter value; suitable values are:
# BITS_DLPF_CFG_256HZ_NOLPF2
# BITS_DLPF_CFG_188HZ
# BITS_DLPF_CFG_98HZ
# BITS_DLPF_CFG_42HZ
# BITS_DLPF_CFG_20HZ
# BITS_DLPF_CFG_10HZ
# BITS_DLPF_CFG_5HZ
# BITS_DLPF_CFG_2100HZ_NOLPF
# returns 1 if an error occurred
# -----------------------------------------------------------------------------------------------

    def initialize(self, sample_rate_div = 1, low_pass_filter = 0x01):
        MPU_InitRegNum = 17
        MPU_Init_Data = [[0, 0]] * MPU_InitRegNum

        MPU_Init_Data = [
        [0x80, self.__MPUREG_PWR_MGMT_1],   # Reset Device
        [0x01, self.__MPUREG_PWR_MGMT_1],          # Clock Source
        [0x00, self.__MPUREG_PWR_MGMT_2],          # Enable Acc & Gyro
        [low_pass_filter, self.__MPUREG_CONFIG],   # Use DLPF set Gyroscope bandwidth 184Hz, temperature bandwidth 188Hz
        [0x18, self.__MPUREG_GYRO_CONFIG],         # +-2000dps
        [0x08, self.__MPUREG_ACCEL_CONFIG],        # +-4G
        [0x09, self.__MPUREG_ACCEL_CONFIG_2],      # Set Acc Data Rates, Enable Acc LPF , Bandwidth 184Hz
        [0x30, self.__MPUREG_INT_PIN_CFG],
        #[0x40, self.__MPUREG_I2C_MST_CTRL],       # I2C Speed 348 kHz
        #[0x20, self.__MPUREG_USER_CTRL],          # Enable AUX
        [0x20, self.__MPUREG_USER_CTRL],           # I2C Master mode
        [0x0D, self.__MPUREG_I2C_MST_CTRL],        #  I2C configuration multi-master  IIC 400KHz

        [self.__AK8963_I2C_ADDR, self.__MPUREG_I2C_SLV0_ADDR],  #Set the I2C slave addres of AK8963 and set for write.
        #[0x09, self.__MPUREG_I2C_SLV4_CTRL],
        #[0x81, self.__MPUREG_I2C_MST_DELAY_CTRL], #Enable I2C delay

        [self.__AK8963_CNTL2, self.__MPUREG_I2C_SLV0_REG], #I2C slave 0 register address from where to begin data transfer
        [0x01, self.__MPUREG_I2C_SLV0_DO], # Reset AK8963
        [0x81, self.__MPUREG_I2C_SLV0_CTRL],  #Enable I2C and set 1 byte

        [self.__AK8963_CNTL1, self.__MPUREG_I2C_SLV0_REG], #I2C slave 0 register address from where to begin data transfer
        [0x12, self.__MPUREG_I2C_SLV0_DO], # Register value to continuous measurement in 16bit
        [0x81, self.__MPUREG_I2C_SLV0_CTRL]  #Enable I2C and set 1 byte
        ]

        for i in range(0, MPU_InitRegNum):
            self.WriteReg(MPU_Init_Data[i][1], MPU_Init_Data[i][0])
            time.sleep(0.01) # I2C must slow down the write speed, otherwise it won't work

        self.set_acc_scale(self.__BITS_FS_16G)
        self.set_gyro_scale(self.__BITS_FS_2000DPS)

        self.calib_mag()

# -----------------------------------------------------------------------------------------------
#                                 ACCELEROMETER SCALE
# usage: call this function at startup, after initialization, to set the right range for the
# accelerometers. Suitable ranges are:
# BITS_FS_2G
# BITS_FS_4G
# BITS_FS_8G
# BITS_FS_16G
# returns the range set (2,4,8 or 16)
# -----------------------------------------------------------------------------------------------

    def set_acc_scale(self, scale):
        self.WriteReg(self.__MPUREG_ACCEL_CONFIG, scale)
        if (scale == self.__BITS_FS_2G):
            self.acc_divider = 16384.0
        elif (scale == self.__BITS_FS_4G):
            self.acc_divider = 8192.0
        elif (scale == self.__BITS_FS_8G):
            self.acc_divider = 4096.0
        elif (scale == self.__BITS_FS_16G):
            self.acc_divider = 2048.0

        temp_scale = self.ReadReg(self.__MPUREG_ACCEL_CONFIG)
        if (temp_scale == self.__BITS_FS_2G):
            temp_scale = 2
        elif (temp_scale == self.__BITS_FS_4G):
            temp_scale = 4
        elif (temp_scale == self.__BITS_FS_8G):
            temp_scale = 8
        elif (temp_scale == self.__BITS_FS_16G):
            temp_scale = 16

        return temp_scale

# -----------------------------------------------------------------------------------------------
#                                 GYROSCOPE SCALE
# usage: call this function at startup, after initialization, to set the right range for the
# gyroscopes. Suitable ranges are:
# BITS_FS_250DPS
# BITS_FS_500DPS
# BITS_FS_1000DPS
# BITS_FS_2000DPS
# returns the range set (250,500,1000 or 2000)
# -----------------------------------------------------------------------------------------------

    def set_gyro_scale(self, scale):
        self.WriteReg(self.__MPUREG_GYRO_CONFIG, scale)
        if (scale == self.__BITS_FS_250DPS):
            self.gyro_divider = 131.0
        elif (scale == self.__BITS_FS_500DPS):
            self.gyro_divider = 65.6
        elif (scale == self.__BITS_FS_1000DPS):
            self.gyro_divider = 32.8
        elif (scale == self.__BITS_FS_2000DPS):
            self.gyro_divider = 16.4

        temp_scale = self.ReadReg(self.__MPUREG_GYRO_CONFIG)
        if (temp_scale == self.__BITS_FS_250DPS):
            temp_scale = 250
        elif (temp_scale == self.__BITS_FS_500DPS):
            temp_scale = 500
        elif (temp_scale == self.__BITS_FS_1000DPS):
            temp_scale = 1000
        elif (temp_scale == self.__BITS_FS_2000DPS):
            temp_scale = 2000

        return temp_scale

# -----------------------------------------------------------------------------------------------
#                                 WHO AM I?
# usage: call this function to know if SPI is working correctly. It checks the I2C address of the
# mpu9250 which should be 104 when in SPI mode.
# returns the I2C address (104)
# -----------------------------------------------------------------------------------------------

    def whoami(self):
        return self.ReadReg(self.__MPUREG_WHOAMI)

# -----------------------------------------------------------------------------------------------
#                                 READ ACCELEROMETER
# usage: call this function to read accelerometer data. Axis represents selected axis:
# 0 -> X axis
# 1 -> Y axis
# 2 -> Z axis
# -----------------------------------------------------------------------------------------------

    def read_acc(self):
        response = self.ReadRegs(self.__MPUREG_ACCEL_XOUT_H, 6)

        for i in range(0, 3):
            data = self.byte_to_float(response[i*2:i*2+2])
            self.accelerometer_data[i] = self.G_SI*data/self.acc_divider

# -----------------------------------------------------------------------------------------------
#                                 READ GYROSCOPE
# usage: call this function to read gyroscope data. Axis represents selected axis:
# 0 -> X axis
# 1 -> Y axis
# 2 -> Z axis
# -----------------------------------------------------------------------------------------------

    def read_gyro(self):
        response = self.ReadRegs(self.__MPUREG_GYRO_XOUT_H, 6)

        for i in range(0, 3):
            data = self.byte_to_float(response[i*2:i*2+2])
            self.gyroscope_data[i] = (self.PI/180)*data/self.gyro_divider

# -----------------------------------------------------------------------------------------------
#                                 READ TEMPERATURE
# usage: call this function to read temperature data.
# returns the value in Celsius
# -----------------------------------------------------------------------------------------------

    def read_temp(self):
        response = self.ReadRegs(self.__MPUREG_TEMP_OUT_H, 2)

        #temp = response[0]*256.0 + response[1]
        temp = self.byte_to_float(response)
        self.temperature = (temp/340.0)+36.53

# -----------------------------------------------------------------------------------------------
#                                 READ ACCELEROMETER CALIBRATION
# usage: call this function to read accelerometer data. Axis represents selected axis:
# 0 -> X axis
# 1 -> Y axis
# 2 -> Z axis
# returns Factory Trim value
# -----------------------------------------------------------------------------------------------

    def calib_acc(self):
        temp_scale = self.ReadReg(self.__MPUREG_ACCEL_CONFIG)
        self.set_acc_scale(self.__BITS_FS_8G)

        response = self.ReadRegs(self.__MPUREG_SELF_TEST_X, 4)

        self.calib_data[0] = ((response[0] & 11100000) >> 3) | ((response[3] & 00110000) >> 4)
        self.calib_data[1] = ((response[1] & 11100000) >> 3) | ((response[3] & 00001100) >> 2)
        self.calib_data[2] = ((response[2] & 11100000) >> 3) | ((response[3] & 00000011))

        self.set_acc_scale(temp_scale)

# -----------------------------------------------------------------------------------------------

    def AK8963_whoami(self):
        self.WriteReg(self.__MPUREG_I2C_SLV0_ADDR, self.__AK8963_I2C_ADDR | self.__READ_FLAG) #Set the I2C slave addres of AK8963 and set for read.
        self.WriteReg(self.__MPUREG_I2C_SLV0_REG, self.__AK8963_WIA) #I2C slave 0 register address from where to begin data transfer
        self.WriteReg(self.__MPUREG_I2C_SLV0_CTRL, 0x81) #Read 1 byte from the magnetometer

        #self.WriteReg(self.__MPUREG_I2C_SLV0_CTRL, 0x81) # Enable I2C and set bytes
        time.sleep(0.01)

        return self.ReadReg(self.__MPUREG_EXT_SENS_DATA_00) # Read I2C

# -----------------------------------------------------------------------------------------------

    def calib_mag(self):
        # Set the I2C slave addres of AK8963 and set for read.
        self.WriteReg(self.__MPUREG_I2C_SLV0_ADDR, self.__AK8963_I2C_ADDR | self.__READ_FLAG)
        # I2C slave 0 register address from where to begin data transfer
        self.WriteReg(self.__MPUREG_I2C_SLV0_REG, self.__AK8963_ASAX)
        # Read 3 bytes from the magnetometer
        self.WriteReg(self.__MPUREG_I2C_SLV0_CTRL, 0x83)

        time.sleep(0.01)

        response = self.ReadRegs(self.__MPUREG_EXT_SENS_DATA_00, 3)

        for i in range(0, 3):
            self.magnetometer_ASA[i] = ((float(response[i]) - 128)/256 + 1) * self.__Magnetometer_Sensitivity_Scale_Factor

# -----------------------------------------------------------------------------------------------

    def read_mag(self):
        # Set the I2C slave addres of AK8963 and set for read.
        self.WriteReg(self.__MPUREG_I2C_SLV0_ADDR, self.__AK8963_I2C_ADDR | self.__READ_FLAG)
        # I2C slave 0 register address from where to begin data transfer
        self.WriteReg(self.__MPUREG_I2C_SLV0_REG, self.__AK8963_HXL)
        # Read 6 bytes from the magnetometer
        self.WriteReg(self.__MPUREG_I2C_SLV0_CTRL, 0x87)

        time.sleep(0.01)

        response = self.ReadRegs(self.__MPUREG_EXT_SENS_DATA_00, 7)
        #must start your read from AK8963A register 0x03 and read seven bytes so that upon read of ST2 register 0x09 the AK8963A will unlatch the data registers for the next measurement.
        for i in range(0, 3):
            data = self.byte_to_float_le(response[i*2:i*2+2])
            self.magnetometer_data[i] = data * self.magnetometer_ASA[i]

# -----------------------------------------------------------------------------------------------

    def read_all(self):
        # Send I2C command at first
        # Set the I2C slave addres of AK8963 and set for read.
        self.WriteReg(self.__MPUREG_I2C_SLV0_ADDR, self.__AK8963_I2C_ADDR | self.__READ_FLAG)
        # I2C slave 0 register address from where ; //Read 7 bytes from the magnetometerto begin data transfer
        self.WriteReg(self.__MPUREG_I2C_SLV0_REG, self.__AK8963_HXL)
        # Read 7 bytes from the magnetometer
        self.WriteReg(self.__MPUREG_I2C_SLV0_CTRL, 0x87)
        # must start your read from AK8963A register 0x03 and read seven bytes so that upon read of ST2 register 0x09 the AK8963A will unlatch the data registers for the next measurement.

        # time.sleep(0.001)
        response = self.ReadRegs(self.__MPUREG_ACCEL_XOUT_H, 21);

        # Get Accelerometer values
        for i in range(0, 3):
            data = self.byte_to_float(response[i*2:i*2+2])
            self.accelerometer_data[i] = self.G_SI*data/self.acc_divider

        # Get temperature
        i = 3
        temp = self.byte_to_float(response[i*2:i*2+2])
        self.temperature = (temp/340.0)+36.53

        # Get gyroscope values
        for i in range(4, 7):
            data = self.byte_to_float(response[i*2:i*2+2])
            self.gyroscope_data[i-4] =(self.PI/180)*data/self.gyro_divider

        # Get magnetometer values
        for i in range(7, 10):
            data = self.byte_to_float_le(response[i*2:i*2+2])
            self.magnetometer_data[i-7] = data * self.magnetometer_ASA[i-7]

# -----------------------------------------------------------------------------------------------
#                                          GET VALUES
# usage: call this functions to read and get values
# returns accel, gyro and mag values
# -----------------------------------------------------------------------------------------------

    def getMotion9(self):
        self.read_all()
        m9a = self.accelerometer_data
        m9g = self.gyroscope_data
        m9m = self.magnetometer_data

        return m9a, m9g, m9m

# -----------------------------------------------------------------------------------------------

    def getMotion6(self):
        self.read_acc()
        self.read_gyro()

        m6a = self.accelerometer_data
        m6g = self.gyroscope_data

        return m6a, m6g

# -----------------------------------------------------------------------------------------------

    def byte_to_float(self, input_buffer):
        byte_array = array.array("B", input_buffer)
        signed_16_bit_int, = struct.unpack(">h", byte_array)
        return float(signed_16_bit_int)

# -----------------------------------------------------------------------------------------------

    def byte_to_float_le(self, input_buffer):
        byte_array = array.array("B", input_buffer)
        signed_16_bit_int, = struct.unpack("<h", byte_array)
        return float(signed_16_bit_int)




