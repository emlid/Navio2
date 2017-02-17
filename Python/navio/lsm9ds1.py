"""
MPU9250 driver code is placed under the BSD license.
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

class LSM9DS1:
    __READ_FLAG   = 0x80
    __LSM9DS1M_WHO_AM_I_REG = 0x0F  # should return 0x3D

    def __init__(self, spi_bus_number = 0, spi_dev_number = 2):
        self.bus = spidev.SpiDev()
        self.spi_bus_number = spi_bus_number
        self.spi_dev_number = spi_dev_number

    def testConnection(self):
        sensor_id = self.register_read(self.__LSM9DS1M_WHO_AM_I_REG);

        if sensor_id == 0x3d:
            return True

        return False

    def register_read(self, reg_address):
        self.bus.open(self.spi_bus_number, self.spi_dev_number)
        tx = [reg_address | self.__READ_FLAG, 0x00]
        rx = self.bus.xfer2(tx)
        self.bus.close()
        return rx[1]
