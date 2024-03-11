#ifndef clox_vm_h
#define clox_vm_h

#include "chunk.h"
#include "common.h"
#include "table.h"
#include "value.h"

#define STACK_MAX 256

typedef struct {
    Chunk *chunk;
    u8 *ip;
    Value stack[STACK_MAX];
    Value *stack_top;
    Table strings;
    Obj *objects;
} VM;

typedef enum {
    INTERPRET_OK,
    INTERPRET_COMPILE_ERROR,
    INTERPRET_RUNTIME_ERROR,
} InterpretResult;

void init_VM(void);
void free_VM(void);
InterpretResult interpret(const char *source);
void push(Value value);
Value pop();

#endif
