CXX ?= g++
NAVIO = ../../Navio
INCLUDES = -I ../../Navio
CFLAGS = -std=c++11

all:
	$(MAKE) -C ../../Navio all
	$(CXX) $(CFLAGS) $(INCLUDES) -L$(NAVIO) LED.cpp -lnavio -o LED

clean:
	rm -f LED
