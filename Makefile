CC=g++
PNAME=fltime

CFLAGS=-c -Wall
GKTFLAGS=`pkg-config --cflags gtk+-3.0` `pkg-config --libs gtk+-3.0` 

all: fltime

rebuild: clear all

fltime: main.o
	$(CC) main.o -o $(PNAME) $(GKTFLAGS)

main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp $(GKTFLAGS)

clear: 
	rm -rf *.o $(PNAME)