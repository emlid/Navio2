CC=g++
CFLAGS=-c -Wall
NAVIO = ../../Navio
INCLUDES = -I ../..

all: gps

gps: gps.o Ublox.o
	$(CC) gps.o Ublox.o -o gps
	rm -rf *.o

gps.o: gps.cpp
	$(CC) $(INCLUDES) $(CFLAGS) gps.cpp

Ublox.o: $(NAVIO)/Ublox.cpp
	$(CC) $(INCLUDES) $(CFLAGS) $(NAVIO)/Ublox.cpp

clean:
	rm -rf *.o gps
