CXX ?= g++
NAVIO = ../../Navio
INCLUDES = -I ../../Navio

all:
	$(CXX) -std=c++11 $(INCLUDES) $(NAVIO)/Navio2/LSM9DS1.cpp $(NAVIO)/Common/MPU9250.cpp $(NAVIO)/Common/Util.cpp AccelGyroMag.cpp -o AccelGyroMag

clean:
	rm -f AccelGyroMag
