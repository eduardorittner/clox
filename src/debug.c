#include "debug.h"
#include "chunk.h"
#include "common.h"
#include "value.h"

static size_t instruction_simple(const char *name, int offset) {
    printf("   %s\n", name);
    return offset + 1;
}

static size_t instruction_constant(const char *name, Chunk *chunk, u8 offset) {
    u8 constant_index = chunk->code[offset + 1];
    printf("   %-16s | %4u ", name, constant_index);
    print_Value(chunk->constants.items[constant_index]);
    printf("\n");
    return offset + 2;
}

size_t get_line(Chunk *chunk, size_t offset) {
    size_t result = 0;
    for (size_t i = 0; i <= offset;) {
        i += chunk->lines.items[result];
        result++;
    }
    return result;
}

void disassemble_chunk(Chunk *chunk, const char *name) {
    printf("== %s ==\n", name);
    printf("Offset | Line | OP               | Constant\n");

    for (size_t offset = 0; offset < chunk->count;) {
        offset = disassemble_instruction(chunk, offset);
    }
}

size_t disassemble_instruction(Chunk *chunk, size_t offset) {
    printf("%07zu ", offset);
    printf("%4zu ", get_line(chunk, offset));

    u8 instruction = chunk->code[offset];
    switch (instruction) {
    case OP_CONSTANT: return instruction_constant("OP_CONSTANT", chunk, offset);
    case OP_NIL     : return instruction_simple("OP_NIL", offset);
    case OP_TRUE    : return instruction_simple("OP_TRUE", offset);
    case OP_FALSE   : return instruction_simple("OP_FALSE", offset);
    case OP_EQUAL   : return instruction_simple("OP_EQUAL", offset);
    case OP_GREATER : return instruction_simple("OP_GREATER", offset);
    case OP_LESS    : return instruction_simple("OP_LESS", offset);
    case OP_NOT     : return instruction_simple("OP_NOT", offset);
    case OP_NEGATE  : return instruction_simple("OP_NEGATE", offset);
    case OP_ADD     : return instruction_simple("OP_ADD", offset);
    case OP_SUBTRACT: return instruction_simple("OP_SUBTRACT", offset);
    case OP_MULTIPLY: return instruction_simple("OP_MULTIPLY", offset);
    case OP_DIVIDE  : return instruction_simple("OP_DIVIDE", offset);
    case OP_RETURN  : return instruction_simple("OP_RETURN", offset);
    default         : printf("Unknown opcode %d\n", instruction); return offset + 1;
    }
}
