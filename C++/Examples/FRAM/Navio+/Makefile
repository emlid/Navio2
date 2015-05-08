CC = g++
NAVIO = ../../../Navio
INCLUDES = -I ../../..

all:
	$(CC) $(INCLUDES) FRAM.cpp $(NAVIO)/MB85RC256.cpp $(NAVIO)/I2Cdev.cpp -o FRAM

clean:
	rm FRAM
