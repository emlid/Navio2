CXX ?= g++
NAVIO = ../../Navio
INCLUDES = -I ../..
CFLAGS = -std=gnu++11

all:
	$(CXX) $(CFLAGS) $(INCLUDES) $(NAVIO)/gpio.cpp $(NAVIO)/RGBled.cpp $(NAVIO)/Util.cpp LED.cpp -o LED

clean:
	rm -f LED
