# This is a comment line
CC=g++
# CFLAGS will be the options passed to the compiler.
CFLAGS=-c -Wall
OBJECTS=EECS_348_Assignment_3.o
all: prog

prog: $(OBJECTS)
	$(CC) $(OBJECTS) -o prog

%.o: %.cpp
	$(CC) $(CFLAGS) $<

clean:
	rm -rf *.o prog
