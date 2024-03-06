#ifndef clox_object_h
#define clox_object_h

#include "common.h"
#include "value.h"

#define TYPEOF_OBJ(value) (as_obj(value)->type)

#define IS_STRING(value) is_obj_type(value, ObjString)

#define AS_STRING(value) ((ObjString *)as_obj(value))
#define AS_CSTRING(value) ((ObjString *)as_obj(value)->chars)

typedef enum {
    OBJ_STRING,
} ObjType;

struct Obj {
    ObjType type;
};

struct ObjString {
    Obj obj;
    size_t length;
    char *chars;
};

ObjString *copy_str(const char *string, size_t length);
void print_obj(Value value);

static inline bool is_obj_type(Value value, ObjType type) {
    return IS_OBJ(value) && AS_OBJ(value)->type == type;
}

#endif
