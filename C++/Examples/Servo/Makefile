CXX ?= g++
NAVIO = ../../Navio
INCLUDES = -I ../..

all:
	$(CXX) -std=gnu++11 $(INCLUDES) Servo.cpp $(NAVIO)/PWM.cpp $(NAVIO)/Util.cpp -o Servo

clean:
	rm Servo
