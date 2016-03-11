CXX ?= g++
NAVIO = ../../Navio
INCLUDES = -I ../..

all:
	$(CXX) $(INCLUDES) $(NAVIO)/LSM9DS1.cpp $(NAVIO)/MPU9250.cpp $(NAVIO)/Util.cpp AccelGyroMag.cpp -o AccelGyroMag

clean:
	rm -f AccelGyroMag
