CC = g++
CFLAGS =

#build debug or release
BUILDVARIANT := debug

#name of the executable
EXE = ezr

#all directories with source files (cpp/h) in them. edit if you add a new one
SRC := src lib/glm
#include these dirs for headers
INCLUDES := lib lib/glut lib/glu
#names of the libs at linking time
LIBS = GL GLU glut
#put binaries in BIN/$(BUILDVARIANT)
BIN := bin

#stop editing here

VPATH := $(BIN)

bin := $(BIN)/${BUILDVARIANT}
#find cpp files in all source dirs
sources := $(foreach dir,$(SRC),$(wildcard $(dir)/*.cpp))
objects := $(addsuffix .o, $(basename $(sources)))
includes := $(INCLUDES) $(SRC)
libs = $(LIBS)

.PHONY: clean

all: $(BIN)/$(EXE)

$(BIN)/$(EXE): $(BIN) $(objects)
	$(CC) $(foreach lib,$(libs), -l $(lib) ) -o $@ $(foreach obj,$(objects), $(BIN)/$(obj) )


%.o: %.cpp
	mkdir -p $(BIN)/$(dir $<)
	${CC} ${CFLAGS} $(foreach include,$(includes),-I$(include) ) -o $(BIN)/$@ -c $(filter %.cpp,$^)


clean:
	-rm -rf $(BIN)/

%:
	\mkdir -p $@
