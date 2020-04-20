CXX ?= g++
NAVIO = ../../Navio
INCLUDES = -I ../../Navio

all:
	$(MAKE) -C ../../Navio all
	$(CXX) $(INCLUDES) FRAM.cpp -L$(NAVIO) -lnavio -o FRAM

clean:
	rm -f FRAM
