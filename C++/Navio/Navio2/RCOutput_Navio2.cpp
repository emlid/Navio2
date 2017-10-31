#include "RCOutput_Navio2.h"

RCOutput_Navio2::RCOutput_Navio2()
{
}

bool RCOutput_Navio2::initialize(int channel)
{
    return pwm.init(channel);
}

bool RCOutput_Navio2::enable(int channel)
{
    return pwm.enable(channel);
}

bool RCOutput_Navio2::set_frequency(int channel, float frequency)
{
    return pwm.set_period(channel, frequency);
}

bool RCOutput_Navio2::set_duty_cycle(int channel, float period)
{
    return pwm.set_duty_cycle(channel, period / 1000);
}
