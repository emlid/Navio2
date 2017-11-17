from adafruit_ads1x15 import ADS1x15

class ADC():
    ADS1015 = 0x00  # 12-bit ADC
    ADS1115 = 0x01  # 16-bit ADC

    # Select the gain
    # gain = 6144  # +/- 6.144V
    gain = 4096  # +/- 4.096V
    # gain = 2048  # +/- 2.048V
    # gain = 1024  # +/- 1.024V
    # gain = 512   # +/- 0.512V
    # gain = 256   # +/- 0.256V

    # Select the sample rate
    # sps = 8    # 8 samples per second
    # sps = 16   # 16 samples per second
    # sps = 32   # 32 samples per second
    # sps = 64   # 64 samples per second
    # sps = 128  # 128 samples per second
    sps = 250  # 250 samples per second
    # sps = 475  # 475 samples per second
    # sps = 860  # 860 samples per second

    # Initialise the ADC using the default mode (use default I2C address)
    # Set this to ADS1015 or ADS1115 depending on the ADC you are using!

    channel_count = 4

    def __init__(self):
        self.adc = ADS1x15(ic=self.ADS1115)

    def read(self, ch):
        return self.adc.readADCSingleEnded(ch, self.gain, self.sps)


