/*
Example: Control ADS1115 connected to PCA9685 driver onboard of Navio shield for Rapspberry Pi.
	
Connect a wire to P1 and the ADC will measure its voltage and display it. 
Be careful, do not connect high-voltage circuits, for acceptable range check the datasheet for ADS1115.

To run this example navigate to the directory containing it and run following commands:
make ADC
./ADC

Provided to you by Emlid Ltd, www.emlid.com, info@emlid.com
*/
#include <stdio.h>
#include <unistd.h>

#include "Navio/ADS1115.h"

int main() {

	ADS1115 adc(ADS1115_DEFAULT_ADDRESS);
	adc.setMultiplexer(ADS1115_MUX_P1_NG); 
	adc.setMode(ADS1115_MODE_CONTINUOUS);
	
	while (true) {
		float mV = adc.getMilliVolts();
		printf("mV: %f\n", mV );
	}	
	
	return 0;
}
