#include "Navio/RGBled.h"

int main()
{
    RGBled led;

    if(!led.initialize()) return EXIT_FAILURE;
    led.setColor(Colors::Yellow);
    printf("LED is yellow\n");
    sleep(1);

    while (true) {
        led.setColor(Colors::Green);
        printf("LED is green\n");
        sleep(1);

        led.setColor(Colors::Cyan);
        printf("LED is cyan\n");
        sleep(1);

        led.setColor(Colors::Blue);
        printf("LED is blue\n");
        sleep(1);

        led.setColor(Colors::Magneta);
        printf("LED is magneta\n");
        sleep(1);

        led.setColor(Colors::Red);
        printf("LED is red\n");
        sleep(1);

        led.setColor(Colors::Yellow);
        printf("LED is yellow\n");
        sleep(1);
    }

    return 0;
}
