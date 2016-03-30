import copy
import Queue
import spidev
import math
import struct
import navio.util

navio.util.check_apm()

waiting_header = 0
msg_class = 1
msg_id = 2
length = 3
payload = 4
checksum = 5

class U_blox_message:
	def __init__(self, msg_class = 0, msg_id = 0, msg_length = 0, msg_payload = []):
		self.msg_class = msg_class
		self.msg_id = msg_id 
		self.msg_length = msg_length
		self.msg_payload = msg_payload
	
	def clear(self):
		self.msg_class = 0
		self.msg_id = 0
		self.msg_length = 0
		self.msg_payload = []

class U_blox:

	def __init__(self):
		self.mess_queue = Queue.Queue()
		self.curr_mess = U_blox_message()
		self.bus = spidev.SpiDev()
		self.bus.open(0,0)
		self.state=0
		self.counter1=0
		self.chk_a=0
		self.chk_b=0
		self.accepted_chk_a=0
		self.accepted_chk_b=0

	def enable_posllh(self):
		msg = [0xb5, 0x62, 0x06, 0x01, 0x03, 0x00, 0x01, 0x02, 0x01, 0x0e, 0x47]
		self.bus.xfer2(msg)


	def scan_ubx(self, byte):
		if(self.state == waiting_header):
			self.result = [0,0,0,0,0,0,0,0,0]
			self.accepted = 0
			self.chk_a = 0
			self.chk_b = 0
			if((self.counter1 == 0) & (byte == 0xb5)):
				self.counter1 += 1
			elif((self.counter1 == 0) & (byte != 0xb5)):
				self.state = waiting_header
				self.counter1 = 0
			elif((self.counter1 == 1) & (byte == 0x62)):
				self.counter1 = 0
				self.state = msg_class
			elif((self.counter1 == 1) & (byte != 0x62)):
				self.counter1 = 0
				self.state = waiting_header
		elif(self.state == msg_class):
			self.chk_a = (self.chk_a + byte)%256
			self.chk_b = (self.chk_b + self.chk_a)%256
			self.curr_mess.msg_class = byte
			self.state = msg_id
		elif(self.state == msg_id):
			self.chk_a = (self.chk_a + byte)%256
			self.chk_b = (self.chk_b + self.chk_a)%256
			self.curr_mess.msg_id = byte
			self.state = length
		elif(self.state == length):
			if(self.counter1 == 0):
				self.chk_a = (self.chk_a + byte)%256
				self.chk_b = (self.chk_b + self.chk_a)%256
				self.counter1 += 1
				self.curr_mess.msg_length = byte
			elif(self.counter1 == 1):
				self.chk_a = (self.chk_a + byte)%256
				self.chk_b = (self.chk_b + self.chk_a)%256
				self.counter1 = 0
				self.curr_mess.msg_length = self.curr_mess.msg_length + 256*byte
				self.state = payload
		elif(self.state == payload):
			self.chk_a = (self.chk_a + byte)%256
			self.chk_b = (self.chk_b + self.chk_a)%256
			
			self.curr_mess.msg_payload.append(byte)
			if(self.counter1 < self.curr_mess.msg_length - 1):
				self.counter1 += 1
			else:
				self.counter1 = 0
				self.state = checksum

		elif(self.state == checksum):
			if(self.counter1 == 0):
				self.accepted_chk_a = byte
				self.counter1 += 1
			elif(self.counter1 == 1):
				self.accepted_chk_b = byte
				self.counter1 = 0
				self.state = waiting_header
				self.curr_mess.msg_length = 0
				if((ubl.chk_a == ubl.accepted_chk_a) & (ubl.chk_b == ubl.accepted_chk_b)):
					self.mess_queue.put(copy.deepcopy(self.curr_mess))
					self.curr_mess.clear()
				else:
					print("Error! Checksum doesn't match")

	def parse_ubx(self):
		curr_values = [0,0,0,0,0,0,0]
		curr_mess = self.mess_queue.get(False)
		if((curr_mess.msg_class  == 0x01) & (curr_mess.msg_id == 0x02)):
			msg = NavPosllhMsg()
			curr_values = struct.unpack("<IiiiiII", str(bytearray(curr_mess.msg_payload)))
			msg.itow = curr_values[0]
			msg.lon = curr_values[1]
			msg.lat = curr_values[2]
			msg.heightEll = curr_values[3]
			msg.heightSea = curr_values[4]
			msg.horAcc = curr_values[5]
			msg.verAcc = curr_values[6]
			return msg

		if((curr_mess.msg_class == 0x01) & (curr_mess.msg_id == 0x03)):
			msg = NavStatusMsg()
			msg.fixStatus = curr_mess.msg_payload[4]
			msg.fixOk = curr_mess.msg_payload[5]
			return msg
		return None

class NavStatusMsg:

	def __init__(self):
		self.fixOk = 0
		self.fixStatus = 0

	def __str__(self):
		Status = "Reserved value. Current state unknown\n"
		if   (self.fixStatus == 0x00): Status = "no fix\n"
		elif (self.fixStatus == 0x01): Status = "dead reckoning only\n"
		elif (self.fixStatus == 0x02): Status = "2D-fix\n"
		elif (self.fixStatus == 0x03): Status = "3D-fix\n"
		elif (self.fixStatus == 0x04): Status = "GPS + dead reckoning combined\n"
		elif (self.fixStatus == 0x05): Status = "Time only fix\n"
		return 'Current GPS status:\ngpsFixOk: {}\ngps Fix status: {}'.format(self.fixOk & 0x01, Status)

class NavPosllhMsg:

	def __init__(self):
		self.itow=0
		self.lon=0
		self.lat=0
		self.heightEll=0
		self.heightSea=0
		self.horAcc=0
		self.verAcc=0

	def __str__(self):
		itow = "GPS Millisecond Time of Week: %d s" % (self.itow/1000)
		lon = "Longitude: %.6f"  % (self.lon/10000000.0)
		lat = "Latitude: %.6f" % (self.lat/10000000.0)
		heightEll = "Height above Ellipsoid: %.3f m" % (self.heightEll/1000.0)
		heightSea = "Height above mean sea level: %.3f m" % (self.heightSea/1000.0)
		horAcc = "Horizontal Accuracy Estateimate: %.3f m" % (self.horAcc/1000.0)
		verAcc = "Vertical Accuracy Estateimate: %.3f m" % (self.verAcc/1000.0)
		return '{}\n{}\n{}\n{}\n{}\n{}\n{}'.format(itow, lon, lat, heightEll, heightSea, horAcc, verAcc)

ubl = U_blox()
for ind in range(0, 10):
	ubl.enable_posllh()
while(1):
	buffer = ubl.bus.xfer2([100])
	for byt in buffer:
		ubl.scan_ubx(byt)
		if(ubl.mess_queue.empty() != True):
			msg = ubl.parse_ubx()
			if (msg != None): print(msg)
