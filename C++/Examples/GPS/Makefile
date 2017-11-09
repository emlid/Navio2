CXX ?=g++
CFLAGS=-c -Wall
NAVIO = ../../Navio
INCLUDES = -I ../../Navio

all:
	$(MAKE) -C ../../Navio all
	$(CXX) -std=c++11 $(INCLUDES) gps.cpp -L$(NAVIO) -lnavio -o gps
	rm -rf *.o

clean:
	rm -rf *.o gps
