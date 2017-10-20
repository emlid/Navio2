CC = g++
NAVIO = ../../Navio/Common
INCLUDES = -I ../../Navio

all:
	$(CC) $(INCLUDES) threaded_baro.cpp $(NAVIO)/MS5611.cpp $(NAVIO)/I2Cdev.cpp $(NAVIO)/Util.cpp -lpthread -o threaded_baro

clean:
	rm threaded_baro
