import subprocess as sub
import sys

def check_apm():
    ret = sub.call(["ps -AT | grep -c ap-timer > /dev/null"], shell = True)
    if ret <= 0:
        sys.exit("APM is running. Can't launch the example")

def get_navio_version():
    file = open("/sys/firmware/devicetree/base/hat/product_id")
    version = "NAVIO" if file.readline(6) == "0x0001" else "NAVIO2"
    return version
