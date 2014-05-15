CC = g++
NAVIO = ../../Navio
INCLUDES = -I ../..

all:
	$(CC) $(INCLUDES) Orientation.cpp $(NAVIO)/MPU9150.cpp $(NAVIO)/I2Cdev.cpp -o Orientation

clean:
	rm Orientation
