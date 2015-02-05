ROOTCFLAGS    = $(shell $(ROOTSYS)/bin/root-config --cflags)
ROOTGLIBS     = $(shell $(ROOTSYS)/bin/root-config --glibs)

CXX            = g++
CXXFLAGS       = $(filter-out -stdlib=libc++ -pthread , $(ROOTCFLAGS)) -fPIC
GLIBS          = $(filter-out -stdlib=libc++ -pthread , $(ROOTGLIBS))
#GLIBS        += -lMinuit -lRooFit -lRooFitCore
SOFLAGS       = -shared

#ARCH         := $(shell root-config --arch)
#PLATFORM     := $(shell root-config --platform)

INCLUDEDIR       = ./include
SRCDIR           = ./src/
CXX	         += -I$(INCLUDEDIR) -I.
OUTLIB	         = ./lib/
OUTOBJ	         = ./obj/

CC_FILES := $(wildcard src/*.cc)
HH_FILES := $(wildcard include/RestFrames/*.hh)
OBJ_FILES := $(addprefix $(OUTOBJ),$(notdir $(CC_FILES:.cc=.o)))

all: $(OUTLIB)libRestFrames.so

$(OUTLIB)libRestFrames.so: $(OBJ_FILES) $(HH_FILES)
	$(CXX) $(SOFLAGS) $(CXXFLAGS) -o $@ $(GLIBS) $(OBJ_FILES)

$(OUTOBJ)%.o: src/%.cc include/RestFrames/%.hh
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OUTOBJ)*.o 
	rm -f $(OUTLIB)*.so 
