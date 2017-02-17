import os.path

class ADC():
    SYSFS_ADC_PATH_BASE = "/sys/kernel/rcio/adc/"
    channel_count = 6
    channels = []

    def __init__(self):
        if not os.path.exists(self.SYSFS_ADC_PATH_BASE):
            raise OSError("rcio_adc module wasn't loaded")

        self.channels = [open(self.SYSFS_ADC_PATH_BASE + "ch{}".format(channel), "r") for channel in range(self.channel_count)]
    
    def read(self, ch):
        value = self.channels[ch].read()
        position = self.channels[ch].seek(0, 0)
        return float(value[:-1])
        
