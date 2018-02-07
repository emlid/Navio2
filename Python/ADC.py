import sys, signal, time
import navio.Common.util


if navio.Common.util.get_navio_version() == "NAVIO2":
    import navio.Navio2.ADC as ADC
else:
    import navio.Navio.ADC as ADC

navio.Common.util.check_apm()


def signal_handler(signal, frame):
    print 'You pressed Ctrl+C!'
    sys.exit(0)


signal.signal(signal.SIGINT, signal_handler)
# print 'Press Ctrl+C to exit'

adc = ADC()
results = [0] * adc.channel_count

while (True):
    s = ''
    for i in range(0, adc.channel_count):
        results[i] = adc.read(i)
        s += 'A{0}: {1:6.4f}V '.format(i, results[i] / 1000)
    print(s)
    time.sleep(0.5)
