/*
ADS1115 driver code is placed under the BSD license.
Copyright (c) 2014, Emlid Limited, www.emlid.com
Written by Georgii Staroselskii
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

#ifndef _ADS1115_H_
#define _ADS1115_H_

#include <string>

#ifdef DEBUG_ADS1115
#define debug(M, ...) fprintf(stderr, "DEBUG %s:%d: " M "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#else
#define debug(M, ...)
#endif

#define ADS1115_ADDRESS_ADDR_GND    0x48 // address pin low (GND)
#define ADS1115_ADDRESS_ADDR_VDD    0x49 // address pin high (VCC)
#define ADS1115_ADDRESS_ADDR_SDA    0x4A // address pin tied to SDA pin
#define ADS1115_ADDRESS_ADDR_SCL    0x4B // address pin tied to SCL pin
#define ADS1115_DEFAULT_ADDRESS     ADS1115_ADDRESS_ADDR_GND

#define ADS1115_RA_CONVERSION       0x00
#define ADS1115_RA_CONFIG           0x01
#define ADS1115_RA_LO_THRESH        0x02
#define ADS1115_RA_HI_THRESH        0x03

#define ADS1115_OS_SHIFT            15
#define ADS1115_OS_INACTIVE         0x00 << ADS1115_OS_SHIFT
#define ADS1115_OS_ACTIVE           0x01 << ADS1115_OS_SHIFT

#define ADS1115_MUX_SHIFT            12
#define ADS1115_MUX_P0_N1           0x00 << ADS1115_MUX_SHIFT /* default */
#define ADS1115_MUX_P0_N3           0x01 << ADS1115_MUX_SHIFT
#define ADS1115_MUX_P1_N3           0x02 << ADS1115_MUX_SHIFT
#define ADS1115_MUX_P2_N3           0x03 << ADS1115_MUX_SHIFT
#define ADS1115_MUX_P0_NG           0x04 << ADS1115_MUX_SHIFT
#define ADS1115_MUX_P1_NG           0x05 << ADS1115_MUX_SHIFT
#define ADS1115_MUX_P2_NG           0x06 << ADS1115_MUX_SHIFT
#define ADS1115_MUX_P3_NG           0x07 << ADS1115_MUX_SHIFT

#define ADS1115_PGA_SHIFT            9
#define ADS1115_PGA_6P144           0x00 << ADS1115_PGA_SHIFT
#define ADS1115_PGA_4P096           0x01 << ADS1115_PGA_SHIFT
#define ADS1115_PGA_2P048           0x02 << ADS1115_PGA_SHIFT // default
#define ADS1115_PGA_1P024           0x03 << ADS1115_PGA_SHIFT
#define ADS1115_PGA_0P512           0x04 << ADS1115_PGA_SHIFT
#define ADS1115_PGA_0P256           0x05 << ADS1115_PGA_SHIFT
#define ADS1115_PGA_0P256B          0x06 << ADS1115_PGA_SHIFT
#define ADS1115_PGA_0P256C          0x07 << ADS1115_PGA_SHIFT

#define ADS1115_MV_6P144            0.187500
#define ADS1115_MV_4P096            0.125000
#define ADS1115_MV_2P048            0.062500 // default
#define ADS1115_MV_1P024            0.031250
#define ADS1115_MV_0P512            0.015625
#define ADS1115_MV_0P256            0.007813
#define ADS1115_MV_0P256B           0.007813
#define ADS1115_MV_0P256C           0.007813

#define ADS1115_MODE_SHIFT            8
#define ADS1115_MODE_CONTINUOUS     0x00 << ADS1115_MODE_SHIFT
#define ADS1115_MODE_SINGLESHOT     0x01 << ADS1115_MODE_SHIFT // default

#define ADS1115_RATE_SHIFT            5
#define ADS1115_RATE_8              0x00 << ADS1115_RATE_SHIFT
#define ADS1115_RATE_16             0x01 << ADS1115_RATE_SHIFT
#define ADS1115_RATE_32             0x02 << ADS1115_RATE_SHIFT
#define ADS1115_RATE_64             0x03 << ADS1115_RATE_SHIFT
#define ADS1115_RATE_128            0x04 << ADS1115_RATE_SHIFT // default
#define ADS1115_RATE_250            0x05 << ADS1115_RATE_SHIFT
#define ADS1115_RATE_475            0x06 << ADS1115_RATE_SHIFT
#define ADS1115_RATE_860            0x07 << ADS1115_RATE_SHIFT

#define ADS1115_COMP_MODE_SHIFT            4
#define ADS1115_COMP_MODE_HYSTERESIS    0x00 << ADS1115_COMP_MODE_SHIFT        // default
#define ADS1115_COMP_MODE_WINDOW        0x01 << ADS1115_COMP_MODE_SHIFT

#define ADS1115_COMP_POL_SHIFT            3
#define ADS1115_COMP_POL_ACTIVE_LOW     0x00 << ADS1115_COMP_POL_SHIFT     // default
#define ADS1115_COMP_POL_ACTIVE_HIGH    0x01 << ADS1115_COMP_POL_SHIFT

#define ADS1115_COMP_LAT_SHIFT            2
#define ADS1115_COMP_LAT_NON_LATCHING   0x00 << ADS1115_COMP_LAT_SHIFT    // default
#define ADS1115_COMP_LAT_LATCHING       0x01 << ADS1115_COMP_LAT_SHIFT

#define ADS1115_COMP_QUE_SHIFT        0
#define ADS1115_COMP_QUE_ASSERT1    0x00 << ADS1115_COMP_SHIFT
#define ADS1115_COMP_QUE_ASSERT2    0x01 << ADS1115_COMP_SHIFT
#define ADS1115_COMP_QUE_ASSERT4    0x02 << ADS1115_COMP_SHIFT
#define ADS1115_COMP_QUE_DISABLE    0x03 // default

#include "I2Cdev.h"

class ADS1115 {
    public:
        ADS1115(uint8_t address = ADS1115_DEFAULT_ADDRESS);
        ~ADS1115();

        bool testConnection();

        int16_t getConversion();

        void setOpStatus(uint16_t op);

        uint16_t getMultiplexer();
        void setMultiplexer(uint16_t mux);

        uint16_t getGain();
        void setGain(uint16_t gain);

        uint16_t getMode();
        void setMode(uint16_t mode);

        uint16_t getRate();
        void setRate(uint16_t rate);

        float getMilliVolts();

        void setComparatorMode(uint16_t comparatorMode);
        void setComparatorPolarity(uint16_t polarit);
        void setComparatorLatchEnabled(uint16_t latchStatus);
        void setComparatorQueueMode(uint16_t queueMode);

    private:
        void updateConfigRegister();

        uint8_t address;

        struct configRegister {
            uint16_t status;
            uint16_t mux;
            uint16_t gain;
            uint16_t mode;
            uint16_t rate;
            uint16_t comparator;
            uint16_t polarity;
            uint16_t latch;
            uint16_t queue;
        } config;

        void showConfigRegister();
};

#endif /* _ADS1115_H_ */
