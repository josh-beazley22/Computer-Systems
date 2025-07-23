CC=gcc
CFLAGS=-g

run: build
	./hello

build: main.o foo.o
	gcc -o hello main.o foo.o

main.o: foo.h main.c
	gcc -c main.c

foo.o: foo.h foo.c
	gcc -c foo.c

clean:
	rm -rf *.o a.out