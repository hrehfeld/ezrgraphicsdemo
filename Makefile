CC = g++

#build debug or release
BUILDVARIANT := debug

#name of the executable
EXE = ezr

SYSTEM = MINGW

#all directories with source files (cpp/h) in them. edit if you add a new one
SRC := src src/gl lib/glm 
#include these dirs for headers
INCLUDES := lib 

#names of the libs at linking time
LIBS = 
ifeq ($(SYSTEM),LINUX)
  LIBS += GL GLU glut GLEW
	BOOSTLINK=boost_filesystem-gcc-mt-s boost_system-gcc
else
  ifeq ($(SYSTEM),MINGW)
    LIBS +=  opengl32 glu32 freeglut glew32 
	BOOSTLINK=boost_filesystem-mgw44-mt boost_system-mgw44
  endif
endif

#directories to search when linking libs
LIBINCLUDES=

#put binaries in BIN/$(BUILDVARIANT)
BIN := bin

################################################################################
#stop editing here
################################################################################

include librarypaths.makefile

CFLAGS = -D $(SYSTEM) -D __cplusplus

ifeq ($(BUILDVARIANT),debug)
CFLAGS += -O0 -g -Wall
else
ifeq ($(BUILDVARIANT),normal)
CFLAGS += -O2 -g -Wall
else
ifeq ($(BUILDVARIANT),release)
CFLAGS += -O2 -Wall
endif
endif
endif

VPATH := $(BIN)

bin := $(BIN)/${BUILDVARIANT}
#find cpp files in all source dirs
sources := $(foreach dir,$(SRC),$(wildcard $(dir)/*.cpp))
objects := $(addprefix $(bin)/,$(addsuffix .o, $(basename $(sources))))
includes := $(INCLUDES) $(SRC) $(BOOST)
libs = $(LIBS) $(BOOSTLINK)
libincludes = $(LIBINCLUDES) $(BOOSTBIN)/lib

.PHONY: clean

all: $(bin)/$(EXE)

$(bin)/$(EXE): $(bin) $(objects)
	$(CC) -o $@ $(foreach L,$(libincludes),-L$(L)) $(objects) $(foreach lib,$(libs),-l$(lib))


$(bin)/%.o: %.cpp
	echo $@
	mkdir -p $(dir $@)
	${CC} ${CFLAGS} $(foreach include,$(includes),-I$(include) ) -o $@ -c $(filter %.cpp,$^)


clean:
	-rm -rf $(bin)/

%:

