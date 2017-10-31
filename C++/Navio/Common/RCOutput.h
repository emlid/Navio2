#ifndef RCOUTPUT
#define RCOUTPUT

class RCOutput
{
public:
    virtual bool initialize(int channel) = 0;
    virtual bool enable(int channel) = 0;
    virtual bool set_frequency(int channel, float frequency) = 0;
    virtual bool set_duty_cycle(int channel, float period) = 0;
};

#endif // RCOUTPUT

