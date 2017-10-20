CXX ?= g++
NAVIO = ../../Navio/Common
INCLUDES = -I ../../Navio

all:
	$(CXX) $(INCLUDES) Barometer.cpp $(NAVIO)/MS5611.cpp $(NAVIO)/I2Cdev.cpp $(NAVIO)/Util.cpp -o Barometer

clean:
	rm Barometer
