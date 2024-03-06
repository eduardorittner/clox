CC=gcc -Wall -Wextra -std=c11 -Wno-implicit-fallthrough
LDFLAGS=-Wall -Wextra -std=c11 -Wno-implicit-fallthrough
DEPS=common.h

all: main

main: memory.o chunk.o main.o debug.o value.o vm.o scanner.o compiler.o object.o
	$(CC) $(LDFLAGS) -o clox main.o memory.o chunk.o debug.o value.o vm.o scanner.o compiler.o object.o

%.o: %.c %.h $(DEPS)
	$(CC) -c -o $@  $< $(CFLAGS)

.PHONY = clean
clean:
	rm -f *.o *.gch main
