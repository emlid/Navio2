#include "RCOutput_Navio.h"

RCOutput_Navio::RCOutput_Navio()
{
}

bool RCOutput_Navio::initialize(int channel)
{
    static const uint8_t outputEnablePin = RPI_GPIO_27;


    Pin pin(outputEnablePin);

    if (pin.init()) {
        pin.setMode(Pin::GpioModeOutput);
        pin.write(0); /* drive Output Enable low */
    } else {
        return false;
    }

    return true;
}

bool RCOutput_Navio::enable(int channel)
{
    pwm.initialize();
    return true;
}

bool RCOutput_Navio::set_frequency(int channel, float frequency)
{
    pwm.setFrequency(frequency);
    return true;
}

bool RCOutput_Navio::set_duty_cycle(int channel, float period)
{
    pwm.setPWMmS(channel+3, period / 1000); // 1st Navio RC output is 3
    return true;
}
