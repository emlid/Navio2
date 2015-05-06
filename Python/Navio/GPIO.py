import os

class Pin():
    """Minimal wrapper for Pins. To be deprecated soon"""

    def __get_direction(self):
        with open("/sys/class/gpio/gpio%d/direction" % self.pin, "r") as direction_file:
            direction = direction_file.read()
            return direction

    def __set_direction(self, direction):
        with open("/sys/class/gpio/gpio%d/direction" % self.pin, "w") as direction_file:
            direction_file.write(direction)

    def __init__(self, pin):
        self.pin = pin
        try:
            with open("/sys/class/gpio/export", "a") as sysfs_export:
                sysfs_export.write("%s" % pin)
        except IOError:
        #already exported. nothing to do
            pass

        self.direction = self.__get_direction()

    def write(self, value):
        if self.direction != "out":
            self.__set_direction("out")

        with open("/sys/class/gpio/gpio%d/value" % self.pin, "w") as value_file:
            value_file.write(str(value))
            
    def read(self):
        with open("/sys/class/gpio/gpio%d/value" % self.pin, "r") as value_file:
            return int(value_file.read())
            

if __name__ == "__main__":
    pin = Pin(27)
    pin.write(0)

    assert pin.read() == 0
