#include "vm.h"
#include "common.h"
#include "compiler.h"
#include "debug.h"
#include "value.h"

VM vm;

static void reset_stack(void) { vm.stack_top = (Value *)&vm.stack; }

static bool stack_is_empty(void) { return (vm.stack == vm.stack_top); }

static bool stack_has(int items) { return (vm.stack == vm.stack_top - items); }

static Value peek(int distance) { return vm.stack_top[-1 - distance]; }

static bool is_falsey(Value value) {
    return IS_NIL(value) || (IS_BOOL(value) && !AS_BOOL(value));
}

static void vm_error(const char *format, ...) {
    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
    fputs("\n", stderr);

    size_t instruction = vm.ip - vm.chunk->code - 1;
    size_t line = get_line(vm.chunk, instruction);
    fprintf(stderr, "[line %zu] in script\n", line);
    reset_stack();
}

void init_VM(void) { reset_stack(); }

void free_VM(void) {}

static InterpretResult run(void) {
#define read_byte() (*vm.ip++)
#define read_constant() (vm.chunk->constants.items[read_byte()])
#define binary_op(valueType, op)                                               \
    do {                                                                       \
        if (!IS_NUMBER(peek(0)) || !IS_NUMBER(peek(1))) {                      \
            vm_error("Operands must be numbers.");                             \
            return INTERPRET_RUNTIME_ERROR;                                    \
        }                                                                      \
        double b = AS_NUMBER(pop());                                           \
        double a = AS_NUMBER(pop());                                           \
        push(valueType(a op b));                                               \
    } while (false)

    while (true) {

#ifdef DEBUG_TRACE_EXECUTION
        printf("        ");
        for (Value *slot = (Value *)vm.stack; slot < vm.stack_top; slot++) {
            printf("[ ");
            print_Value(*slot);
            printf(" ]");
        }
        printf("\n");
        disassemble_instruction(vm.chunk, (size_t)(vm.ip - vm.chunk->code));
#endif
        u8 instruction;
        switch (instruction = read_byte()) {
        case OP_CONSTANT: {
            Value constant = read_constant();
            push(constant);
            break;
        }
        case OP_NIL:
            push(NIL_VAL);
            break;
        case OP_TRUE:
            push(BOOL_VAL(true));
            break;
        case OP_FALSE:
            push(BOOL_VAL(false));
            break;
        case OP_EQUAL:
            Value b = pop();
            Value a = pop();
            push(BOOL_VAL(values_equal(a, b)));
            break;
        case OP_GREATER:
            binary_op(BOOL_VAL, >);
            break;
        case OP_LESS:
            binary_op(BOOL_VAL, <);
            break;
        case OP_NOT:
            push(BOOL_VAL(is_falsey(pop())));
            break;
        case OP_NEGATE:
            if (stack_is_empty()) {
                vm_error("Can't negate because the stack is empty.");
                return INTERPRET_RUNTIME_ERROR;
            }
            if (!IS_NUMBER(peek(0))) {
                vm_error("Operand must be an a number.");
                return INTERPRET_RUNTIME_ERROR;
            }

            push(NUMBER_VAL(-AS_NUMBER(pop())));
            break;
        case OP_ADD:
            if (stack_is_empty() | stack_has(1)) {
                return INTERPRET_COMPILE_ERROR;
            }
            binary_op(NUMBER_VAL, +);
            break;
        case OP_SUBTRACT:
            if (stack_is_empty() | stack_has(1)) {
                return INTERPRET_COMPILE_ERROR;
            }
            binary_op(NUMBER_VAL, -);
            break;
        case OP_MULTIPLY:
            if (stack_is_empty() | stack_has(1)) {
                return INTERPRET_COMPILE_ERROR;
            }
            binary_op(NUMBER_VAL, *);
            break;
        case OP_DIVIDE:
            if (stack_is_empty() | stack_has(1)) {
                return INTERPRET_COMPILE_ERROR;
            }
            binary_op(NUMBER_VAL, /);
            break;
        case OP_RETURN:
            print_Value(pop());
            printf("\n");
            return INTERPRET_OK;
        default:
            return INTERPRET_COMPILE_ERROR;
        }
    }

#undef binary_op
#undef read_constant
#undef read_byte
}

InterpretResult interpret(const char *source) {
    Chunk chunk;
    init_chunk(&chunk);

#ifdef DEBUG_PRINT_CODE
    printf("\n=============\nStarting compilation\n\n");
#endif
    if (!compile(source, &chunk)) {
        free_chunk(&chunk);
        return INTERPRET_COMPILE_ERROR;
    }
#ifdef DEBUG_PRINT_CODE
    printf("\nCompilation succesful\n=============\n");
#endif

    vm.chunk = &chunk;
    vm.ip = vm.chunk->code;

    InterpretResult result = run();

    free_chunk(&chunk);
    return result;
}

void push(Value value) {
    *vm.stack_top = value;
    vm.stack_top++;
}

Value pop(void) {
    vm.stack_top--;
    return *vm.stack_top;
}
