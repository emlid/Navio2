CC = g++
NAVIO = ../../Navio
INCLUDES = -I ../..
CFLAGS = -std=gnu++11

all:
	$(CC) $(CFLAGS) $(INCLUDES) $(NAVIO)/gpio.cpp $(NAVIO)/RGBled.cpp LED.cpp -o LED

clean:
	rm -f LED
