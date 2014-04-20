CC=g++
CFLAGS=-c -Wall
    
Demo: I2Cdev.o PCA9685.o MS5611.o ADS1115.o AK8975.o MPU9150.o Demo.o
	$(CC) I2Cdev.o PCA9685.o MS5611.o ADS1115.o AK8975.o MPU9150.o Demo.o -o Demo
	rm -rf *.o  	
	
LED: I2Cdev.o PCA9685.o LED.o
	$(CC) I2Cdev.o PCA9685.o LED.o -o LED
	rm -rf *.o 
	
Servo: I2Cdev.o PCA9685.o Servo.o
	$(CC) I2Cdev.o PCA9685.o Servo.o -o Servo
	rm -rf *.o   
	
Barometer: I2Cdev.o MS5611.o Barometer.o
	$(CC) I2Cdev.o MS5611.o Barometer.o -o Barometer
	rm -rf *.o  
	
Orientation: I2Cdev.o MPU9150.o AK8975.o Orientation.o
	$(CC) I2Cdev.o MPU9150.o AK8975.o Orientation.o -o Orientation
	rm -rf *.o  
	
ADC: I2Cdev.o ADS1115.o ADC.o
	$(CC) I2Cdev.o ADS1115.o ADC.o -o ADC
	rm -rf *.o   
	
clean:
	rm -rf *.o LED Servo Barometer Orientation ADC Demo 
	
Demo.o: Demo.cpp
	$(CC) $(CFLAGS) Demo.cpp  

LED.o: LED.cpp
	$(CC) $(CFLAGS) LED.cpp 
	
Servo.o: Servo.cpp
	$(CC) $(CFLAGS) Servo.cpp  
	
Barometer.o: Barometer.cpp
	$(CC) $(CFLAGS) Barometer.cpp  
	
ADC.o: ADC.cpp
	$(CC) $(CFLAGS) ADC.cpp  
	
Orientation.o: Orientation.cpp
	$(CC) $(CFLAGS) Orientation.cpp  
    
I2Cdev.o: Navio/I2Cdev.cpp
	$(CC) $(CFLAGS) Navio/I2Cdev.cpp
	
PCA9685.o: Navio/PCA9685.cpp
	$(CC) $(CFLAGS) Navio/PCA9685.cpp
	
MS5611.o: Navio/MS5611.cpp
	$(CC) $(CFLAGS) Navio/MS5611.cpp
	
ADS1115.o: Navio/ADS1115.cpp
	$(CC) $(CFLAGS) Navio/ADS1115.cpp	
	
AK8975.o: Navio/AK8975.cpp
	$(CC) $(CFLAGS) Navio/AK8975.cpp	

MPU9150.o: Navio/MPU9150.cpp
	$(CC) $(CFLAGS) Navio/MPU9150.cpp	
	
u-blox.o: Navio/u-blox.cpp
	$(CC) $(CFLAGS) Navio/u-blox.cpp	

