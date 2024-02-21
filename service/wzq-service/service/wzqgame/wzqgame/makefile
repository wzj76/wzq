###############################################################################
#
# Generic Makefile for C/C++ Program
#
# Date:   2011/07/01

# Description:
# The makefile searches in <SRCDIRS> directories for the source files
# with extensions specified in <SOURCE_EXT>, then compiles the sources
# and finally produces the <PROGRAM>, the executable file, by linking
# the objectives.

# Usage:
#   $ make           compile and link the program.
#   $ make objs      compile only (no linking. Rarely used).
#   $ make clean     clean the objectives and dependencies.
#   $ make cleanall  clean the objectives, dependencies and executable.
#   $ make rebuild   rebuild the program. The same as make clean && make all.
#==============================================================================

## Customizing Section: adjust the following if necessary.
##=============================================================================

# The executable file name.
# It must be specified.
# PROGRAM   := a.out    # the executable name
PROGRAM   := gamesvrd

# The directories in which source files reside.
# At least one path should be specified.
# SRCDIRS   := .        # current directory
#SRCDIRS   := .
#SRCDIRS += ../proto

SRCDIRS   := .
SRCDIRS += ./proto
SRCDIRS += ./mcgservice
SRCDIRS += ./mcgcommon

ADDONS_BASE_PATH = $(shell find ./addons -type d)
SRCDIRS += $(ADDONS_BASE_PATH)

# The source file types (headers excluded).
# At least one type should be specified.
# The valid suffixes are among of .c, .C, .cc, .cpp, .CPP, .c++, .cp, or .cxx.
# SRCEXTS   := .c      # C program
# SRCEXTS   := .cpp    # C++ program
# SRCEXTS   := .c .cpp # C/C++ program
SRCEXTS   := .cpp .cc

# The flags used by the cpp (man cpp for more).
# CPPFLAGS  := -Wall -Werror # show all warnings and take them as errors
#CPPFLAGS  := -Wall -ansi -D_DEBUG 
CPPFLAGS  := -Wall -ansi -D__DAEMON__  -std=c++11 -std=c++0x

# The compiling flags used only for C.
# If it is a C++ program, no need to set these flags.
# If it is a C and C++ merging program, set these flags for the C parts.
CFLAGS    :=
CFLAGS    +=

BASE_PROTO_PATH := ./proto
SUFFIX = -I
BASE_ADDONS_INCLUDES = $(foreach d,$(ADDONS_BASE_PATH),$(addsuffix $(d),$(SUFFIX)))

# The compiling flags used only for C++.
# If it is a C program, no need to set these flags.
# If it is a C and C++ merging program, set these flags for the C++ parts.
CXXFLAGS  := -g -I. -I$(BASE_PROTO_PATH) -I./mcgcommon -I./mcgservice
CXXFLAGS  += -I/usr/include/lua5.1 -I/usr/include/hiredis -I/usr/include/jsoncpp
CXXFLAGS  += $(BASE_ADDONS_INCLUDES)

# The library and the link options ( C and C++ common).
LDFLAGS :=
LDFLAGS += -llua -lhiredis
LDFLAGS += -lpthread -levent -lprotobuf -llog4cplus -luuid -ldl -lboost_regex -ljsoncpp -lz
LDFLAGS += -L/usr/lib/x86_64-linux-gnu -L/home/liuting/boost_1_57_0/libs/regex/build/gcc
# -Wl,-rpath,./:/home/rong/vpoker/server/lib

## Implict Section: change the following only when necessary.
##=============================================================================
# The C program compiler. Uncomment it to specify yours explicitly.
#CC      = gcc

# The C++ program compiler. Uncomment it to specify yours explicitly.
CXX     = g++

# Uncomment the 2 lines to compile C programs as C++ ones.
#CC      = $(CXX)
#CFLAGS  = $(CXXFLAGS)

# The command used to delete file.
#RM        = rm -f

## Stable Section: usually no need to be changed. But you can add more.
##=============================================================================
SHELL   = /bin/sh
SOURCES = $(foreach d,$(SRCDIRS),$(wildcard $(addprefix $(d)/*,$(SRCEXTS))))
OBJS    = $(foreach x,$(SRCEXTS), \
      $(patsubst %$(x),%.o,$(filter %$(x),$(SOURCES))))
DEPS    = $(patsubst %.o,%.d,$(OBJS))

.PHONY : all objs clean cleanall rebuild

all : $(PROGRAM)

# Rules for creating the dependency files (.d).
#---------------------------------------------------
%.d : %.c
	 @$(CC) -MM -MD $(CFLAGS) $<

%.d : %.C
	@$(CC) -MM -MD $(CXXFLAGS) $<

%.d : %.cc
	@$(CC) -MM -MD $(CXXFLAGS) $< -o $@

%.d : %.cpp
	@$(CC) -MM -MD $(CXXFLAGS) $< -o $@

%.d : %.CPP
	@$(CC) -MM -MD $(CXXFLAGS) $<

%.d : %.c++
	@$(CC) -MM -MD $(CXXFLAGS) $<

%.d : %.cp
	@$(CC) -MM -MD $(CXXFLAGS) $<

%.d : %.cxx
	@$(CC) -MM -MD $(CXXFLAGS) $<

# Rules for producing the objects.
#---------------------------------------------------
objs : $(OBJS)

%.o : %.c
	$(CC) -c $(CPPFLAGS) $(CFLAGS) $<

%.o : %.C
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $<

%.o : %.cc
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $< -o $@

%.o : %.cpp
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $< -o $@

%.o : %.CPP
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $<

%.o : %.c++
	$(CXX -c $(CPPFLAGS) $(CXXFLAGS) $<

%.o : %.cp
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $<

%.o : %.cxx
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $<

# Rules for producing the executable.
#----------------------------------------------
$(PROGRAM) : $(OBJS)
ifeq ($(strip $(SRCEXTS)), .c)  # C file
	$(CC) -o $(PROGRAM) $(OBJS) $(LDFLAGS)
else                            # C++ file
	$(CXX) -o $(PROGRAM) $(OBJS) $(LDFLAGS)
endif

-include $(DEPS)

rebuild: clean all

clean :
	find ./ -name "*.o" -exec rm -rf {} \;
	find ./ -name "*.d" -exec rm -rf {} \;
cleanall: clean
	@$(RM) $(PROGRAM) $(PROGRAM).exe

### End of the Makefile ##  Suggestions are welcome  ## All rights reserved ###
###############################################################################
