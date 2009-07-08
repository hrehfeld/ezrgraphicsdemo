CC = g++

#build debug or release
BUILDVARIANT := debug

#name of the executable
EXE = ezr

SYSTEM = MINGW

#all directories with source files (cpp/h) in them. edit if you add a new one
SRC := src src/gl src/shader src/font lib/glm 
#include these dirs for headers
INCLUDES := lib lib/eigen lib/devil/include

#names of the libs at linking time
LIBS = DevIL ILU ILUT
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
LIBINCLUDES = lib/devil/lib

#put binaries in BIN/$(BUILDVARIANT)
BIN := bin

################################################################################
#stop editing here
################################################################################

include librarypaths.makefile

CFLAGS = -D $(SYSTEM) -D __cplusplus
  ifeq ($(SYSTEM),MINGW)
	CFLAGS += -mpreferred-stack-boundary=2
  endif

ifeq ($(BUILDVARIANT),debug)
CFLAGS += -g -Wall
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
libs := $(LIBS) $(BOOSTLINK)
libincludes := $(LIBINCLUDES) $(BOOSTBIN)/lib

dependencies := $(addprefix $(bin)/,$(addsuffix .d, $(basename $(sources))))

.PHONY: clean all

all: $(dependencies) $(bin)/$(EXE)

#build dependency file, and call sed on it to make the paths right
#(g++ puts $(notdir file): as the target!) and to add the dependency
#file as a target, so it gets rebuild itself if one of the deps
#changes
$(bin)/%.d: %.cpp
	-mkdir -p $(dir $@)
	$(SHELL) -ec '$(CC) -MM $(CFLAGS) $(foreach include,$(includes),-I$(include) ) $< | sed -e "s|\($(notdir $(basename $*))\).o:|$(basename $@).o $(basename $@).d:|g" > $@'

$(bin)/$(EXE): $(bin) $(objects)
	$(CC) $(CFLAGS) -o $@ $(foreach L,$(libincludes),-L$(L)) $(objects) $(foreach lib,$(libs),-l$(lib))


$(bin)/%.o: %.cpp
	mkdir -p $(dir $@)
	${CC} ${CFLAGS} $(foreach include,$(includes),-I$(include) ) -o $@ -c $(filter %.cpp,$^)


clean:
	-rm -f $(objects)

cleandeps:
	-rm -f $(dependencies)

%:
	mkdir $(dir $@)

#debugging
print-%: ; @$(error $* is \'$($*)\' ($(value $*)) (from $(origin $*)))
#OLD_SHELL := $(SHELL)
#SHELL = $(warning [$@ ($^) ($?)])$(OLD_SHELL)



include $(dependencies)

