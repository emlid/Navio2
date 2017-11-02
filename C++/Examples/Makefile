MODULES = AccelGyroMag ADC AHRS Barometer FRAM GPS LED Multithread Servo RCInput

all:
	cd ../Navio; \
	($(MAKE) ); \
	cd ../Examples; \
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
