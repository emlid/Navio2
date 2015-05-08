/*
MB85RC256 driver code is placed under the BSD license.
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

#include "MB85RC256.h"

MB85RC256::MB85RC256(uint8_t address)
{
    this->device_address = address;
}

uint8_t MB85RC256::readByte(uint16_t register_address, uint8_t* data)
{
    return MB85RC256::readBytes(register_address, 1, data);
}

uint8_t MB85RC256::writeByte(uint16_t register_address, uint8_t data)
{
	return MB85RC256::writeBytes(register_address, 1, &data);
}

uint8_t MB85RC256::writeBytes(uint16_t register_address, uint8_t length, uint8_t* data)
{
    uint8_t reg_address_low = register_address; // lower part of the address
    uint8_t reg_address_high = register_address >> 8; // higher part of the address
    uint8_t msg[length + 1];

    msg[0] = reg_address_low;

    for (int i = 0; i < length; i++) {
        msg[i + 1] = data[i];
    }

	return I2Cdev::writeBytes(this->device_address, reg_address_high, length + 1, msg);
}

uint8_t MB85RC256::readBytes(uint16_t register_address, uint8_t length, uint8_t* data)
{
    uint8_t reg_address_low = register_address; // lower part of the address
    uint8_t reg_address_high = register_address >> 8; // higher part of the address

    I2Cdev::writeByte(this->device_address, reg_address_high, reg_address_low); // set the read pointer to the desired address

	return I2Cdev::readBytesNoRegAddress(this->device_address, length, data);
}

