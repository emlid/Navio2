/*
Provided to you by Emlid Ltd (c) 2014.
twitter.com/emlidtech || www.emlid.com || info@emlid.com

Example: Control RGB LED with PCA9685 driver onboard of Navio shield for Raspberry Pi.

RGB LED is connected to 0,1,2 channels of PWM controller PCA9685.
As channels are connected to LED's cathodes the logic is inverted,
that means that 0 value sets max brightness and 4095 sets min brightness.

To run this example navigate to the directory containing it and run following commands:
make
./LED
*/

#include <Navio/gpio.h>

#include "Navio/PCA9685.h"

using namespace Navio;

int main()
{
    static const uint8_t outputEnablePin = RPI_GPIO_27;

    Pin pin(outputEnablePin);

    if (pin.init()) {
        pin.setMode(Pin::GpioModeOutput);
        pin.write(0); /* drive Output Enable low */
    } else {
        fprintf(stderr, "Output Enable not set. Are you root?");
    }

    PCA9685 pwm;
    
    pwm.initialize();

    uint16_t R = 0, G = 0, B = 4095;

    pwm.setPWM(0, R);
    pwm.setPWM(1, G);
    pwm.setPWM(2, B);

    while (true) {
        for (R = 0; R < 4095; R++)
            pwm.setPWM(0, R);

        for (B = 4095; B > 0; B--)
            pwm.setPWM(2, B);

        for (G = 0; G < 4095; G++)
            pwm.setPWM(1, G);

        for (R = 4095; R > 0; R--)
            pwm.setPWM(0, R);

        for (B = 0; B < 4095; B++)
            pwm.setPWM(2, B);

        for (G = 4095; G > 0; G--)
            pwm.setPWM(1, G);
    }

    return 0;
}
