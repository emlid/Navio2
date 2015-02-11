/*
Provided to you by Emlid Ltd (c) 2014.
twitter.com/emlidtech || www.emlid.com || info@emlid.com

Example: Control servos connected to PCA9685 driver onboard of Navio shield for Raspberry Pi.

Connect servo to Navio's rc output and watch it work.
Output 1 on board is connected to PCA9685 channel 3, output 2 to channel 4 and so on.
To use full range of your servo correct SERVO_MIN and SERVO_MAX according to it's specification.

To run this example navigate to the directory containing it and run following commands:
make
./Servo
*/

#define NAVIO_RCOUTPUT_1 3
#define SERVO_MIN 1.250 /*mS*/
#define SERVO_MAX 1.750 /*mS*/

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
    pwm.setFrequency(50);

    while (true) {
        pwm.setPWMmS(NAVIO_RCOUTPUT_1, SERVO_MIN);
        sleep(1);
        pwm.setPWMmS(NAVIO_RCOUTPUT_1, SERVO_MAX);
        sleep(1);
    }

    return 0;
}
