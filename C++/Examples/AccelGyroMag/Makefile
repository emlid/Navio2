CXX ?= g++
NAVIO = ../../Navio
INCLUDES = -I ../../Navio

all:
	$(MAKE) -C ../../Navio all
	$(CXX) -std=c++11 $(INCLUDES) -L$(NAVIO) AccelGyroMag.cpp -lnavio -o AccelGyroMag

clean:
	rm -f AccelGyroMag
