CXX ?=g++
CFLAGS=-c -Wall
NAVIO = ../../Navio
INCLUDES = -I ../..

all: gps

gps: gps.o Ublox.o Util.o
	$(CXX) gps.o Ublox.o Util.o -o gps
	rm -rf *.o

gps.o: gps.cpp
	$(CXX) $(INCLUDES) $(CFLAGS) gps.cpp

Ublox.o: $(NAVIO)/Ublox.cpp
	$(CXX) $(INCLUDES) $(CFLAGS) $(NAVIO)/Ublox.cpp

Util.o: $(NAVIO)/Util.cpp
	$(CXX) $(INCLUDES) $(CFLAGS) $(NAVIO)/Util.cpp

clean:
	rm -rf *.o gps
