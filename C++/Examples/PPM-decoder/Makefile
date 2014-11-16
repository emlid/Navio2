CC = g++
NAVIO = ../../Navio
INCLUDES = -I ../..

all:
	$(CC) $(INCLUDES) PPM.cpp $(NAVIO)/PCA9685.cpp $(NAVIO)/I2Cdev.cpp -o PPM -lrt -lpthread -lpigpio

clean:
	rm PPM
