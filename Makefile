CC = g++
CFLAGS =


VARIANT := debug

EXE = ezr

SRC := src
BIN := bin/${VARIANT}

VPATH := $(BIN) $(SRC)

#find cpp files in src/
sources := $(subst $(SRC)/,,$(wildcard $(SRC)/*.cpp))
objects := $(addsuffix .o, $(basename $(sources)))
includes := lib lib/glut lib/glu
libs = GL GLU glut

.PHONY: clean

all: $(BIN) $(objects)
	$(CC) $(foreach lib,$(libs), -l $(lib) ) \
-o $(BIN)/$(EXE) $(foreach obj,$(objects), $(BIN)/$(obj) )

$(BIN):
	\mkdir -p $(BIN)

%.o: %.cpp
	${CC} ${CFLAGS} $(foreach include,$(includes),-I$(include) ) -o $(BIN)/$@ -c $<


clean:
	-rm $(BIN)/*.o