#pragma once

#include <cstddef>

class ADC 
{
public:
    void init();
    int read(int ch); 
    int get_channel_count(void);
    ADC();
    ~ADC();

private:
    int open_channel(int ch);

    static const size_t CHANNEL_COUNT = 6;
    int channels[CHANNEL_COUNT];
};
