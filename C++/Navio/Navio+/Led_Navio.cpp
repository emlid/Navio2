#include "Led_Navio.h"

Led_Navio::Led_Navio()
{

}


bool Led_Navio::initialize()
{
    static const uint8_t outputEnablePin = RPI_GPIO_27;
    Pin pin(outputEnablePin);

    if (pin.init()) {
        pin.setMode(Pin::GpioModeOutput);
        pin.write(0); /* drive Output Enable low */
    } else {
        return false;
    }

    led.initialize();
    return true;
}

void Led_Navio::setColor(Colors color)
{
    switch (color)
    {
        case Colors::Yellow:
            led.setPWM(2, 0);
            led.setPWM(1, 0);
            led.setPWM(0, 4095);
            break;
        case Colors::Green:
            led.setPWM(2, 4095);
            led.setPWM(1, 0);
            led.setPWM(0, 4095);
            break;
        case Colors::Cyan:
            led.setPWM(2, 4095);
            led.setPWM(1, 0);
            led.setPWM(0, 0);
            break;
        case Colors::Magenta:
            led.setPWM(2, 0);
            led.setPWM(1, 4095);
            led.setPWM(0, 0);
            break;
        case Colors::Red:
            led.setPWM(2, 0);
            led.setPWM(1, 4095);
            led.setPWM(0, 4095);
            break;
        case Colors::Blue:
            led.setPWM(2, 4095);
            led.setPWM(1, 4095);
            led.setPWM(0, 0);
            break;
    }
}
