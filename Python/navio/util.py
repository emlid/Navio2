import subprocess as sub
import sys

def check_apm():
    ret = sub.call(["ps -AT | grep -c sched-timer > /dev/null"], shell = True)
    if ret <= 0:
        sys.exit("APM is running. Can't launch the example")
