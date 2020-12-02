CC=g++
CFLAGS=-g -Wall -Wextra

all:
	$(CC) $(CFLAGS) -o generator src/main.cpp src/room.cpp
