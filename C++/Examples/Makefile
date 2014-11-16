MODULES = AccelGyroMag ADC AHRS Barometer FRAM LED Multithread PPM-decoder Servo

all:
	for dir in $(MODULES); do \
	cd $$dir; \
	($(MAKE) ); \
	cd ..; \
	done
	
clean:
	for dir in $(MODULES); do \
	cd $$dir; \
	($(MAKE) clean); \
	cd ..; \
	done
