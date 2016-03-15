import sys, time

import navio.rcinput

rcin = navio.rcinput.RCInput()

while (True):
    period = rcin.read(2)
    print period
    time.sleep(1)
