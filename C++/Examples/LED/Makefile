CC = g++
NAVIO = ../../Navio
INCLUDES = -I ../..

all:
	$(CC) $(INCLUDES) LED.cpp $(NAVIO)/PCA9685.cpp $(NAVIO)/I2Cdev.cpp $(NAVIO)/gpio.cpp -o LED

clean:
	rm LED
