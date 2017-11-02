CXX ?= g++
NAVIO = ../../Navio
INCLUDES = -I ../../Navio

all:
	$(CXX) -std=gnu++11 $(INCLUDES)  ADC.cpp -L$(NAVIO) -lnavio -o ADC

clean:
	rm ADC
