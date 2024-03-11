#include "value.h"
#include "common.h"
#include "memory.h"
#include "object.h"

void init_ValueArray(ValueArray *array) {
    array->count = 0;
    array->alloc = 0;
    array->items = NULL;
}

void free_ValueArray(ValueArray *array) {
    FREE_ARRAY(Value, array->items, array->alloc);
    init_ValueArray(array);
}

void write_ValueArray(ValueArray *array, Value value) {
    if (array->alloc < array->count + 1) {
        size_t old_alloc = array->alloc;
        array->alloc = GROW_CAPACITY(old_alloc);
        array->items = GROW_ARRAY(Value, array->items, old_alloc, array->alloc);
    }

    array->items[array->count] = value;
    array->count++;
}

void print_Value(Value value) {
    switch (value.type) {
    case VAL_NIL   : printf("nil"); break;
    case VAL_BOOL  : printf(AS_BOOL(value) ? "true" : "false"); break;
    case VAL_NUMBER: printf("%g", AS_NUMBER(value)); break;
    case VAL_OBJ:
        print_obj(value);
        break;
        break;
    }
}

bool values_equal(Value a, Value b) {
    if (a.type != b.type) return false;
    switch (a.type) {
    case VAL_NIL   : return true;
    case VAL_BOOL  : return AS_BOOL(a) == AS_BOOL(b);
    case VAL_NUMBER: return AS_NUMBER(a) == AS_NUMBER(b);
    case VAL_OBJ   : return AS_OBJ(a) == AS_OBJ(b);
    default        : return false;
    }
}
