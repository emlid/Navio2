"""
Provided to you by Emlid Ltd (c) 2014.
twitter.com/emlidtech || www.emlid.com || info@emlid.com
Example: Write/Read test for MB85RC04 FRAM onboard of Navio shield for Raspberry Pi
To run this example navigate to the directory containing it and run following commands:
sudo python FRAM.py
"""

from MB85RC04 import MB85RC04

fram = MB85RC04()
fram.writeByte(0x005, 99) #Writing some value to the address 0x005
print "Fram memory Write/Read test!\nWe will read value from the address 0x005 of the fram memory, then rewrite this cell and read it again."
print "Value in 0x005 is: " + str(fram.readByte(0x005)) #Reading from 0x05
fram.writeByte(0x005, 11) #Rewriting data at 0x005
print "The cell was rewritten. The new value is: " + str(fram.readByte(0x005)) #Reading from 0x05
fram.writeBytes(0x103, [2, 12, 15, 3, 7, 2, 12, 12, 3, 4, 5, 6, 1, 12, 43, 123, 11, 23, 33, 11, 43, 12, 43, 35, 11, 111, 234, 124, 43, 12, 11, 122]) #Writing some value to the address 0x103 - 0x123
print "Values in cells from 0x103 to 0x123 are: " + str(fram.readBytes(0x103)) #Reading from 0x103
print "Now we will read a block of bytes from the address 0x103 of the fram memory, then rewrite these bytes and read them again."
fram.writeBytes(0x103, [5, 23, 12, 67, 23, 234, 34, 23, 86, 34, 47, 23, 89, 4, 6, 3, 2, 9, 2, 4, 8, 4, 34, 2, 45, 22, 27, 22, 12, 22, 99, 76]) #Rewriting data at 0x103 - 0x123
print "The cells were rewritten. The new values are: " + str(fram.readBytes(0x103)) #Reading from 0x103 
