#ifndef clox_object_h
#define clox_object_h

#include "common.h"
#include "value.h"

#define TYPEOF_OBJ(value) (AS_OBJ(value)->type)

#define IS_STRING(value) is_obj_type(value, OBJ_STRING)

#define AS_STRING(value) ((ObjString *)AS_OBJ(value))
#define AS_CSTRING(value) (((ObjString *)AS_OBJ(value))->chars)

typedef enum {
    OBJ_STRING,
} ObjType;

struct Obj {
    ObjType type;
    struct Obj *next;
};

struct ObjString {
    Obj obj;
    size_t length;
    char *chars;
    u32 hash;
};

ObjString *take_str(char *chars, size_t length);
ObjString *copy_str(const char *string, size_t length);
void print_obj(Value value);

static inline bool is_obj_type(Value value, ObjType type) {
    return IS_OBJ(value) && AS_OBJ(value)->type == type;
}

#endif
