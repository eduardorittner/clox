#ifndef clox_debug_h
#define clox_debug_h

#include "chunk.h"
#include "common.h"

void disassemble_chunk(Chunk *chunk, const char *name);
size_t disassemble_instruction(Chunk *chunk, size_t offset);
size_t get_line(Chunk *chunk, size_t offset);

#endif
