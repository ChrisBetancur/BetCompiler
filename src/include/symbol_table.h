#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <stdbool.h>
#include "list.h"
#include "node.h"

#define PROC_GLOBAL "global"

typedef struct ENTRY_STRUCT {
    char* name;

    enum {
        ENTRY_INT,
        ENTRY_STRING,
        ENTRY_FUNC
    } type;

    char* base;
    int mem_addr;
    char* label;

    struct ENTRY_STRUCT* next;
} entry_t;

entry_t* init_entry_mem(char *name, int type, char *base, int mem_addr); // for vars

entry_t* init_entry_label(char *name, int type, unsigned int *label_id); // for literals

entry_t* copy_entry(entry_t *entry);

char* entry_type_to_string(int type);

char* entry_proc_to_string(int proc);

char* entry_to_string(entry_t *entry);

typedef struct PROC_STRUCT {
    char* name;
    node_t *def;
    unsigned int offset;
    entry_t *entry;
} proc_t;

proc_t* init_proc(node_t *node);

char* proc_to_string(proc_t *proc);

typedef struct SYMBOL_TABLE_STRUCT {
    list_t *procs;
    unsigned int size;
    unsigned int num_symbols;
    unsigned int num_labels;
} symbol_table_t;

symbol_table_t* init_symbol_table();

bool symbol_in_scope(symbol_table_t *table, proc_t *proc, char *name);

void symbol_table_insert(symbol_table_t *table, proc_t *proc, entry_t *entry);

void symbol_table_insert_proc(symbol_table_t *table, proc_t *proc);

proc_t* symbol_table_lookup_proc(symbol_table_t *table, char *proc_name);

entry_t* symbol_table_lookup(symbol_table_t *table, char *symbol_name);

char* symbol_table_to_string(symbol_table_t *table);

void free_symbol_table(symbol_table_t *table);

#endif
