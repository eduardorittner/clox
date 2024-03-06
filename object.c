#include "object.h"
#include "common.h"
#include "memory.h"
#include "value.h"
#include "vm.h"

#define ALLOCATE_OBJ(type, object_type)                                        \
    (type *)allocate_obj(sizeof(type), object_type)

static Obj *allocate_obj(size_t size, ObjType type) {
    Obj *obj = (Obj *)reallocate(NULL, size);
    obj->type = type;
    return obj;
}

static ObjString *allocate_string(char *chars, size_t length) {
    ObjString *string = ALLOCATE_OBJ(ObjString, OBJ_STRING);
    string->length = length;
    string->chars = chars;
    return string;
}

ObjString *copy_str(const char *string, size_t length) {
    char *heap_chars = ALLOCATE(char, length + 1);
    memcpy(heap_chars, string, length);
    heap_chars[length] = '\0';
    return allocate_string(heap_chars, length);
}
