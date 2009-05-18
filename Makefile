CC = g++

#build debug or release
BUILDVARIANT := debug

#name of the executable
EXE = ezr

SYSTEM = LINUX

#all directories with source files (cpp/h) in them. edit if you add a new one
SRC := src lib/glm
#include these dirs for headers
INCLUDES := lib #lib/glut #lib/glu #lib/glew/include
#names of the libs at linking time
LIBS = GL GLU glut GLEW
#put binaries in BIN/$(BUILDVARIANT)
BIN := bin

#stop editing here

CFLAGS = -D $(SYSTEM) -D __cplusplus

ifeq ($(BUILDVARIANT),debug)
CFLAGS += -O0 -g -Wall
else ifeq ($(BUILDVARIANT),normal)
CFLAGS += -O2 -g -Wall
else ifeq ($(BUILDVARIANT),release)
CFLAGS += -O2 -Wall
endif

VPATH := $(BIN)

bin := $(BIN)/${BUILDVARIANT}
#find cpp files in all source dirs
sources := $(foreach dir,$(SRC),$(wildcard $(dir)/*.cpp))
objects := $(addprefix $(bin)/,$(addsuffix .o, $(basename $(sources))))
includes := $(INCLUDES) $(SRC)
libs = $(LIBS)

.PHONY: clean

all: $(bin)/$(EXE)

$(bin)/$(EXE): $(bin) $(objects)
	$(CC) $(foreach lib,$(libs), -l $(lib) ) -o $@ $(objects)


$(bin)/%.o: %.cpp
	echo $@
	mkdir -p $(dir $@)
	${CC} ${CFLAGS} $(foreach include,$(includes),-I$(include) ) -o $@ -c $(filter %.cpp,$^)


clean:
	-rm -rf $(bin)/

%:
	\mkdir -p $@
