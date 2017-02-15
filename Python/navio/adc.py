class ADC():
    channel_count = 6
    channels = []

    def __init__(self):
        for i in range(0, self.channel_count):
            try:
                f = open("/sys/kernel/rcio/adc/ch%d" % i, "r")
                self.channels.append(f)
            except: 
                raise
    
    def read(self, ch):
        value = self.channels[ch].read()
        position = self.channels[ch].seek(0, 0)
        return float(value[:-1])
        
