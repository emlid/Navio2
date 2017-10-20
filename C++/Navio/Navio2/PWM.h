#ifndef _PWD_H_
#define _PWD_H_

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <errno.h>

class PWM {
public:
    PWM();

    bool init(unsigned int channel);
    bool enable(unsigned int channel);
    bool set_period(unsigned int channel, unsigned int freq);
    bool set_duty_cycle(unsigned int channel, float period);
};

#endif //_PWD_H_

