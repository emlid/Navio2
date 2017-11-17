CXX ?= g++
NAVIO = ../../Navio
INCLUDES = -I ../../Navio

all:
	$(MAKE) -C ../../Navio all
	$(CXX) -std=gnu++11 $(INCLUDES)  ADC.cpp -L$(NAVIO) -lnavio -o ADC

clean:
	rm -f ADC
