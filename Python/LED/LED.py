import sys

sys.path.append('..')

from Navio import LEDS
import time

led = LEDS.Led()

led.setColor('Yellow')
print "LED is yellow"
time.sleep(1)

while (True):

    led.setColor('Green')
    print "LED is green"
    time.sleep(1)

    led.setColor('Cyan')
    print "LED is cyan"
    time.sleep(1)

    led.setColor('Blue')
    print "LED is blue"
    time.sleep(1)

    led.setColor('Magneta')
    print "LED is magneta"
    time.sleep(1)

    led.setColor('Red')
    print "LED is red"
    time.sleep(1)

    led.setColor('Yellow')
    print "LED is yellow"
    time.sleep(1)
