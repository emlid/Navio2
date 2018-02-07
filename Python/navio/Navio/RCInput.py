import RPi.GPIO as GPIO
import pigpio

class RCInput():

    PPM_SYNC_LENGTH = 4000

    CHANNEL_COUNT = 8
    channels = [0,0,0,0,0,0,0,0]

    def cbf(self, gpio, level, tick):
        if (level == 0):
            deltaTime = tick - self.previousTick
            self.previousTick = tick

            if (deltaTime >= self.PPM_SYNC_LENGTH):
                self.currentchannel = 0
            else:
                if (self.currentchannel < self.CHANNEL_COUNT):
                    self.channels[self.currentchannel] = deltaTime
                    self.currentchannel += 1

    def __init__(self):

        GPIO.setmode(GPIO.BCM)
        GPIO.setwarnings(False)
        GPIO.setup(27, GPIO.OUT)
        GPIO.output(27, GPIO.LOW)

        self.pi = pigpio.pi()
        self.pi.set_mode(4, pigpio.INPUT)

        self.currentchannel = 0
        self.previousTick = self.pi.get_current_tick()
        self.cb1 = self.pi.callback(4, pigpio.EITHER_EDGE, self.cbf)

    def read(self, ch):
        try:
            return self.channels[ch]
        except IndexError:
            print("Channel number too large")
            exit(1)
