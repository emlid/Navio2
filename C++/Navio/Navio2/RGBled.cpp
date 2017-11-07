#include "RGBled.h"

// Output is inverted
#define OFF 1
#define ON 0

using namespace Navio;

RGBled::RGBled()
{
}

bool RGBled::initialize() {
    pinR = new Pin(RPI_GPIO_4);
    pinG = new Pin(RPI_GPIO_27);
    pinB = new Pin(RPI_GPIO_6);
    if (pinR->init() && pinG->init() && pinB->init()) {
        pinR->setMode(Pin::GpioModeOutput);
        pinG->setMode(Pin::GpioModeOutput);
        pinB->setMode(Pin::GpioModeOutput);

        // Switch of LED
        pinR->write(OFF);
        pinG->write(OFF);
        pinB->write(OFF);
    }
    else {
        return false;
    }
    return true;
}

void RGBled::setColor(Colors color)
{
    switch (color) {
    case Colors::Black: pinR->write(OFF); pinG->write(OFF); pinB->write(OFF); break;
    case Colors::Red: pinR->write(ON); pinG->write(OFF); pinB->write(OFF); break;
    case Colors::Green: pinR->write(OFF); pinG->write(ON); pinB->write(OFF); break;
    case Colors::Blue: pinR->write(OFF); pinG->write(OFF); pinB->write(ON); break;
    case Colors::Cyan: pinR->write(OFF); pinG->write(ON); pinB->write(ON); break;
    case Colors::Magenta: pinR->write(ON); pinG->write(OFF); pinB->write(ON); break;
    case Colors::Yellow: pinR->write(ON); pinG->write(ON); pinB->write(OFF); break;
    case Colors::White: pinR->write(ON); pinG->write(ON); pinB->write(ON); break;
    }
}
