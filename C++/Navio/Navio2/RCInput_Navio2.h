#pragma once

#include <cstddef>
#include <Common/RCInput.h>

class RCInput_Navio2 : public RCInput
{
public:
    void initialize() override;
    int read(int ch) override; 
    RCInput_Navio2();
    ~RCInput_Navio2();

private:
    int open_channel(int ch);

    static const size_t CHANNEL_COUNT = 14;
    int channels[CHANNEL_COUNT];
};
