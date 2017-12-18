import os.path

class PWM():
    SYSFS_PWM_PATH_BASE = "/sys/class/pwm/pwmchip0/"
    SYSFS_PWM_EXPORT_PATH = "/sys/class/pwm/pwmchip0/export"
    SYSFS_PWM_UNEXPORT_PATH = "/sys/class/pwm/pwmchip0/unexport"

    def __init__(self, channel):
        self.channel = channel
        self.channel_path = self.SYSFS_PWM_PATH_BASE + "pwm{}/".format(self.channel)
        self.is_initialized = False
        self.is_enabled = False

    def __enter__(self):
        self.initialize()
        return self

    def __exit__(self, *args):
        self.deinitialize()

    def deinitialize(self):
        if self.is_enabled:
            self.set_period(1)
            self.disable()
        with open(self.SYSFS_PWM_UNEXPORT_PATH, "a") as pwm_unexport:
            pwm_unexport.write(str(self.channel))

    def initialize(self):
        if not os.path.exists(self.SYSFS_PWM_PATH_BASE):
            raise OSError("rcio_pwm module wasn't loaded")

        if not os.path.exists(self.channel_path):
            with open(self.SYSFS_PWM_EXPORT_PATH, "a") as pwm_export:
                pwm_export.write(str(self.channel))

        self.is_initialized = True

    def enable(self):
        with open(self.channel_path + "enable", "w") as pwm_enable:
            pwm_enable.write("1")
            self.is_enabled = True

    def disable(self):
        with open(self.channel_path + "enable", "w") as pwm_enable:
            pwm_enable.write("0")
            self.is_enabled = False

    def set_period(self, freq):
        if not self.is_initialized:
            raise RuntimeError("PWM not initialized. Call initialize first")

        period_ns = int(1e9/freq)
        with open(self.channel_path + "period",  "w") as pwm_period:
            pwm_period.write(str(period_ns))

    def set_duty_cycle(self, period):
        if not self.is_initialized:
            raise RuntimeError("PWM not initialized. Call initialize first")

        period_ns = int(period*1e6)
        with open(self.channel_path + "duty_cycle", "w") as pwm_duty:
            pwm_duty.write(str(period_ns))
