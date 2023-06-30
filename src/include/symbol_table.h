#include <stdbool.h>
#include "list.h"
#include "ast_node.h"

#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#define HASH 33
#define PRIME_NUM 997

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
} Entry;

Entry* init_entry_mem(char* name, int type, char* base, int mem_addr); // for vars

Entry* init_entry_label(char* name, int type, unsigned int* label_id); // for literals

Entry* copy_entry(Entry* entry);

char* entry_type_to_string(int type);

char* entry_proc_to_string(int proc);

char* entry_to_string(Entry* entry);

typedef struct PROC_STRUCT {
    char* name;
    ASTNode* def;
    Entry* entry;
} Proc;

Proc* init_proc(ASTNode* node);

typedef struct SYMBOL_TABLE_STRUCT {
    List* procs;
    unsigned int size;
    unsigned int num_symbols;
    unsigned int top_offset;
    unsigned int num_labels;
} SymbolTable;

SymbolTable* init_symbol_table();

int generate_hash(char* key);

int compress_hash(int hash_code, unsigned int size);

bool symbol_in_scope(SymbolTable* table, Proc* proc, char* name);

void symbol_table_insert(SymbolTable* table, Proc* proc, Entry* entry);

void symbol_table_insert_proc(SymbolTable* table, Proc* proc);

Proc* symbol_table_lookup_proc(SymbolTable* table, char* proc_name);

Entry* symbol_table_lookup(SymbolTable* table, char* symbol_name);

char* symbol_table_to_string(SymbolTable* table);

void free_symbol_table(SymbolTable* table);

#endif
