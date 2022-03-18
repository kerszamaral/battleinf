# raylib compiled with
# make OS=Windows_NT CC=x86_64-w64-mingw32-gcc

APPNAME=game

LDFLAGS:=-L../raylib/src -lm -lraylib  -pthread -lopengl32 -lgdi32 -lwinmm

CFLAGS:= -g -Wfatal-errors -pedantic -Wall -Wextra -Werror
CFLAGS+= -std=c99 -I ./include -I ../raylib/src

SRC:=$(wildcard *.c) \
	 $(wildcard src/*.c)
OBJ:=$(SRC:src/%.c=obj/%.o)
INC:=$(wildcard include/*.h)

CC=x86_64-w64-mingw32-gcc

$(APPNAME).exe: $(OBJ)
	$(CC) $(OBJ) -o $(APPNAME).exe $(LDFLAGS)

$(OBJ): obj/%.o : src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: debug release
debug: CFLAGS+= -g
release: CFLAGS+= -O3

debug release: clean $(APPNAME)

.PHONY:	clean
clean:
	del obj\*
	del $(APPNAME).exe

style: $(SRC) $(INC)
	astyle -A10 -s4 -S -p -xg -j -z2 -n src/* include/*
