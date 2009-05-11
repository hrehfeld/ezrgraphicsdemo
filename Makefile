CC = g++
CFLAGS =


VARIANT := debug

EXE = ezr

SRC := src
BIN := bin/${VARIANT}

VPATH := $(BIN) $(SRC)

sources := $(subst $(SRC)/,,$(wildcard $(SRC)/*.cpp))
objects := $(addsuffix .o, $(basename $(sources)))

.PHONY: clean

all: $(BIN) ${objects}

$(BIN):
	\mkdir -p $(BIN)

%.o: %.cpp
	${CC} ${CFLAGS} -o $(BIN)/$@ $<


clean:
	-rm $(BIN)/*.o