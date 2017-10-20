CXX ?= g++
NAVIO = ../../Navio
INCLUDES = -I ../../Navio

all:
	$(CXX) -std=gnu++11 $(INCLUDES) ADC.cpp $(NAVIO)/Common/Util.cpp $(NAVIO)/Navio+/ADS1115.cpp $(NAVIO)/Common/I2Cdev.cpp $(NAVIO)/Navio2/ADC_Navio2.cpp $(NAVIO)/Navio+/ADC_Navio.cpp -o ADC

clean:
	rm ADC
