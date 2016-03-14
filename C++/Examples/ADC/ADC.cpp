#include <unistd.h>
#include <cstdio>
#include <Navio/Util.h>
#include <Navio/ADC.h>

int main(int argc, char *argv[])
{
    ADC adc{};
    adc.init();
    float results[adc.get_channel_count()] = {0.0f};

    if (check_apm()) {
        return 1;
    }

    while (true) {
        for (int i = 0; i < ARRAY_SIZE(results); i++){
            results[i] = adc.read(i);
            printf("A%d: %.4fV ", i, results[i] / 1000);
        }
        printf("\n");
        
        usleep(500000);
    }

    return 0;
}
