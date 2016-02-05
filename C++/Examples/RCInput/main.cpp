#include <unistd.h>
#include <cstdio>

#include <Navio/RCInput.h>

int main(int argc, char *argv[])
{
    RCInput rcin{};
    rcin.init();

    while (true) 
    {
        int period = rcin.read(2);
        printf("%d\n", period);
        
        sleep(1);
    }

    return 0;
}
