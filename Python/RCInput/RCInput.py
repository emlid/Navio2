import sys, time

sys.path.append('..')

import Navio.RCInput

rcin = Navio.RCInput.RCin()

while (True):
    period = rcin.read(2)
    print period
    time.sleep(1)
