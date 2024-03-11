#ifndef clox_chunk_h
#define clox_chunk_h

#include "common.h"
#include "value.h"

typedef enum {
    OP_CONSTANT,
    OP_NIL,
    OP_TRUE,
    OP_FALSE,
    OP_NOT,
    OP_EQUAL,
    OP_GREATER,
    OP_LESS,
    OP_NEGATE,
    OP_ADD,
    OP_SUBTRACT,
    OP_MULTIPLY,
    OP_DIVIDE,
    OP_RETURN,
} OpCode;

// Lines are stored in run-length encoding, which means
// that lineArray->lines[x] is equal to the number of
// tokens in line x
typedef struct {
    size_t count, alloc;
    size_t *items;
} LineArray;

typedef struct {
    size_t count, alloc;
    u8 *code;
    LineArray lines;
    ValueArray constants;
} Chunk;

void init_chunk(Chunk *chunk);
void free_chunk(Chunk *chunk);
void write_chunk(Chunk *chunk, u8 byte, size_t line);
size_t add_constant(Chunk *chunk, Value value);

#endif
