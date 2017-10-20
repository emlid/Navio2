/*
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
*/

#include "MS5611.h"

/** MS5611 constructor.
 * @param address I2C address
 * @see MS5611_DEFAULT_ADDRESS
 */
MS5611::MS5611(uint8_t address) {
    this->devAddr = address;
}

/** Power on and prepare for general usage.
 * This method reads coefficients stored in PROM.
 */
void MS5611::initialize() {
    // Reading 6 calibration data values
    uint8_t buff[2];
    I2Cdev::readBytes(devAddr, MS5611_RA_C1, 2, buff);
    C1 = buff[0]<<8 | buff[1];
    I2Cdev::readBytes(devAddr, MS5611_RA_C2, 2, buff);
    C2 = buff[0]<<8 | buff[1];
    I2Cdev::readBytes(devAddr, MS5611_RA_C3, 2, buff);
    C3 = buff[0]<<8 | buff[1];
    I2Cdev::readBytes(devAddr, MS5611_RA_C4, 2, buff);
    C4 = buff[0]<<8 | buff[1];
    I2Cdev::readBytes(devAddr, MS5611_RA_C5, 2, buff);
    C5 = buff[0]<<8 | buff[1];
    I2Cdev::readBytes(devAddr, MS5611_RA_C6, 2, buff);
    C6 = buff[0]<<8 | buff[1];

    update();
}

/** Verify the I2C connection.
 * @return True if connection is valid, false otherwise
 */
bool MS5611::testConnection() {
    uint8_t data;
    int8_t status = I2Cdev::readByte(devAddr, MS5611_RA_C0, &data);
    if (status > 0)
        return true;
    else
        return false;
}

/** Initiate the process of pressure measurement
 * @param OSR value
 * @see MS5611_RA_D1_OSR_4096
 */
void MS5611::refreshPressure(uint8_t OSR) {
    I2Cdev::writeBytes(devAddr, OSR, 0, 0);
}

/** Read pressure value
 */
void MS5611::readPressure() {
    //
    uint8_t buffer[3];
    I2Cdev::readBytes(devAddr, MS5611_RA_ADC, 3, buffer);
    D1 = (buffer[0] << 16) | (buffer[1] << 8) | buffer[2];
}

/** Initiate the process of temperature measurement
 * @param OSR value
 * @see MS5611_RA_D2_OSR_4096
 */
void MS5611::refreshTemperature(uint8_t OSR) {
	I2Cdev::writeBytes(devAddr, OSR, 0, 0);
}

/** Read temperature value
 */
void MS5611::readTemperature() {
	uint8_t buffer[3];
	I2Cdev::readBytes(devAddr, MS5611_RA_ADC, 3, buffer);
	D2 = (buffer[0] << 16) | (buffer[1] << 8) | buffer[2];
}

/** Calculate temperature and pressure calculations and perform compensation
 *  More info about these calculations is available in the datasheet.
 */
void MS5611::calculatePressureAndTemperature() {
    float dT = D2 - C5 * pow(2, 8);
    TEMP = (2000 + ((dT * C6) / pow(2, 23)));
    float OFF = C2 * pow(2, 16) + (C4 * dT) / pow(2, 7);
    float SENS = C1 * pow(2, 15) + (C3 * dT) / pow(2, 8);

    float T2, OFF2, SENS2;

    if (TEMP >= 2000)
    {
        T2 = 0;
        OFF2 = 0;
        SENS2 = 0;
    }
    if (TEMP < 2000)
    {
        T2 = dT * dT / pow(2, 31);
        OFF2 = 5 * pow(TEMP - 2000, 2) / 2;
        SENS2 = OFF2 / 2;
    }
    if (TEMP < -1500)
    {
        OFF2 = OFF2 + 7 * pow(TEMP + 1500, 2);
        SENS2 = SENS2 + 11 * pow(TEMP + 1500, 2) / 2;
    }

    TEMP = TEMP - T2;
    OFF = OFF - OFF2;
    SENS = SENS - SENS2;

    // Final calculations
    PRES = ((D1 * SENS) / pow(2, 21) - OFF) / pow(2, 15) / 100;
    TEMP = TEMP / 100;
}

/** Perform pressure and temperature reading and calculation at once.
 *  Contains sleeps, better perform operations separately.
 */
void MS5611::update() {
    refreshPressure();
    usleep(10000); // Waiting for pressure data ready
    readPressure();

    refreshTemperature();
    usleep(10000); // Waiting for temperature data ready
    readTemperature();

    calculatePressureAndTemperature();
}

/** Get calculated temperature value
 @return Temperature in degrees of Celsius
 */
float MS5611::getTemperature() {
    return TEMP;
}

/** Get calculated pressure value
 @return Pressure in millibars
 */
float MS5611::getPressure() {
	return PRES;
}
