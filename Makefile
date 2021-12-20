
# eenvoudige makefile
CFLAGS=-lglut -lGLU -lGL -lm -lstdc++

all: main.o cannon.o
	g++  -o firework main.o cannon.o $(CFLAGS) 
cannon.o: cannon.cpp cannon.h
	g++ -Wall  -c cannon.cpp $(CFLAGS)
main.o: main.cpp 
	g++ -Wall -c main.cpp $(CFLAGS)

clean: 
	rm *.o firework