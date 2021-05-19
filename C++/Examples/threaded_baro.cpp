/*
Provided to you by Emlid Ltd (c) 2014.
twitter.com/emlidtech || www.emlid.com || info@emlid.com

Example: Get pressure from MS5611 barometer onboard of Navio shield for Raspberry Pi
using a different thread, to update pressure info in the background

To run this example navigate to the directory containing it and run following commands:
make
sudo ./threaded_baro
*/

#include <Common/MS5611.h>
#include <Common/Util.h>
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>

void * acquireBarometerData(void * barom)
{
    MS5611* barometer = (MS5611*)barom;

    while (true) {
        barometer->refreshPressure();
        usleep(10000); // Waiting for pressure data ready
        barometer->readPressure();

        barometer->refreshTemperature();
        usleep(10000); // Waiting for temperature data ready
        barometer->readTemperature();

        barometer->calculatePressureAndTemperature();

        //sleep(0.5);
    }

    pthread_exit(NULL);
}

int main()
{
    if (check_apm()) {
        return 1;
    }

    MS5611 baro;
    baro.initialize();

    pthread_t baro_thread;

    if(pthread_create(&baro_thread, NULL, acquireBarometerData, (void *)&baro))
    {
        printf("Error: Failed to create barometer thread\n");
        return 0;
    }

    while(true)
    {
        printf("Temperature(C): %f Pressure(millibar): %f\n", baro.getTemperature(), baro.getPressure());
        sleep(1);
    }

    pthread_exit(NULL);

    return 1;
}
