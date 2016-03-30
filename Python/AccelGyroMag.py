"""
MS5611 driver code is placed under the BSD license.
Copyright (c) 2014, Emlid Limited, www.emlid.com
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
	* Redistributions of source code must retain the above copyright
	notice, this list of conditions and the following disclaimer.
	* Redistributions in binary form must reproduce the above copyright
	notice, this list of conditions and the following disclaimer in the
	documentation and/or other materials provided with the distribution.
	* Neither the name of the Emlid Limited nor the names of its contributors
	may be used to endorse or promote products derived from this software
	without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL EMLID LIMITED BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
"""

import spidev
import time
import argparse 
import sys
import navio.mpu9250
import navio.util

navio.util.check_apm()

parser = argparse.ArgumentParser()
parser.add_argument("-i", help = "Sensor selection: -i [sensor name]. Sensors names: mpu is MPU9250, lsm is LSM9DS1")

if len(sys.argv) == 1:
    print "Enter parameter"
    parser.print_help()
    sys.exit(1)
elif len(sys.argv) == 2:
    sys.exit("Enter sensor name: mpu or lsm")

args = parser.parse_args()

if args.i == 'mpu':
    print "Selected: MPU9250"
elif args.i == 'lsm':
    sys.exit( "Driver for LSM9DS1 is not implement yet")
else:
    print "Wrong sensor name. Select: mpu or lsm"
    sys.exit(1)


imu = navio.mpu9250.MPU9250()

if imu.testConnection():
    print "Connection established: True"
else:
    sys.exit("Connection established: False")

imu.initialize()

time.sleep(1)

while True:
	# imu.read_all()
	# imu.read_gyro()
	# imu.read_acc()
	# imu.read_temp()
	# imu.read_mag()

	# print "Accelerometer: ", imu.accelerometer_data
	# print "Gyroscope:     ", imu.gyroscope_data
	# print "Temperature:   ", imu.temperature
	# print "Magnetometer:  ", imu.magnetometer_data

	# time.sleep(0.1)

	m9a, m9g, m9m = imu.getMotion9()

	print "Acc:", "{:+7.3f}".format(m9a[0]), "{:+7.3f}".format(m9a[1]), "{:+7.3f}".format(m9a[2]),
	print " Gyr:", "{:+8.3f}".format(m9g[0]), "{:+8.3f}".format(m9g[1]), "{:+8.3f}".format(m9g[2]),
	print " Mag:", "{:+7.3f}".format(m9m[0]), "{:+7.3f}".format(m9m[1]), "{:+7.3f}".format(m9m[2])

	time.sleep(0.5)
