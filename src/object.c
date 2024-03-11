#include "object.h"
#include "common.h"
#include "memory.h"
#include "table.h"
#include "value.h"
#include "vm.h"

extern VM vm;

#define ALLOCATE_OBJ(type, object_type)                                        \
    (type *)allocate_obj(sizeof(type), object_type)

static Obj *allocate_obj(size_t size, ObjType type) {
    Obj *obj = (Obj *)reallocate(NULL, size);
    obj->type = type;

    obj->next = vm.objects;
    vm.objects = obj;
    return obj;
}

static ObjString *allocate_string(char *chars, size_t length, u32 hash) {
    ObjString *string = ALLOCATE_OBJ(ObjString, OBJ_STRING);
    string->length = length;
    string->chars = chars;
    string->hash = hash;
    table_set(&vm.strings, string, NIL_VAL);
    return string;
}

static u32 hash_string(const char *key, int length) {
    u32 hash = 2166136261u;
    for (int i = 0; i < length; i++) {
        hash ^= (u8)key[i];
        hash *= 16777619;
    }
    return hash;
}

ObjString *take_str(char *chars, size_t length) {
    u32 hash = hash_string(chars, length);
    ObjString *interned = table_find_string(&vm.strings, chars, length, hash);
    if (interned != NULL) {
        FREE_ARRAY(char, chars, length + 1);
        return interned;
    }

    return allocate_string(chars, length, hash);
}

ObjString *copy_str(const char *chars, size_t length) {
    u32 hash = hash_string(chars, length);

    // If the same string has already been created, just return it
    ObjString *interned = table_find_string(&vm.strings, chars, length, hash);
    if (interned != NULL) return interned;

    char *heap_chars = ALLOCATE(char, length + 1);
    memcpy(heap_chars, chars, length);
    heap_chars[length] = '\0';
    return allocate_string(heap_chars, length, hash);
}

void print_obj(Value value) {
    switch (TYPEOF_OBJ(value)) {
    case OBJ_STRING: printf("%s", AS_CSTRING(value)); break;
    }
}
