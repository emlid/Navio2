/*
This code is provided under the BSD license.
Copyright (c) 2014, Emlid Limited. All rights reserved.
twitter.com/emlidtech || www.emlid.com || info@emlid.com

Application: PPM to PWM decoder for Raspberry Pi with Navio.

Requres pigpio library, please install it first - http://abyz.co.uk/rpi/pigpio/
To run this app navigate to the directory containing it and run following commands:
make
sudo ./PPM
*/

#include <pigpio.h>
#include <stdio.h>
#include <unistd.h>
#include "Navio/PCA9685.h"

//================================ Options =====================================

unsigned int samplingRate      = 5;      // 5 microseconds
unsigned int ppmInputGpio      = 4;      // PPM input on Navio's 2.54 header
unsigned int ppmSyncLength     = 4000;   // Length of PPM sync pause
unsigned int ppmChannelsNumber = 8;      // Number of channels packed in PPM
bool verboseOutputEnabled      = true;   // Output channels values to console

//============================ Objects & data ==================================

PCA9685 *pwm;
float channels[8];

//============================== PPM decoder ===================================

unsigned int currentChannel = 0;
unsigned int previousTick;
unsigned int deltaTime;

void ppmOnEdge(int gpio, int level, uint32_t tick)
{
	deltaTime = tick - previousTick;
	previousTick = tick;

	if (level == 0)
		if (deltaTime >= ppmSyncLength) { // Sync
			currentChannel = 0;

			// RC output
			for (int i = 0; i < ppmChannelsNumber; i++)
			    pwm->setPWMuS(i + 3, channels[i]); // First Navio RC output is 3

			// Console output
			if (verboseOutputEnabled) {
				printf("\n");
				for (int i = 0; i < ppmChannelsNumber; i++)
					printf("%4.f ", channels[i]);
			}
		}
		else
			if (currentChannel < ppmChannelsNumber)
				channels[currentChannel++] = deltaTime;
}

//================================== Main ======================================

int main(int argc, char *argv[])
{
	// Servo controller setup

	pwm = new PCA9685();
	pwm->initialize();
	pwm->setFrequency(100);

	// GPIO setup

	gpioCfgClock(samplingRate, PI_DEFAULT_CLK_PERIPHERAL, PI_DEFAULT_CLK_SOURCE);
	gpioInitialise();
	previousTick = gpioTick();
	gpioSetAlertFunc(ppmInputGpio, ppmOnEdge);

	// Infinite sleep - all action in now happening in ppmOnEdge() function

	while(1)
		sleep(10);
	return 0;
}
