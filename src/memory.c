#include "memory.h"
#include "common.h"
#include "vm.h"

extern VM vm;

void *reallocate(void *pointer, size_t new_size) {
    if (new_size == 0) return NULL;

    void *result = realloc(pointer, new_size);
    if (result == NULL) exit(1);
    return result;
}

static void free_object(Obj *object) {
    ObjString *str;
    switch (object->type) {
    case OBJ_STRING:
        str = (ObjString *)object;
        FREE_ARRAY(char, str->chars, str->length + 1);
        FREE(ObjString, object);
        break;
    }
}

void free_objects(void) {
    Obj *object = vm.objects;
    while (object != NULL) {
        Obj *next = object->next;
        free_object(object);
        object = next;
    }
}
