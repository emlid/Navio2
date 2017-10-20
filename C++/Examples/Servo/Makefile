CXX ?= g++
NAVIO = ../../Navio
INCLUDES = -I ../../Navio

all:
	$(CXX) -std=gnu++11 $(INCLUDES) Servo.cpp $(NAVIO)/Navio2/PWM.cpp $(NAVIO)/Navio+/PCA9685.cpp $(NAVIO)/Common/I2Cdev.cpp $(NAVIO)/Common/gpio.cpp $(NAVIO)/Common/Util.cpp $(NAVIO)/Navio+/RCOutput_Navio.cpp $(NAVIO)/Navio2/RCOutput_Navio2.cpp -o Servo

clean:
	rm Servo
