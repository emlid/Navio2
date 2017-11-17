CXX ?= g++
NAVIO = ../../Navio
INCLUDES = -I ../../Navio

all:
	$(MAKE) -C ../../Navio all
	$(CXX) -std=c++11 $(INCLUDES) AHRS.cpp -L$(NAVIO) -lnavio -o AHRS -lrt -lpthread

clean:
	rm -f AHRS
