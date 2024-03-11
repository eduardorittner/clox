#include "table.h"
#include "common.h"
#include "memory.h"
#include "object.h"
#include "value.h"

#define TABLE_MAX_LOAD 0.75

void init_table(Table *table) {
    table->count = 0;
    table->alloc = 0;
    table->entries = NULL;
}

void free_table(Table *table) {
    FREE_ARRAY(Entry, table->entries, table->alloc);
    init_table(table);
}

static Entry *find_entry(Entry *entries, size_t alloc, ObjString *key) {
    u32 index = key->hash % alloc;
    Entry *tombstone = NULL;

    while (true) {
        Entry *entry = &entries[index];
        if (entry->key == NULL) {
            if (IS_NIL(entry->value)) {
                return tombstone != NULL ? tombstone : entry;
            } else {
                if (tombstone == NULL) tombstone = entry;
            }
        } else if (entry->key == key) {
            return entry;
        }
        index = (index + 1) % alloc;
    }
}

static void resize_table(Table *table, size_t new_alloc) {
    Entry *entries = ALLOCATE(Entry, new_alloc);

    for (size_t i = 0; i < new_alloc; i++) {
        entries[i].key = NULL;
        entries[i].value = NIL_VAL;
    }

    table->count = 0;
    for (size_t i = 0; i < table->alloc; i++) {
        Entry *entry = &table->entries[i];
        if (entry->key == NULL) continue;

        Entry *dest = find_entry(entries, new_alloc, entry->key);
        dest->key = entry->key;
        dest->value = entry->value;
        table->count++;
    }

    FREE_ARRAY(Entry, table->entries, table->alloc);

    table->entries = entries;
    table->alloc = new_alloc;
}

bool table_set(Table *table, ObjString *key, Value value) {
    if (table->count + 1 > table->alloc * TABLE_MAX_LOAD) {
        size_t new_alloc = GROW_CAPACITY(table->alloc);
        resize_table(table, new_alloc);
    }

    Entry *entry = find_entry(table->entries, table->alloc, key);
    bool key_is_new = entry->key == NULL;
    // Increment count only if it's not a tombstone
    if (key_is_new && IS_NIL(entry->value)) table->count++;

    entry->key = key;
    entry->value = value;
    return key_is_new;
}

bool table_get(Table *table, ObjString *key, Value *value) {
    if (table->count == 0) return false;

    Entry *entry = find_entry(table->entries, table->alloc, key);
    if (entry->key == NULL) return false;
    *value = entry->value;
    return true;
}

bool table_delete(Table *table, ObjString *key) {
    if (table->count == 0) return false;

    Entry *entry = find_entry(table->entries, table->alloc, key);
    if (entry->key == NULL) return false;

    // Don't set value to NULL since there would be way of diferentiating a
    // deleted bucket from an empty bucket
    entry->key = NULL;
    entry->value = BOOL_VAL(true);
    return true;
}

void table_add_all(Table *from, Table *to) {
    for (size_t i = 0; i < from->alloc; i++) {
        Entry *entry = &from->entries[i];
        if (entry->key != NULL) { table_set(to, entry->key, entry->value); }
    }
}

ObjString *table_find_string(Table *table, const char *chars, size_t length,
                             u32 hash) {
    if (table->count == 0) return NULL;

    u32 index = hash % table->alloc;

    while (true) {
        Entry *entry = &table->entries[index];
        if (entry->key == NULL) {
            if (IS_NIL(entry->value)) return NULL;
        } else if (entry->key->length == length && entry->key->hash == hash &&
                   memcmp(entry->key->chars, chars, length) == 0) {
            return entry->key;
        }
        index = (index + 1) % table->alloc;
    }
}
