#include <cstdio>
#include <unistd.h>
#include <fcntl.h>
#include <cstdlib>
#include <err.h>

#include "ADC_Navio2.h"
#include <Common/Util.h>

#define ADC_SYSFS_PATH "/sys/kernel/rcio/adc"

ADC_Navio2::ADC_Navio2()
{

}

ADC_Navio2::~ADC_Navio2()
{
}

void ADC_Navio2::initialize()
{
    for (size_t i = 0; i < ARRAY_SIZE(channels); i++) {
        channels[i] = open_channel(i);
        if (channels[i] < 0) {
            perror("open");
        }
    }
}

int ADC_Navio2::get_channel_count(void)
{
    return CHANNEL_COUNT;
}

int ADC_Navio2::read(int ch)
{
    if (ch > ARRAY_SIZE(channels) )
	{
        fprintf(stderr,"Channel number too large\n");
        return -1;
	}
    char buffer[10];

    if (::pread(channels[ch], buffer, ARRAY_SIZE(buffer), 0) < 0) {
        perror("pread");
    }

    return atoi(buffer);
}

int ADC_Navio2::open_channel(int channel)
{
    char *channel_path;
    if (asprintf(&channel_path, "%s/ch%d", ADC_SYSFS_PATH, channel) == -1) {
        err(1, "adc channel: %d\n", channel);
    }

    int fd = ::open(channel_path, O_RDONLY);

    free(channel_path);

    return fd;
}

