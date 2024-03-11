#ifndef clox_image_h
#define clox_image_h

#include "common.h"
#include "value.h"

typedef struct {
    ObjString *key;
    Value value;
} Entry;

typedef struct {
    size_t count, alloc;
    Entry *entries;
} Table;

void init_table(Table *table);
void free_table(Table *table);
bool table_set(Table *table, ObjString *key, Value value);
bool table_get(Table *table, ObjString *key, Value *value);
bool table_delete(Table *table, ObjString *key);
void table_add_all(Table *from, Table *to);
ObjString *table_find_string(Table *table, const char *chars, size_t length,
                             u32 hash);

#endif
