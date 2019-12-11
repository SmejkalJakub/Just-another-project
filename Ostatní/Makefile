CC=gcc
CFLAGS=-Wall -Wextra -std=c99 -pedantic
PROG=compiler
TESTFILE=test
OBJFILES=symTableStack.o symtable.o symstack.o stack.o generator.o fileScanner.o expr.o dynamicString.o compiler.o compiler-main.o

all: $(PROG)

run: compile interpret

$(PROG): $(OBJFILES)
	$(CC) $(CFLAGS) $(OBJFILES) -o $(PROG)

compiler-main.o: compiler-main.c
	$(CC) $(CFLAGS) compiler-main.c -c

compiler.o: compiler.c
	$(CC) $(CFLAGS) compiler.c -c

dynamicString.o: dynamicString.c
	$(CC) $(CFLAGS) dynamicString.c -c

expr.o: expr.c
	$(CC) $(CFLAGS) expr.c -c

fileScanner.o: fileScanner.c
	$(CC) $(CFLAGS) fileScanner.c -c

generator.o: generator.c
	$(CC) $(CFLAGS) generator.c -c

stack.o: stack.c
	$(CC) $(CFLAGS) stack.c -c

symstack.o: symstack.c
	$(CC) $(CFLAGS) symstack.c -c

symtable.o: symtable.c
	$(CC) $(CFLAGS) symtable.c -c

symTableStack.o: symTableStack.c
	$(CC) $(CFLAGS) symTableStack.c -c

compile:
	./compiler $(TESTFILE) > file

interpret:
	./ic19int file 

clean:
	rm -f file *.o $(PROG)






