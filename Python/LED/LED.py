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

pwm = PWM(0x40, debug=True)

# Set frequency to 60 Hz
pwm.setPWMFreq(60)

step = 8 # light color changing step

while (True):
    for R in xrange(0, 4095, step):
        pwm.setPWM(0, 0, R)

    for B in xrange(4095, 0, step):
        pwm.setPWM(2, 0, B)

    for G in xrange(0, 4095, step):
        pwm.setPWM(1, 0, G)

    for R in xrange(4095, 0, step):
        pwm.setPWM(0, 0, R)

    for B in xrange(0, 4095, step):
        pwm.setPWM(2, 0, B)

    for G in xrange(4095, 0, step):
        pwm.setPWM(1, 0, G)

  # Change speed of continuous servo on channel O
  # pwm.setPWM(0, 0, servoMin)
  # time.sleep(1)
  # pwm.setPWM(0, 0, servoMax)
  # time.sleep(1)


