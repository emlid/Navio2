"""
MS5611 driver code is placed under the BSD license.
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

import time

from smbus import SMBus

class MS5611:

	__MS5611_ADDRESS_CSB_LOW  = 0x76
	__MS5611_ADDRESS_CSB_HIGH = 0x77
	__MS5611_DEFAULT_ADDRESS  = 0x77

	__MS5611_RA_ADC           = 0x00
	__MS5611_RA_RESET         = 0x1E

	__MS5611_RA_C0            = 0xA0
	__MS5611_RA_C1            = 0xA2
	__MS5611_RA_C2            = 0xA4
	__MS5611_RA_C3            = 0xA6
	__MS5611_RA_C4            = 0xA8
	__MS5611_RA_C5            = 0xAA
	__MS5611_RA_C6            = 0xAC
	__MS5611_RA_C7            = 0xAE

	__MS5611_RA_D1_OSR_256    = 0x40
	__MS5611_RA_D1_OSR_512    = 0x42
	__MS5611_RA_D1_OSR_1024   = 0x44
	__MS5611_RA_D1_OSR_2048   = 0x46
	__MS5611_RA_D1_OSR_4096   = 0x48

	__MS5611_RA_D2_OSR_256    = 0x50
	__MS5611_RA_D2_OSR_512    = 0x52
	__MS5611_RA_D2_OSR_1024   = 0x54
	__MS5611_RA_D2_OSR_2048   = 0x56
	__MS5611_RA_D2_OSR_4096   = 0x58

	def __init__(self, I2C_bus_number = 1, address = 0x77):
		self.bus = SMBus(I2C_bus_number)
		self.address = address
		self.C1 = 0
		self.C2 = 0
		self.C3 = 0
		self.C4 = 0
		self.C5 = 0
		self.C6 = 0
		self.D1 = 0
		self.D2 = 0
		self.TEMP = 0.0 # Calculated temperature
		self.PRES = 0.0 # Calculated Pressure

	def initialize(self):
		## The MS6511 Sensor stores 6 values in the EPROM memory that we need in order to calculate the actual temperature and pressure
		## These values are calculated/stored at the factory when the sensor is calibrated.
		##      I probably could have used the read word function instead of the whole block, but I wanted to keep things consistent.
		C1 = self.bus.read_i2c_block_data(self.address, self.__MS5611_RA_C1) #Pressure Sensitivity
		#time.sleep(0.05)
		C2 = self.bus.read_i2c_block_data(self.address, self.__MS5611_RA_C2) #Pressure Offset
		#time.sleep(0.05)
		C3 = self.bus.read_i2c_block_data(self.address, self.__MS5611_RA_C3) #Temperature coefficient of pressure sensitivity
		#time.sleep(0.05)
		C4 = self.bus.read_i2c_block_data(self.address, self.__MS5611_RA_C4) #Temperature coefficient of pressure offset
		#time.sleep(0.05)
		C5 = self.bus.read_i2c_block_data(self.address, self.__MS5611_RA_C5) #Reference temperature
		#time.sleep(0.05)
		C6 = self.bus.read_i2c_block_data(self.address, self.__MS5611_RA_C6) #Temperature coefficient of the temperature

		## Again here we are converting the 2 8bit packages into a single decimal
		self.C1 = C1[0] * 256.0 + C1[1]
		self.C2 = C2[0] * 256.0 + C2[1]
		self.C3 = C3[0] * 256.0 + C3[1]
		self.C4 = C4[0] * 256.0 + C4[1]
		self.C5 = C5[0] * 256.0 + C5[1]
		self.C6 = C6[0] * 256.0 + C6[1]

		self.update()

	def refreshPressure(self, OSR = __MS5611_RA_D1_OSR_4096):
		self.bus.write_byte(self.address, OSR)

	def refreshTemperature(self, OSR = __MS5611_RA_D2_OSR_4096):
		self.bus.write_byte(self.address, OSR)

	def readPressure(self):
		D1 = self.bus.read_i2c_block_data(self.address, self.__MS5611_RA_ADC)
		self.D1 = D1[0] * 65536 + D1[1] * 256.0 + D1[2]

	def readTemperature(self):
		D2 = self.bus.read_i2c_block_data(self.address, self.__MS5611_RA_ADC)
		self.D2 = D2[0] * 65536 + D2[1] * 256.0 + D2[2]

	def calculatePressureAndTemperature(self):
		dT = self.D2 - self.C5 * 2**8
		self.TEMP = 2000 + dT * self.C6 / 2**23

		OFF = self.C2 * 2**16 + (self.C4 * dT) / 2**7
		SENS = self.C1 * 2**15 + (self.C3 * dT) / 2**8

		if (self.TEMP >= 2000):
			T2 = 0
			OFF2 = 0
			SENS2 = 0
		elif (self.TEMP < 2000):
			T2 = dT * dT / 2**31
			OFF2 = 5 * ((self.TEMP - 2000) ** 2) / 2
			SENS2 = OFF2 / 2
		elif (self.TEMP < -1500):
			OFF2 = OFF2 + 7 * ((self.TEMP + 1500) ** 2)
			SENS2 = SENS2 + 11 * (self.TEMP + 1500) ** 2 / 2

		self.TEMP = self.TEMP - T2
		OFF = OFF - OFF2
		SENS = SENS - SENS2

		self.PRES = (self.D1 * SENS / 2**21 - OFF) / 2**15

		self.TEMP = self.TEMP / 100 # Temperature updated
		self.PRES = self.PRES / 100 # Pressure updated

	def returnPressure(self):
		return self.PRES

	def returnTemperature(self):
		return self.TEMP

	def update(self):
		self.refreshPressure()
		time.sleep(0.01) # Waiting for pressure data ready
		self.readPressure()

		self.refreshTemperature()
		time.sleep(0.01) # Waiting for temperature data ready
		self.readTemperature()

		self.calculatePressureAndTemperature()
