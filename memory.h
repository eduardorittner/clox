#ifndef clox_memory_h
#define clox_memory_h

#include "common.h"

#define ALLOCATE(type, count) (type *)reallocate(NULL, sizeof(type) * (count))

// Calculates the new capacity for an array
#define GROW_CAPACITY(capacity) ((capacity) < 8 ? 8 : (capacity) * 2)

// Calls reallocate with the correct arguments
#define GROW_ARRAY(type, pointer, old_size, new_size)                          \
    (type *)reallocate(pointer, sizeof(type) * (new_size))

#define FREE_ARRAY(type, pointer, alloc) reallocate(pointer, 0)

void *reallocate(void *pointer, size_t new_size);

#endif
