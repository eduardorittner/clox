#ifndef clox_compiler_h
#define clox_compiler_h

#include "chunk.h"
#include "object.h"
#include "scanner.h"

typedef struct {
    Token current, previous;
    bool had_error, panic_mode;
} Parser;

bool compile(const char *source, Chunk *chunk);

#endif
