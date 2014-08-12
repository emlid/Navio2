CC = g++
NAVIO = ../../Navio
INCLUDES = -I ../..

all:
	$(CC) $(INCLUDES) threaded_baro.cpp $(NAVIO)/MS5611.cpp $(NAVIO)/I2Cdev.cpp -lpthread -o threaded_baro

clean:
	rm threaded_baro
