CXX ?= g++
NAVIO = ../../Navio
INCLUDES = -I ../../Navio

all:
	$(CXX) -std=c++11 $(INCLUDES) AHRS.cpp -L$(NAVIO) -lnavio -o AHRS -lrt -lpthread

clean:
	rm AHRS
