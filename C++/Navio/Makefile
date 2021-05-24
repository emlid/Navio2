CXX ?= g++
PIGPIO_PATH ?= pigpio
CFLAGS = -std=c++11 -Wno-psabi -c -I . -I$(PIGPIO_PATH)

SRC=$(wildcard */*.cpp)
OBJECTS = $(SRC:.cpp=.o) 

%.o: %.cpp
	$(CXX) $(CFLAGS) -o $@ $< 

.PHONY: get_deps clean

all: get_deps $(OBJECTS)
	ar rcs libnavio.a $(OBJECTS)

get_deps:
	bash get_dependencies.sh

clean:
	rm -f */*.o *.a
	if [ -e $(PIGPIO_PATH)/Makefile ] ; then \
		$(MAKE) -C $(PIGPIO_PATH) clean ;\
	fi
