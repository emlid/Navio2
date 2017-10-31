#include <cstdio>
#include <unistd.h>
#include <fcntl.h>
#include <cstdlib>
#include <err.h>

#include "RCInput_Navio2.h"
#include <Common/Util.h>

#define RCIN_SYSFS_PATH "/sys/kernel/rcio/rcin"

RCInput_Navio2::RCInput_Navio2()
{

}

RCInput_Navio2::~RCInput_Navio2()
{
}

void RCInput_Navio2::initialize()
{
    for (size_t i = 0; i < ARRAY_SIZE(channels); i++) {
        channels[i] = open_channel(i);
        if (channels[i] < 0) {
            perror("open");
        }
    }
}

int RCInput_Navio2::read(int ch)
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

int RCInput_Navio2::open_channel(int channel)
{
    char *channel_path;
    if (asprintf(&channel_path, "%s/ch%d", RCIN_SYSFS_PATH, channel) == -1) {
        err(1, "channel: %d\n", channel);
    }

    int fd = ::open(channel_path, O_RDONLY);

    free(channel_path);

    return fd;
}

