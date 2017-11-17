from navio.Navio.adafruit_pwm_servo_driver import PWM
import RPi.GPIO as GPIO

import math


class RCOutput():

    PCA9685_DEFAULT_ADDRESS = 0x40

    def __init__(self, pwm_output):
        self.NAVIO_RCOUTPUT = pwm_output + 3
        self.is_initialized = False

    def __enter__(self):
        self.initialize()
        return self

    def __exit__(self):
        GPIO.output(27, GPIO.HIGH)
        self.is_initialized = False


    def initialize(self):
        GPIO.setmode(GPIO.BCM)
        GPIO.setwarnings(False)
        GPIO.setup(27, GPIO.OUT)
        GPIO.output(27, GPIO.LOW)
        self.is_initialized = True

    def enable(self):
        self.pwm = PWM(0X40, debug=False)
        self.pwm.setPWMFreq(self.frequency)

    def set_period(self, period):

        if not self.is_initialized:
            raise RuntimeError("PWM not initialized. Call initialize first")
        self.frequency = period

    def set_duty_cycle(self, ch):
        ch = math.trunc((ch * 4096.0) / (1000.0 / self.frequency) - 1)
        if not self.is_initialized:
            raise RuntimeError("PWM not initialized. Call initialize first")
        self.pwm.setPWM(self.NAVIO_RCOUTPUT, 0, ch)




