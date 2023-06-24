#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#define HASH 33
#define PRIME_NUM 997

typedef struct ENTRY_STRUCT {
    char* name;

    enum {
        ENTRY_INT,
        ENTRY_STRING,
        ENTRY_FUNC
    } type;

    int mem_addr;
    char* label;

    enum {
        PROC_GLOBAL,
        PROC_FUNC,
        PROC_INNER
    } proc;

    struct ENTRY_STRUCT* next;
} Entry;

Entry* init_entry_mem(char* name, int type, int mem_addr, int proc); // for vars

Entry* init_entry_label(char* name, int type, unsigned int* label_id, int proc); // for literals

Entry* copy_entry(Entry* entry);

char* entry_type_to_string(int type);

char* entry_proc_to_string(int proc);

char* entry_to_string(Entry* entry);

typedef struct SYMBOL_TABLE_STRUCT {
    Entry** buckets;
    unsigned int size;
    unsigned int num_symbols;
    unsigned int top_offset;
    unsigned int num_labels;
} SymbolTable;

SymbolTable* init_symbol_table(int size);

int generate_hash(char* key);

int compress_hash(int hash_code, unsigned int size);

void symbol_table_insert(SymbolTable* table, Entry* entry);

Entry* symbol_table_lookup(SymbolTable* table, char* symbol_name);

char* symbol_table_to_string(SymbolTable* table);

void free_symbol_table(SymbolTable* table);

#endif
