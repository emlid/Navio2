CXX ?= g++
NAVIO = ../../Navio
INCLUDES = -I ../../Navio
CFLAGS = -std=c++11

all:
	$(CXX) $(CFLAGS) $(INCLUDES) $(NAVIO)/Common/gpio.cpp $(NAVIO)/Navio2/RGBled.cpp $(NAVIO)/Navio+/PCA9685.cpp $(NAVIO)/Common/I2Cdev.cpp $(NAVIO)/Common/Util.cpp $(NAVIO)/Navio2/Led_Navio2.cpp $(NAVIO)/Navio+/Led_Navio.cpp LED.cpp -o LED

clean:
	rm -f LED
