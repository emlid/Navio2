CXX ?= g++
NAVIO = ../../Navio
INCLUDES = -I ../../Navio

all:
	$(MAKE) -C ../../Navio all
	$(CXX) $(INCLUDES) threaded_baro.cpp -L$(NAVIO) -lnavio -lpthread -o threaded_baro

clean:
	rm threaded_baro
