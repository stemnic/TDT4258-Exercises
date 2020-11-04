#
# Simple development makefile
#

#
# add more flags here,
# but don't remove the "+="
#
CC = arm-cortexm3-uclinuxeabi-gcc
CFLAGS += -O2 -g -Wall -W -std=c99 #-Wsign-compare -Wfloat-equal -Wformat-security #-Werror
LDFLAGS += -lm

OBJECTS = ball.o display.o paddle.o

all: game

game: game.c ${OBJECTS}
	${CC} ${CFLAGS} game.c ${OBJECTS} -o game ${LDFLAGS}

clean:
	-rm -f game *.o

install:


.PHONY: all install clean
