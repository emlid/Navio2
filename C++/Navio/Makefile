all:
	g++ -std=c++11 -Wno-psabi -I . -c Common/*.cpp Navio2/*.cpp Navio+/*.cpp
	ar rcs libnavio.a *.o
	rm *.o
clean:
	rm *.o
