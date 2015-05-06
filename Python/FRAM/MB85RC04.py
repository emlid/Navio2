"""
MB85RC04 driver code is placed under the BSD license.
Copyright (c) 2014, Emlid Limited, www.emlid.com
Written by Vladislav Zakharov
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


from smbus import SMBus

class MB85RC04:
	def __init__(self, I2C_bus_number = 1,  address = 0x50):
		self.bus = SMBus(I2C_bus_number)
		self.address = address


	def readByte(self, registerAddress):
		if(registerAddress > 255):
			self.address = self.address | 1
			registerAddress = registerAddress - 256
		else:
			self.address = self.address & 0xFE	
		return self.bus.read_byte_data(self.address, registerAddress)
	
	def writeByte(self, registerAddress, data):
		if(registerAddress > 255):
                        self.address = self.address | 1
                        registerAddress = registerAddress - 256
                else:
                        self.address = self.address & 0xFE
                self.bus.write_byte_data(self.address, registerAddress, data)

	def readBytes(self, registerAddress):
                if(registerAddress > 255):
                        self.address = self.address | 1
                        registerAddress = registerAddress - 256
                else:
                        self.address = self.address & 0xFE
                return self.bus.read_i2c_block_data(self.address, registerAddress)
	
	def writeBytes(self, registerAddress, data):
                if(registerAddress > 255):
                        self.address = self.address | 1
                        registerAddress = registerAddress - 256
                else:
                        self.address = self.address & 0xFE
                self.bus.write_i2c_block_data(self.address, registerAddress, data)
