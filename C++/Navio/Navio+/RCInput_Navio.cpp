#include "RCInput_Navio.h"

RCInput_Navio::RCInput_Navio()
{
}

RCInput_Navio::~RCInput_Navio()
{
}

void RCInput_Navio::ppmOnEdgeTrampolin(int gpio, int level, uint32_t tick, void *userdata)
{
    RCInput_Navio *self = (RCInput_Navio *) userdata;

    self->ppmOnEdge(gpio, level, tick);
}

void RCInput_Navio::ppmOnEdge(int gpio, int level, uint32_t tick)
    {
    if (level == 0) {
        deltaTime = tick - previousTick;
        previousTick = tick;

        if (deltaTime >= ppmSyncLength) // Sync
            currentChannel = 0;
        else
            if (currentChannel < ppmChannelsNumber)
                channels[currentChannel++] = deltaTime;
    }
}

void RCInput_Navio::initialize()
{
    Pin pin(outputEnablePin);

    if (pin.init()) {
        pin.setMode(Pin::GpioModeOutput);
        pin.write(0); /* drive Output Enable low */
    } else {
        fprintf(stderr, "Output Enable not set. Are you root?");
    }

    // GPIO setup

    gpioCfgClock(samplingRate, PI_DEFAULT_CLK_PERIPHERAL, 0);
    gpioInitialise();
    gpioSetMode(4,PI_INPUT);

    previousTick = gpioTick();
    gpioSetAlertFuncEx(ppmInputGpio, RCInput_Navio::ppmOnEdgeTrampolin, this);
}

int RCInput_Navio::read(int ch)
{
    if (ch > ppmChannelsNumber)
    {
        fprintf(stderr,"Channel number too large\n");
        return -1;
    }
    return channels[ch];
}

