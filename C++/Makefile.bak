CXX ?= g++
CFLAGS = -std=gnu++11
LDFLAGS = -lnavio -lrt -lpthread -lpigpio

LIBDIR= $(CURDIR)/Navio
PIGPIODIR= $(LIBDIR)/pigpio
SOURCEDIR = $(CURDIR)/Examples
BUILDDIR = $(CURDIR)/Build

INCLUDES = -I$(LIBDIR) -I$(PIGPIODIR)
LIB_INCLUDES = -L$(LIBDIR) -L$(PIGPIODIR)

MODULES = $(wildcard $(SOURCEDIR)/*.cpp)
EXECUTABLES = $(patsubst %, $(BUILDDIR)/%, $(notdir $(MODULES:.cpp=)))

.PHONY: all lib clean

all: $(EXECUTABLES)

lib:
	$(MAKE) -C $(LIBDIR) all

$(BUILDDIR)/%: $(SOURCEDIR)/%.cpp lib
	@mkdir -p $(BUILDDIR)
	$(CXX) $(CFLAGS) $(INCLUDES) $< $(LIB_INCLUDES) $(LDFLAGS) -o $@
		
clean:
	$(MAKE) -C $(LIBDIR) clean
	rm -rf $(BUILDDIR)
