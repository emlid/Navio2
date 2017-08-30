"""
LSM9DS1 driver code is placed under the BSD license.
Copyright (c) 2017, Emlid Limited, emlid.com
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
import array
import struct

class LSM9DS1:

    __DEVICE_ACC_GYRO = 3
    __DEVICE_MAGNETOMETER = 2

    G_SI = 9.80665
    PI = 3.14159

    # who am I values
    __WHO_AM_I_ACC_GYRO = 0x68
    __WHO_AM_I_MAG = 0x3D
    
    # Accelerometer and Gyroscope registers
    __LSM9DS1XG_ACT_THS = 0x04
    __LSM9DS1XG_ACT_DUR = 0x05
    __LSM9DS1XG_INT_GEN_CFG_XL = 0x06
    __LSM9DS1XG_INT_GEN_THS_X_XL = 0x07
    __LSM9DS1XG_INT_GEN_THS_Y_XL = 0x08
    __LSM9DS1XG_INT_GEN_THS_Z_XL = 0x09
    __LSM9DS1XG_INT_GEN_DUR_XL = 0x0A
    __LSM9DS1XG_REFERENCE_G = 0x0B
    __LSM9DS1XG_INT1_CTRL = 0x0C
    __LSM9DS1XG_INT2_CTRL = 0x0D
    __LSM9DS1XG_WHO_AM_I = 0x0F # should return = 0x68
    __LSM9DS1XG_CTRL_REG1_G = 0x10
    __LSM9DS1XG_CTRL_REG2_G = 0x11
    __LSM9DS1XG_CTRL_REG3_G = 0x12
    __LSM9DS1XG_ORIENT_CFG_G = 0x13
    __LSM9DS1XG_INT_GEN_SRC_G = 0x14
    __LSM9DS1XG_OUT_TEMP_L = 0x15
    __LSM9DS1XG_OUT_TEMP_H = 0x16
    __LSM9DS1XG_STATUS_REG = 0x17
    __LSM9DS1XG_OUT_X_L_G = 0x18
    __LSM9DS1XG_OUT_X_H_G = 0x19
    __LSM9DS1XG_OUT_Y_L_G = 0x1A
    __LSM9DS1XG_OUT_Y_H_G = 0x1B
    __LSM9DS1XG_OUT_Z_L_G = 0x1C
    __LSM9DS1XG_OUT_Z_H_G = 0x1D
    __LSM9DS1XG_CTRL_REG4 = 0x1E
    __LSM9DS1XG_CTRL_REG5_XL = 0x1F
    __LSM9DS1XG_CTRL_REG6_XL = 0x20
    __LSM9DS1XG_CTRL_REG7_XL = 0x21
    __LSM9DS1XG_CTRL_REG8 = 0x22
    __LSM9DS1XG_CTRL_REG9 = 0x23
    __LSM9DS1XG_CTRL_REG10 = 0x24
    __LSM9DS1XG_INT_GEN_SRC_XL = 0x26
    __LSM9DS1XG_OUT_X_L_XL = 0x28
    __LSM9DS1XG_OUT_X_H_XL = 0x29
    __LSM9DS1XG_OUT_Y_L_XL = 0x2A
    __LSM9DS1XG_OUT_Y_H_XL = 0x2B
    __LSM9DS1XG_OUT_Z_L_XL = 0x2C
    __LSM9DS1XG_OUT_Z_H_XL = 0x2D
    __LSM9DS1XG_FIFO_CTRL = 0x2E
    __LSM9DS1XG_FIFO_SRC = 0x2F
    __LSM9DS1XG_INT_GEN_CFG_G = 0x30
    __LSM9DS1XG_INT_GEN_THS_XH_G = 0x31
    __LSM9DS1XG_INT_GEN_THS_XL_G = 0x32
    __LSM9DS1XG_INT_GEN_THS_YH_G = 0x33
    __LSM9DS1XG_INT_GEN_THS_YL_G = 0x34
    __LSM9DS1XG_INT_GEN_THS_ZH_G = 0x35
    __LSM9DS1XG_INT_GEN_THS_ZL_G = 0x36
    __LSM9DS1XG_INT_GEN_DUR_G = 0x37
    
    # Magnetometer registers
    __LSM9DS1M_OFFSET_X_REG_L_M = 0x05
    __LSM9DS1M_OFFSET_X_REG_H_M = 0x06
    __LSM9DS1M_OFFSET_Y_REG_L_M = 0x07
    __LSM9DS1M_OFFSET_Y_REG_H_M = 0x08
    __LSM9DS1M_OFFSET_Z_REG_L_M = 0x09
    __LSM9DS1M_OFFSET_Z_REG_H_M = 0x0A
    __LSM9DS1M_WHO_AM_I = 0x0F # should return = 0x3D
    __LSM9DS1M_CTRL_REG1_M = 0x20
    __LSM9DS1M_CTRL_REG2_M = 0x21
    __LSM9DS1M_CTRL_REG3_M = 0x22
    __LSM9DS1M_CTRL_REG4_M = 0x23
    __LSM9DS1M_CTRL_REG5_M = 0x24
    __LSM9DS1M_STATUS_REG_M = 0x27
    __LSM9DS1M_OUT_X_L_M = 0x28
    __LSM9DS1M_OUT_X_H_M = 0x29
    __LSM9DS1M_OUT_Y_L_M = 0x2A
    __LSM9DS1M_OUT_Y_H_M = 0x2B
    __LSM9DS1M_OUT_Z_L_M = 0x2C
    __LSM9DS1M_OUT_Z_H_M = 0x2D
    __LSM9DS1M_INT_CFG_M = 0x30
    __LSM9DS1M_INT_SRC_M = 0x31
    __LSM9DS1M_INT_THS_L_M = 0x32
    __LSM9DS1M_INT_THS_H_M = 0x33
    
    # Configuration bits Accelerometer and Gyroscope
    __BITS_XEN_G = 0x08
    __BITS_YEN_G = 0x10
    __BITS_ZEN_G = 0x20
    __BITS_XEN_XL = 0x08
    __BITS_YEN_XL = 0x10
    __BITS_ZEN_XL = 0x20
    __BITS_ODR_G_14900mHZ = 0x20
    __BITS_ODR_G_59500mHZ = 0x40
    __BITS_ODR_G_119HZ = 0x60
    __BITS_ODR_G_238HZ = 0x80
    __BITS_ODR_G_476HZ = 0xA0
    __BITS_ODR_G_952HZ = 0xC0
    __BITS_ODR_XL_10HZ = 0x20
    __BITS_ODR_XL_50HZ = 0x40
    __BITS_ODR_XL_119HZ = 0x60
    __BITS_ODR_XL_238HZ = 0x80
    __BITS_ODR_XL_476HZ = 0xA0
    __BITS_ODR_XL_952HZ = 0xC0
    __BITS_FS_G_MASK = 0xE3
    __BITS_FS_G_245DPS = 0x00
    __BITS_FS_G_500DPS = 0x08
    __BITS_FS_G_2000DPS = 0x18
    __BITS_FS_XL_MASK = 0xE7
    __BITS_FS_XL_2G = 0x00
    __BITS_FS_XL_4G = 0x10
    __BITS_FS_XL_8G = 0x18
    __BITS_FS_XL_16G = 0x08
    
    # Configuration bits Magnetometer
    __BITS_TEMP_COMP = 0x80
    __BITS_OM_LOW = 0x00
    __BITS_OM_MEDIUM = 0x20
    __BITS_OM_HIGH = 0x40
    __BITS_OM_ULTRA_HIGH = 0x60
    __BITS_ODR_M_625mHZ = 0x00
    __BITS_ODR_M_1250mHZ = 0x04
    __BITS_ODR_M_250mHZ = 0x08
    __BITS_ODR_M_5HZ = 0x0C
    __BITS_ODR_M_10HZ = 0x10
    __BITS_ODR_M_20HZ = 0x14
    __BITS_ODR_M_40HZ = 0x18
    __BITS_ODR_M_80HZ = 0x1C
    __BITS_FS_M_MASK = 0x0C
    __BITS_FS_M_4Gs = 0x00
    __BITS_FS_M_8Gs = 0x20
    __BITS_FS_M_12Gs = 0x40
    __BITS_FS_M_16Gs = 0x60
    __BITS_MD_CONTINUOUS = 0x00
    __BITS_MD_SINGLE = 0x01
    __BITS_MD_POWERDOWN = 0x02
    __BITS_OMZ_LOW = 0x00
    __BITS_OMZ_MEDIUM = 0x04
    __BITS_OMZ_HIGH = 0x08
    __BITS_OMZ_ULTRA_HIGH = 0x0C

    __READ_FLAG = 0x80
    __MULTIPLE_READ = 0x40

    def __init__(self, spi_bus_number = 0):
        self.bus = spidev.SpiDev()
        self.spi_bus_number = spi_bus_number
        self.gyro_scale = None
        self.acc_scale = None
        self.mag_scale = None
        self.gyroscope_data = [0.0, 0.0, 0.0]
        self.accelerometer_data = [0.0, 0.0, 0.0]
        self.magnetometer_data = [0.0, 0.0, 0.0]
        self.temperature = 0.0

    def bus_open(self, dev_number):
        self.bus.open(self.spi_bus_number, dev_number)
        self.bus.max_speed_hz = 10000000

    def testConnection(self):
        responseXG = self.readReg(self.__DEVICE_ACC_GYRO, self.__LSM9DS1XG_WHO_AM_I)
        responseM =  self.readReg(self.__DEVICE_MAGNETOMETER, self.__LSM9DS1M_WHO_AM_I)
        if (responseXG == self.__WHO_AM_I_ACC_GYRO and responseM == self.__WHO_AM_I_MAG):
            return True
        return False

    def writeReg(self, dev_number, reg_address, data):
        self.bus_open(dev_number)
        tx = [reg_address, data]
        rx = self.bus.xfer2(tx)
        self.bus.close()
        return rx

    def readReg(self, dev_number, reg_address):
        self.bus_open(dev_number)
        tx = [reg_address | self.__READ_FLAG, 0x00]
        rx = self.bus.xfer2(tx)
        self.bus.close()
        return rx[1]

    def readRegs(self, dev_number, reg_address, length):
        self.bus_open(dev_number)
        tx = [0] * (length + 1)

        tx[0] = reg_address | self.__READ_FLAG
        if dev_number==self.__DEVICE_MAGNETOMETER:
            tx[0] |= self.__MULTIPLE_READ

        rx = self.bus.xfer2(tx)

        self.bus.close()
        return rx[1:len(rx)]

    def initialize(self):
        #--------Accelerometer and Gyroscope---------
        # enable the 3-axes of the gyroscope
        self.writeReg(self.__DEVICE_ACC_GYRO, self.__LSM9DS1XG_CTRL_REG4, self.__BITS_XEN_G | self.__BITS_YEN_G | self.__BITS_ZEN_G)
        # configure the gyroscope
        self.writeReg(self.__DEVICE_ACC_GYRO, self.__LSM9DS1XG_CTRL_REG1_G, self.__BITS_ODR_G_952HZ | self.__BITS_FS_G_2000DPS)
        time.sleep(0.1)

        # enable the three axes of the accelerometer
        self.writeReg(self.__DEVICE_ACC_GYRO, self.__LSM9DS1XG_CTRL_REG5_XL, self.__BITS_XEN_XL | self.__BITS_YEN_XL | self.__BITS_ZEN_XL)
        # configure the accelerometer-specify bandwidth selection with Abw
        self.writeReg(self.__DEVICE_ACC_GYRO, self.__LSM9DS1XG_CTRL_REG6_XL, self.__BITS_ODR_XL_952HZ | self.__BITS_FS_XL_16G)
        time.sleep(0.1)

        #------------Magnetometer----------------
        self.writeReg(self.__DEVICE_MAGNETOMETER, self.__LSM9DS1M_CTRL_REG1_M, self.__BITS_TEMP_COMP | self.__BITS_OM_HIGH | self.__BITS_ODR_M_80HZ)
        self.writeReg(self.__DEVICE_MAGNETOMETER, self.__LSM9DS1M_CTRL_REG2_M, self.__BITS_FS_M_16Gs)
        # continuous conversion mode
        self.writeReg(self.__DEVICE_MAGNETOMETER, self.__LSM9DS1M_CTRL_REG3_M, self.__BITS_MD_CONTINUOUS)
        self.writeReg(self.__DEVICE_MAGNETOMETER, self.__LSM9DS1M_CTRL_REG4_M, self.__BITS_OMZ_HIGH)
        self.writeReg(self.__DEVICE_MAGNETOMETER, self.__LSM9DS1M_CTRL_REG5_M, 0x00 )
        time.sleep(0.1)

        self.set_gyro_scale(self.__BITS_FS_G_2000DPS)
        self.set_acc_scale(self.__BITS_FS_XL_16G)
        self.set_mag_scale(self.__BITS_FS_M_16Gs)


    def set_gyro_scale(self, scale):
        reg = self.__BITS_FS_G_MASK & self.readReg(self.__DEVICE_ACC_GYRO, self.__LSM9DS1XG_CTRL_REG1_G)
        self.writeReg(self.__DEVICE_ACC_GYRO, self.__LSM9DS1XG_CTRL_REG1_G,reg | scale)
        if (scale == self.__BITS_FS_G_245DPS):
            self.gyro_scale = 0.00875
        if (scale == self.__BITS_FS_G_500DPS):
            self.gyro_scale = 0.0175
        if (scale == self.__BITS_FS_G_2000DPS):
            self.gyro_scale = 0.07

    def set_acc_scale(self, scale):
        reg = self.__BITS_FS_XL_MASK & self.readReg(self.__DEVICE_ACC_GYRO, self.__LSM9DS1XG_CTRL_REG6_XL)
        self.writeReg(self.__DEVICE_ACC_GYRO, self.__LSM9DS1XG_CTRL_REG6_XL, reg | scale)
        if (scale == self.__BITS_FS_XL_2G):
            self.acc_scale = 0.000061
        if (scale == self.__BITS_FS_XL_4G):
            self.acc_scale = 0.000122
        if (scale == self.__BITS_FS_XL_8G):
            self.acc_scale = 0.000244
        if (scale == self.__BITS_FS_XL_16G):
            self.acc_scale = 0.000732

    def set_mag_scale(self, scale):
        reg = self.__BITS_FS_M_MASK & self.readReg(self.__DEVICE_MAGNETOMETER, self.__LSM9DS1M_CTRL_REG2_M)
        self.writeReg(self.__DEVICE_MAGNETOMETER, self.__LSM9DS1M_CTRL_REG2_M, reg | scale)
        if (scale == self.__BITS_FS_M_4Gs):
            self.mag_scale = 0.00014
        if (scale == self.__BITS_FS_M_8Gs):
            self.mag_scale = 0.00029
        if (scale == self.__BITS_FS_M_12Gs):
            self.mag_scale = 0.00043
        if (scale == self.__BITS_FS_M_16Gs):
            self.mag_scale = 0.00058

    def read_acc(self):
        # Read accelerometer
        response = self.readRegs(self.__DEVICE_ACC_GYRO, self.__LSM9DS1XG_OUT_X_L_XL, 6)
        for i in xrange(3):
            self.accelerometer_data[i] = self.G_SI * (self.byte_to_float_le(response[2*i:2*i+2]) * self.acc_scale)

        self.accelerometer_data = [-self.accelerometer_data[1], -self.accelerometer_data[0], self.accelerometer_data[2]]

    def read_gyro(self):
        # Read gyroscope
        response = self.readRegs(self.__DEVICE_ACC_GYRO, self.__LSM9DS1XG_OUT_X_L_G, 6)
        for i in xrange(3):
            self.gyroscope_data[i] = (self.PI / 180.0) * (self.byte_to_float_le(response[2*i:2*i+2]) * self.gyro_scale) 

        self.gyroscope_data = [-self.gyroscope_data[1], -self.gyroscope_data[0], self.gyroscope_data[2]]

    def read_mag(self):
        # Read magnetometer
        response = self.readRegs(self.__DEVICE_MAGNETOMETER, self.__LSM9DS1M_OUT_X_L_M, 6)
        for i in xrange(3):
            self.magnetometer_data[i] = 100.0 * (self.byte_to_float_le(response[2*i:2*i+2]) * self.mag_scale)

        self.magnetometer_data[1] *=-1
        self.magnetometer_data[2] *=-1

    def read_temp(self):
        # Read temperature
        response = self.readRegs(self.__DEVICE_ACC_GYRO, self.__LSM9DS1XG_OUT_TEMP_L, 2)
        self.temperature = self.byte_to_float_le(response) / 256.0 + 25.0 

    def read_all(self):
        # Read temperature
        response = self.readRegs(self.__DEVICE_ACC_GYRO, self.__LSM9DS1XG_OUT_TEMP_L, 2)
        self.temperature = self.byte_to_float_le(response) / 256.0 + 25.0

        # Read accelerometer
        response = self.readRegs(self.__DEVICE_ACC_GYRO, self.__LSM9DS1XG_OUT_X_L_XL, 6)
        for i in xrange(3):
            self.accelerometer_data[i] = self.G_SI * (self.byte_to_float_le(response[2*i:2*i+2]) * self.acc_scale)

        # Read gyroscope
        response = self.readRegs(self.__DEVICE_ACC_GYRO, self.__LSM9DS1XG_OUT_X_L_G, 6)
        for i in xrange(3):
            self.gyroscope_data[i] = (self.PI / 180.0) * (self.byte_to_float_le(response[2*i:2*i+2]) * self.gyro_scale) 

        # Read magnetometer
        response = self.readRegs(self.__DEVICE_MAGNETOMETER, self.__LSM9DS1M_OUT_X_L_M, 6)
        for i in xrange(3):
            self.magnetometer_data[i] = 100.0 * (self.byte_to_float_le(response[2*i:2*i+2]) * self.mag_scale)

        # Change rotation of LSM9DS1 like in MPU-9250
        self.rotate()

    def getMotion9(self):
        self.read_all()
        m9a = self.accelerometer_data
        m9g = self.gyroscope_data
        m9m = self.magnetometer_data

        return m9a, m9g, m9m

    def getMotion6(self):
        self.read_acc()
        self.read_gyro()

        m6a = self.accelerometer_data
        m6g = self.gyroscope_data

        return m6a, m6g

    def byte_to_float(self, input_buffer):
        byte_array = array.array("B", input_buffer)
        signed_16_bit_int, = struct.unpack(">h", byte_array)
        return float(signed_16_bit_int)

    def byte_to_float_le(self, input_buffer):
        byte_array = array.array("B", input_buffer)
        signed_16_bit_int, = struct.unpack("<h", byte_array)
        return float(signed_16_bit_int)


    def rotate(self):
        self.accelerometer_data = [-self.accelerometer_data[1], -self.accelerometer_data[0], self.accelerometer_data[2]]

        self.gyroscope_data = [-self.gyroscope_data[1], -self.gyroscope_data[0], self.gyroscope_data[2]]

        self.magnetometer_data[1] *=-1
        self.magnetometer_data[2] *=-1
