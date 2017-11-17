CXX ?= g++
NAVIO = ../../Navio
INCLUDES = -I ../../Navio

all:
	$(MAKE) -C ../../Navio all
	$(CXX) -std=gnu++11 $(INCLUDES) Servo.cpp -L$(NAVIO) -lnavio -o Servo

clean:
	rm -f Servo
