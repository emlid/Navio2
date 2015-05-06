/*
MB85RC04 driver code is placed under the BSD license.
Written by Egor Fedorov (egor.fedorov@emlid.com)
Copyright (c) 2014, Emlid Limited
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
*/

#include "MB85RC04.h"

MB85RC04::MB85RC04(uint8_t address)
{
    this->device_address = address;
}

uint8_t MB85RC04::readByte(uint16_t register_address, uint8_t* data)
{
	bool ninth_bit = register_address & 0x100;
	uint8_t dev_address = device_address | ninth_bit;
	return I2Cdev::readByte(dev_address, register_address, data);
}

uint8_t MB85RC04::writeByte(uint16_t register_address, uint8_t data)
{
	bool ninth_bit = register_address & 0x100;
	uint8_t dev_address = device_address | ninth_bit;
	return I2Cdev::writeByte(dev_address, register_address, data);
}

uint8_t MB85RC04::writeBytes(uint16_t register_address, uint8_t length, uint8_t* data)
{
	bool ninth_bit = register_address & 0x100;
	uint8_t dev_address = device_address | ninth_bit;

	return I2Cdev::writeBytes(dev_address, register_address, 3, data);
}

uint8_t MB85RC04::readBytes(uint16_t register_address, uint8_t length, uint8_t* data)
{
	bool ninth_bit = register_address & 0x100;
	uint8_t dev_address = device_address | ninth_bit;

	return I2Cdev::readBytes(dev_address, register_address, 3, data);
}
