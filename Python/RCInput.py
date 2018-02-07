import time

import navio.Common.util

if navio.Common.util.get_navio_version() == "NAVIO2":
    import navio.Navio2.RCInput as RCInput
else:
    import navio.Navio.RCInput as RCInput

navio.Common.util.check_apm()
    
rcin = RCInput()
while (True):
    period = rcin.read(2)
    print period
    time.sleep(1)

