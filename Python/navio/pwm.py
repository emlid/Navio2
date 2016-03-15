class PWM():

    def __init__(self, channel):
        self.channel = channel
        try:
            with open("/sys/class/pwm/pwmchip0/export", "a") as pwm_export:
                pwm_export.write(str(self.channel))
        except IOError:
            #already exported. nothing to 
            pass
        
        with open("/sys/class/pwm/pwmchip0/pwm%d/enable" % self.channel, "w") as pwm_enable:
            pwm_enable.write("1")

    def set_period(self, freq):
        period_ns = int(1e9/freq)
        with open("/sys/class/pwm/pwmchip0/pwm%d/period" % self.channel, "w") as pwm_period:
            pwm_period.write(str(period_ns))

    def set_duty_cycle(self, period):
        period_ns = int(period*1e6)
        with open("/sys/class/pwm/pwmchip0/pwm%d/duty_cycle" % self.channel, "w") as pwm_duty:
            pwm_duty.write(str(period_ns))
