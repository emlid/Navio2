ON = 0
OFF = 1

class Pin():
    def __init__(self, folder_name):
        self.pin = folder_name
    
    def write(self, value):
        with open("/sys/class/leds/%s/brightness" % self.pin, "w") as value_file:
            value_file.write(str(value))

class Led():

    def __init__(self):
        self.ledR = Pin("rgb_led0")
        self.ledB = Pin("rgb_led1")
        self.ledG = Pin("rgb_led2")

        self.ledR.write(OFF) 
        self.ledG.write(OFF) 
        self.ledB.write(OFF) 
    
    def setColor(self, color):
        self.ledR.write(self.gamma[color][0]) 
        self.ledG.write(self.gamma[color][1]) 
        self.ledB.write(self.gamma[color][2])
    
    gamma = {
        'Black':    (OFF, OFF, OFF),
        'Red':      (ON, OFF, OFF),
        'Green':    (OFF, ON, OFF),
        'Blue':     (OFF, OFF, ON),
        'Cyan':     (OFF, ON, ON),
        'Magenta':  (ON, OFF, ON),
        'Yellow':   (ON,ON,OFF),
        'White':    (ON, ON, ON)
    }
