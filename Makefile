
CC = g++
CFLAGS = -Wall -g -ljpeg
main: main.o map.o player.o
	$(CC) $(CFLAGS) -o main main.o map.o player.o -lglut -lGLU -lGL -lm -ljpeg
 
# The main.o target can be written more simply
 
main.o: main.cpp map.h
	$(CC) $(CFLAGS) -c main.cpp -lglut -lGLU -lGL -lm -lpeg
 
map.o: map.h

player.o: player.h

clean:
	rm -f map.o main main.o player.o
