PRJ=IFJ

PROGS=$(PRJ)-test
CC=gcc
CFLAGS=-Wall -std=c99 -pedantic -lm

all: $(PROGS)

$(PRJ)-test: compiler.c dynamicString.c fileScanner.c
	$(CC) $(CFLAGS) -o test compiler.c dynamicString.c fileScanner.c

clean:
	rm -f *.o *.out $(PROGS)
