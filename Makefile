#      Project: IFJ – Implementace překladače imperativního jazyka IFJ19
#      Team 65, variant 1
#
#      Authors:
#          Diviš Jan       xdivis12
#
#      File: Makefile

CFLAGS = -pedantic -Wall -Wextra -std=c99
SOURCES = $(wildcard *.c)
OBJECTS = $(SOURCES:.c=.o)
PROGRAMS = ifj2019

all: ifj2019
	rm *.o

ifj2019: $(OBJECTS)
	gcc $(CFLAGS) $^ -o $@

%.o: %.c
	gcc $(CFLAGS) -c $^ -o $@

clean:
	rm *.o $(PROGRAMS)
