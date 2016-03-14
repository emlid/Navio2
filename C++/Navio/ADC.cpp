#include <cstdio>
#include <unistd.h>
#include <fcntl.h>
#include <cstdlib>
#include <err.h>

#include "ADC.h"
#include "Util.h"

#define ADC_SYSFS_PATH "/sys/kernel/rcio/adc"

ADC::ADC()
{

}

ADC::~ADC()
{
}

void ADC::init()
{
    for (size_t i = 0; i < ARRAY_SIZE(channels); i++) {
        channels[i] = open_channel(i);
        if (channels[i] < 0) {
            perror("open");
        }
    }
}

int ADC::get_channel_count(void)
{
    return CHANNEL_COUNT;
}

int ADC::read(int ch)
{
    char buffer[10];

    if (::pread(channels[ch], buffer, ARRAY_SIZE(buffer), 0) < 0) {
        perror("pread");
    }

    return atoi(buffer);
}

int ADC::open_channel(int channel)
{
    char *channel_path;
    if (asprintf(&channel_path, "%s/ch%d", ADC_SYSFS_PATH, channel) == -1) {
        err(1, "adc channel: %d\n", channel);
    }

    int fd = ::open(channel_path, O_RDONLY);

    free(channel_path);

    return fd;
}

