CXX ?= g++
NAVIO = ../../Navio

PIGPIO_PATH := $(PIGPIO_PATH)

LIB = -L$(PIGPIO_PATH)

INCLUDES = -I ../../Navio -I$(PIGPIO_PATH)

all:
	$(CXX) -std=gnu++11 $(INCLUDES) $(LIB) main.cpp $(NAVIO)/Common/Util.cpp $(NAVIO)/Navio2/RCInput_Navio2.cpp  $(NAVIO)/Navio+/RCInput_Navio.cpp  $(NAVIO)/Common/I2Cdev.cpp $(NAVIO)/Common/gpio.cpp -o RCInput -lrt -lpthread -lpigpio || $(MAKE) pigpio

clean:
	rm RCInput

