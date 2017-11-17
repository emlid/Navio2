MODULES = $(wildcard */*.cpp)
EXE = $(MODULES:.cpp=)


all: $(EXE) 

%: %.cpp
	$(MAKE) -C $(@D) all


clean:
	rm -f $(EXE)
