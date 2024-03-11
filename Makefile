CC = gcc
CFLAGS = -Wall -Wextra -pedantic -fsanitize=address,undefined -I $(INCLUDE_DIR) -Oz -fno-delete-null-pointer-checks -Werror=int-conversion -pie -fno-strict-overflow -fno-strict-aliasing
SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build
BIN_DIR = bin
DEPS = ./include/common.h

objects = main.o object.o value.o vm.o memory.o table.o debug.o compiler.o chunk.o scanner.o

all: clox

clox: $(objects)
	$(CC) $(CFLAGS) -o ./bin/$@ $(patsubst %, $(BUILD_DIR)/%, $(objects))

$(objects): %.o: $(SRC_DIR)/%.c $(INCLUDE_DIR)/%.h $(INCLUDE_DIR)/common.h
	$(CC) $(CFLAGS) -c -o ./build/$@ $<

clean:
	rm -f build/* bin/*

