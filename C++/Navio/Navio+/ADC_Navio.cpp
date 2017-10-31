#include "ADC_Navio.h"

ADC_Navio::ADC_Navio()
{

}

void ADC_Navio::initialize()
{
    ADS1115 adc;

    adc.setMode(ADS1115_MODE_SINGLESHOT);
    adc.setRate(ADS1115_RATE_860);
}

int ADC_Navio::read(int ch)
{
    if (ch > ARRAY_SIZE(muxes))
    {
        fprintf(stderr,"Channel number too large\n");
        return -1;
    }
    adc.setMultiplexer(muxes[ch]);

    float conversion = adc.getMilliVolts();
    results[ch % ARRAY_SIZE(muxes)] = conversion;
    return results[ch];
}

int ADC_Navio::get_channel_count(void)
{
    return ARRAY_SIZE(muxes);
}
