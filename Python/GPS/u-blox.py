import copy
import Queue
import spidev
import math

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
                        for i in range(0, 7):
                                for j in range(4, 0, -1):
                                        curr_values[i] = (curr_values[i] << 8) + curr_mess.msg_payload[4*i + j - 1]
                        return curr_values
                else: return None

ubl = U_blox()
for ind in range(0, 10):
        ubl.enable_posllh()
while(1):
        buffer = ubl.bus.xfer2([100])
        for byt in buffer:
                ubl.scan_ubx(byt)
                if(ubl.mess_queue.empty() != True):
                        aaa = ubl.parse_ubx()
                        if(aaa != None):
                                print "GPS Millisecond Time of Week: " + str(aaa[0]/1000) + " s"
                                print "Longitude: " + str(aaa[1]/10000000)
                                print "Latitude: " + str(aaa[2]/10000000)
                                print "Height above Ellipsoid: " + str(aaa[3]/1000) + " m"
                                print "Height above mean sea level: " + str(aaa[4]/1000) + " m"
                                print "Horizontal Accuracy Estateimate: " + str(aaa[5]/1000) + " m"
                                print "Vertical Accuracy Estateimate: " + str(aaa[6]/1000) + " m"
