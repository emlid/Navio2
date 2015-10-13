"""
Provided to you by Emlid Ltd (c) 2014.
twitter.com/emlidtech || www.emlid.com || info@emlid.com

Example: Control the Servo machines connected to PCA9685 driver onboard of Navio shield for Raspberry Pi

Connect servo to Navio's rc output and watch it work.
Output 1 on board is connected to PCA9685 channel 3, output 2 to channel 4 and so on.
To use full range of your servo correct SERVO_MIN and SERVO_MAX according to it's specification.

Please note that this example uses Adafruit I2C and PCA9685 drivers.

To run this example navigate to the directory containing it and run following commands:
sudo python Servo.py
"""

from Adafruit_PWM_Servo_Driver import PWM
import time
import math

import sys

sys.path.append("..")
from Navio import GPIO

#drive Output Enable in PCA low
pin = GPIO.Pin(27)
pin.write(0)

PCA9685_DEFAULT_ADDRESS = 0x40
frequency = 50

NAVIO_RCOUTPUT_1 = 3
SERVO_MIN_ms = 1.250 # mS
SERVO_MAX_ms = 1.750 # mS

#convert mS to 0-4096 scale:
SERVO_MIN = math.trunc((SERVO_MIN_ms * 4096.0) / (1000.0 / frequency) - 1)
SERVO_MAX = math.trunc((SERVO_MAX_ms * 4096.0) / (1000.0 / frequency) - 1)

pwm = PWM(0x40, debug=False)

# Set frequency to 60 Hz
pwm.setPWMFreq(frequency)

while(True):
	pwm.setPWM(NAVIO_RCOUTPUT_1, 0, SERVO_MIN);
	time.sleep(1);
	pwm.setPWM(NAVIO_RCOUTPUT_1, 0, SERVO_MAX);
	time.sleep(1);
