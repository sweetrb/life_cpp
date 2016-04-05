# makefile for 'life'

TARGET = life

C = life.cpp

COMP = g++

F = -lncurses

all:
	$(COMP) -o $(TARGET) $(C) $(F) 

clean:
	rm -f *.out

