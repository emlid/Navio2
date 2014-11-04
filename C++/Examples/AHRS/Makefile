CC = g++
NAVIO = ../../Navio
INCLUDES = -I ../..

all:
	$(CC) $(INCLUDES) AHRS.cpp $(NAVIO)/MPU9250.cpp -o AHRS -lrt -lpthread

clean:
	rm AHRS
