CXX = g++
CFLAGS = -std=c++11 -Wno-psabi -c -I .

SRC=$(wildcard */*.cpp)
OBJECTS = $(SRC:.cpp=.o) 

%.o: %.cpp
	$(CXX) $(CFLAGS) -o $@ $< 

all: $(OBJECTS)
	ar rcs libnavio.a $(OBJECTS)

clean:
	rm -f */*.o *.a
