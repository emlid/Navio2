CC = g++
NAVIO = ../../Navio
INCLUDES = -I ../..

all:
	$(CC) $(INCLUDES) FRAM.cpp $(NAVIO)/MB85RC04.cpp $(NAVIO)/I2Cdev.cpp -o FRAM

clean:
	rm FRAM
