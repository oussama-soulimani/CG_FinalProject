
CFLAGS=-lglut -lSDL2 -lGLU -lGL -lm -lstdc++

all: main.o cannon.o audio.o
	g++ -o firework main.o cannon.o audio.o $(CFLAGS) 
cannon.o: cannon.cpp cannon.h
	g++ -Wall  -c cannon.cpp $(CFLAGS)
main.o: main.cpp 
	g++ -Wall -c main.cpp $(CFLAGS)
audio.o: audio.c audio.h
	gcc -Wall -c audio.c $(CFLAGS)
clean: 
	rm *.o firework