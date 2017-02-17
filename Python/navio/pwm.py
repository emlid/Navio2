import os.path

class PWM():
    SYSFS_PWM_PATH_BASE = "/sys/class/pwm/pwmchip0/"
    SYSFS_PWM_EXPORT_PATH = "/sys/class/pwm/pwmchip0/export"

    def __init__(self, channel):
        self.channel = channel
        self.channel_path = self.SYSFS_PWM_PATH_BASE + "pwm{}/".format(self.channel)

        if not os.path.exists(self.SYSFS_PWM_PATH_BASE):
            raise IOError("rcio_pwm module wasn't loaded")

        if not os.path.exists(self.channel_path):
            with open(self.SYSFS_PWM_EXPORT_PATH, "a") as pwm_export:
                pwm_export.write(str(self.channel))

        with open(self.channel_path + "enable", "w") as pwm_enable:
            pwm_enable.write("1")

    def set_period(self, freq):
        period_ns = int(1e9/freq)
        with open(self.channel_path + "period",  "w") as pwm_period:
            pwm_period.write(str(period_ns))

    def set_duty_cycle(self, period):
        period_ns = int(period*1e6)
        with open(self.channel_path + "duty_cycle", "w") as pwm_duty:
            pwm_duty.write(str(period_ns))
