CC=g++
#OBJS=course.o selcour.o main.o list.o
EXE=main

CURR_DIR=$(shell pwd)
MY_SRC_FILES=$(wildcard $(CURR_DIR)/*.c)

SRCDIRS:=.
SRCDIRS+=./src

SRCEXTS:= .cpp .c
SRCEXTS+=

SOURCES:=$(foreach d,$(SRCDIRS),$(wildcard $(addprefix $(d)/*,$(SRCEXTS))))
$(warning $(addprefix $(d)/*,$(SRCEXTS)))
$(warning $(SOURCES))

OBJS:=$(foreach x,$(SRCEXTS),$(patsubst %$(x),%.o,$(filter %$(x),$(SOURCES))))
$(warning $(OBJS))

all:clear
clear:$(EXE)
$(EXE):$(OBJS)
	$(CC) -o $@ $^
	$(warning $(MY_SRC_FILES))
$(OBJS):%.o:%.c
	$(CC) -o $@ -c $^
clear:
	rm *.o
