"""
Provided to you by Emlid Ltd (c) 2014.
twitter.com/emlidtech || www.emlid.com || info@emlid.com

Example: Get pressure from MS5611 barometer onboard of Navio shield for Raspberry Pi

To run this example navigate to the directory containing it and run following commands:
sudo python Barometer_example.py
"""

import time

import navio.ms5611
import navio.util

navio.util.check_apm()

baro = navio.ms5611.MS5611()
baro.initialize()

while(True):
	baro.refreshPressure()
	time.sleep(0.01) # Waiting for pressure data ready 10ms
	baro.readPressure()

	baro.refreshTemperature()
	time.sleep(0.01) # Waiting for temperature data ready 10ms
	baro.readTemperature()

	baro.calculatePressureAndTemperature()

	print "Temperature(C): %.6f" % (baro.TEMP), "Pressure(millibar): %.6f" % (baro.PRES)

	time.sleep(1)
