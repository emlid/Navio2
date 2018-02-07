import time
import os

import navio.Common.util
if navio.Common.util.get_navio_version() == "NAVIO2":
    import navio.Navio2.Led as Led
else:
    import navio.Navio.Led as Led


if (os.getuid() != 0):
    print "Not root. Please, launch like this: sudo python LED.py"
    exit(-1)

navio.Common.util.check_apm()

led = Led()

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

