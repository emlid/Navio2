from .adafruit_pwm_servo_driver import PWM
import RPi.GPIO as GPIO



class Led():

    def __init__(self):

        GPIO.setmode(GPIO.BCM)
        GPIO.setwarnings(False)
        GPIO.setup(27, GPIO.OUT)
        GPIO.output(27, GPIO.LOW)

        self.pwm = PWM(0x40, debug=False)

        # Set frequency to 60 Hz
        self.pwm.setPWMFreq(60)

        # set initial color
        self.pwm.setPWM(0, 0, 0)
        self.pwm.setPWM(1, 0, 0)
        self.pwm.setPWM(2, 0, 0)

    def setColor(self, color):
        if color == 'Yellow':
            self.pwm.setPWM(0, 0, 4095)
            self.pwm.setPWM(1, 0, 0)
            self.pwm.setPWM(2, 0, 0)

        elif color == 'Green':
            self.pwm.setPWM(0, 0, 4095)
            self.pwm.setPWM(1, 0, 0)
            self.pwm.setPWM(2, 0, 4095)

        elif color == 'Cyan':
            self.pwm.setPWM(0, 0, 0)
            self.pwm.setPWM(1, 0, 0)
            self.pwm.setPWM(2, 0, 4095)

        elif color == 'Magenta':
            self.pwm.setPWM(0, 0, 0)
            self.pwm.setPWM(1, 0, 4095)
            self.pwm.setPWM(2, 0, 0)

        elif color == 'Red':
            self.pwm.setPWM(0, 0, 4095)
            self.pwm.setPWM(1, 0, 4095)
            self.pwm.setPWM(2, 0, 0)

        elif color == 'Blue':
            self.pwm.setPWM(0, 0, 0)
            self.pwm.setPWM(1, 0, 4095)
            self.pwm.setPWM(2, 0, 4095)



