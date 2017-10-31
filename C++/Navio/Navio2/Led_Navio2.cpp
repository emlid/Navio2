#include "Led_Navio2.h"

Led_Navio2::Led_Navio2()
{

}

bool Led_Navio2::initialize()
{
    return led.initialize();
}

void Led_Navio2::setColor(Colors color)
{
    led.setColor(color);
}


