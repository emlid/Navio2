#pragma once

#include <cstddef>

class RCInput 
{
public:
    void init();
    int read(int ch); 
    RCInput();
    ~RCInput();

private:
    int open_channel(int ch);

    static const size_t CHANNEL_COUNT = 14;
    int channels[CHANNEL_COUNT];
};
