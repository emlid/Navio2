CC = g++
NAVIO = ../../Navio

PIGPIO_PATH := $(PIGPIO_PATH)

LIB = -L$(PIGPIO_PATH)
INCLUDES = -I ../.. -I$(PIGPIO_PATH)


all:
	$(CC) $(INCLUDES) $(LIB)  PPM.cpp $(NAVIO)/PCA9685.cpp $(NAVIO)/I2Cdev.cpp $(NAVIO)/gpio.cpp -o PPM -lrt -lpthread -lpigpio

clean:
	rm PPM
