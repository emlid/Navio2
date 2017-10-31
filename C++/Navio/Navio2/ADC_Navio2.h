#pragma once

#include <cstddef>
#include <Common/ADC.h>

class ADC_Navio2 : public ADC
{
public:
    void initialize() override;
    int get_channel_count(void) override;
    int read(int ch) override;
    ADC_Navio2();
    ~ADC_Navio2();

private:
    int open_channel(int ch);

    static const size_t CHANNEL_COUNT = 6;
    int channels[CHANNEL_COUNT];
};
