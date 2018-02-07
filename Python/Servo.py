import time
import os

import navio.Common.util

if navio.Common.util.get_navio_version() == "NAVIO2":
    import navio.Navio2.RCOutput as RCOutput
else:
    import navio.Navio.RCOutput as RCOutput

SERVO_MIN = 1.250  # ms
SERVO_MAX = 1.750  # ms
PWM_OUTPUT = 0

if (os.getuid() != 0):
    print "Not root. Please, launch like this: sudo python Servo.py"
    exit(-1)

navio.Common.util.check_apm()

with RCOutput(PWM_OUTPUT) as pwm:
    pwm.set_period(50)
    pwm.enable()

    while (True):
        pwm.set_duty_cycle(SERVO_MIN)
        time.sleep(1)
        pwm.set_duty_cycle(SERVO_MAX)
        time.sleep(1)
