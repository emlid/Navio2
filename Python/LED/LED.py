"""
Provided to you by Emlid Ltd (c) 2014.
twitter.com/emlidtech || www.emlid.com || info@emlid.com

Example: Control the RGB LED onboard of Navio shield for Raspberry Pi

Please note that this example uses Adafruit I2C and PCA9685 drivers.

To run this example navigate to the directory containing it and run following commands:
sudo python LED.py
"""

from Adafruit_PWM_Servo_Driver import PWM
import time

import sys

sys.path.append('..')

from Navio import GPIO

#drive Output Enable in PCA low
pin = GPIO.Pin(27)
pin.write(0)

pwm = PWM(0x40, debug=False)

# Set frequency to 60 Hz
pwm.setPWMFreq(60)

step = 1 # light color changing step

# set initial color
R = 0
G = 0
B = 4095
pwm.setPWM(0, 0, B)
print "LED is yellow"
time.sleep(1)

while (True):
    for R in xrange(0, 4095, step):
        pwm.setPWM(2, 0, R)
    print "LED is green"
    time.sleep(1)

    for B in xrange(4095, 0, -step):
        pwm.setPWM(0, 0, B)
    print "LED is cyan"
    time.sleep(1)
    
    for G in xrange(0, 4095, step):
        pwm.setPWM(1, 0, G)
    print "LED is blue"
    time.sleep(1)

    for R in xrange(4095, 0, -step):
        pwm.setPWM(2, 0, R)
    print "LED is magneta"
    time.sleep(1)

    for B in xrange(0, 4095, step):
        pwm.setPWM(0, 0, B)
    print "LED is red"
    time.sleep(1)

    for G in xrange(4095, 0, -step):
        pwm.setPWM(1, 0, G)
    print "LED is yellow"
    time.sleep(1)

  # Change speed of continuous servo on channel O
  # pwm.setPWM(0, 0, servoMin)
  # time.sleep(1)
  # pwm.setPWM(0, 0, servoMax)
  # time.sleep(1)


