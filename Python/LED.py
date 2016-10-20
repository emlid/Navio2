import sys

import navio.leds
import time
import navio.util

navio.util.check_apm()

led = navio.leds.Led()

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

    led.setColor('Magenta')
    print "LED is magenta"
    time.sleep(1)

    led.setColor('Red')
    print "LED is red"
    time.sleep(1)

    led.setColor('Yellow')
    print "LED is yellow"
    time.sleep(1)
