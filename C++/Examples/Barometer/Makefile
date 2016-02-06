CXX ?= g++
NAVIO = ../../Navio
INCLUDES = -I ../..

all:
	$(CXX) $(INCLUDES) Barometer.cpp $(NAVIO)/MS5611.cpp $(NAVIO)/I2Cdev.cpp -o Barometer

clean:
	rm Barometer
