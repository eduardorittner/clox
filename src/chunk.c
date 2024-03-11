#include "chunk.h"
#include "memory.h"
#include "value.h"

void init_LineArray(LineArray *array) {
    array->count = 0;
    array->alloc = 0;
    array->items = NULL;
}

void init_chunk(Chunk *chunk) {
    chunk->count = 0;
    chunk->alloc = 0;
    chunk->code = NULL;
    init_LineArray(&chunk->lines);
    init_ValueArray(&chunk->constants);
}

void free_LineArray(LineArray *array) {
    FREE_ARRAY(size_t, array->items, array->alloc);
    init_LineArray(array);
}

void free_chunk(Chunk *chunk) {
    FREE_ARRAY(u8, chunk->code, chunk->alloc);
    FREE_ARRAY(size_t, &chunk->lines, chunk->alloc);
    free_LineArray(&chunk->lines);
    free_ValueArray(&chunk->constants);
    init_chunk(chunk);
}

void write_line(LineArray *array, size_t line) {
    // The condition for growing the array is different, we check
    // whether the current size fits the line we want to write,
    // if not, we call grow_capacity until new_alloc is large enough
    // to accomodate line
    if (line >= array->alloc) {
        size_t old_alloc = array->alloc;
        size_t new_alloc = GROW_CAPACITY(old_alloc);
        while (new_alloc < line) { new_alloc = GROW_CAPACITY(new_alloc); }
        array->alloc = new_alloc;
        array->items =
            GROW_ARRAY(size_t, array->items, old_alloc, array->alloc);
        memset(&array->items[old_alloc], 0,
               (array->alloc - old_alloc) * sizeof(size_t));
    }
    array->items[line]++;
}

void write_chunk(Chunk *chunk, u8 byte, size_t line) {
    if (chunk->alloc < chunk->count + 1) {
        int old_alloc = chunk->alloc;
        chunk->alloc = GROW_CAPACITY(old_alloc);
        chunk->code = GROW_ARRAY(u8, chunk->code, old_alloc, chunk->alloc);
    }

    chunk->code[chunk->count] = byte;
    write_line(&chunk->lines, line);
    chunk->count++;
}

// Returns u8 since the constant index must fit inside a byte in the chunk
size_t add_constant(Chunk *chunk, Value value) {
    write_ValueArray(&chunk->constants, value);
    return chunk->constants.count - 1;
}
