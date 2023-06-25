#include "include/symbol_table.h"
#include "include/list.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


Entry* init_entry_mem(char* name, int type, int mem_addr) {
    Entry* entry = calloc(1, sizeof(struct ENTRY_STRUCT));

    entry->name = name;
    entry->type = type;
    entry->mem_addr = mem_addr;
    entry->label = NULL;
    entry->next = NULL;

    return entry;
}

Entry* init_entry_label(char* name, int type, unsigned int* label_id) {
    Entry* entry = calloc(1, sizeof(struct ENTRY_STRUCT));

    entry->name = name;
    entry->type = type;
    entry->mem_addr = -1;

    int length = snprintf(NULL, 0, "%d", *label_id);

    char id_str[length];
    snprintf(id_str, length + 1, "%d", *label_id);

    const char* template = "string%s";
    entry->label = calloc(strlen(template) + strlen(id_str) + 1, sizeof(char));
    sprintf(entry->label, template, id_str);

    *label_id = *label_id + 1;

    entry->next = NULL;

    return entry;
}

Entry* copy_entry(Entry* entry) {
    Entry* entry_copy = calloc(1, sizeof(struct ENTRY_STRUCT));

    entry_copy->name = calloc(strlen(entry->name), sizeof(char));
    strcpy(entry_copy->name, entry->name);
    entry_copy->type = entry->type;
    entry_copy->mem_addr = entry->mem_addr;
    if (entry->label != NULL) {
        entry_copy->label = calloc(strlen(entry->label), sizeof(char));
        strcpy(entry_copy->label, entry->label);
    }
    else {
        entry_copy->label = NULL;
    }
    entry_copy->next = entry->next;

    return entry_copy;

}

char* entry_type_to_string(int type) {
    switch (type) {
        case ENTRY_INT:
            return "INT";
            break;

        case ENTRY_STRING:
            return "STRING";
            break;

        case ENTRY_FUNC:
            return "FUNCTION";
            break;

        default:
            return "ERROR: ENTRY TYPE TYPE NOT STRINGABLE";
            break;
     };
}


char* entry_to_string(Entry* entry) {
    const char* type_str = entry_type_to_string(entry->type);

    char* entry_str = NULL;

    if (entry->label == NULL) {
        const char* template = "Entry <name: %s type: %s addr: %s>";
        int addr_str_len = snprintf(NULL, 0, "%d", entry->mem_addr);
        char addr_str[addr_str_len];

        snprintf(addr_str, addr_str_len + 1, "%d", entry->mem_addr);
        entry_str = calloc(strlen(template) + strlen(entry->name) + strlen(type_str) + strlen(addr_str)  + 1, sizeof(char));
        sprintf(entry_str, template, entry->name, type_str, addr_str);
    }
    else {
        const char* template = "Entry <name: %s type: %s label: %s>";
        entry_str = calloc(strlen(template) + strlen(entry->name) + strlen(type_str) + strlen(entry->label)  + 1, sizeof(char));
        sprintf(entry_str, template, entry->name, type_str, entry->label);
    }


    return entry_str;
}

Proc* init_proc(char* name) {
    Proc* proc = calloc(1, sizeof(struct PROC_STRUCT));
    proc->name = calloc(strlen(name) + 1, sizeof(char));
    strcpy(proc->name, name);
    proc->entry = NULL;

    return proc;
}

SymbolTable* init_symbol_table() {
    SymbolTable* table = calloc(1, sizeof(struct SYMBOL_TABLE_STRUCT));

    table->procs = init_list(sizeof(struct PROC_STRUCT));

    table->size = 0;
    table->num_symbols = 0;
    table->top_offset = 0x10;

    return table;
}

int generate_hash(char* key) {
    int hash_code = 0;

    for (int i = 0; i < strlen(key); i++) {
        int multiplier = 1;

        for (int j = 0; j < i; j++) {
            multiplier = multiplier * HASH;
        }

        hash_code = (int) key[i] * multiplier;
    }

    return hash_code;
}

int compress_hash(int hash_code, unsigned int size) {
    int a = rand();
    int b = rand();

    int compressed_hash = ((a * hash_code + b) % PRIME_NUM) % size;

    return compressed_hash;
}


bool symbol_in_scope(SymbolTable* table, char* proc, char* name) {
    if (strcmp(proc, PROC_GLOBAL)) {
        return true;
    }

    for (int i = 0; i < table->size; i++) {
        Entry* curr_entry = ((Proc*)table->procs->arr[i])->entry;

        while (curr_entry != NULL) {
            if (strcmp(curr_entry->name, name) == 0) { // no entries can have the same symbol name
                if (strcmp(((Proc*)table->procs->arr[i])->name, proc) == 0) {
                    return true;
                }
            }
            curr_entry = curr_entry->next;
        }
    }
    return false;
}

void symbol_table_insert(SymbolTable* table, char* proc_name, Entry* entry) {
    entry = copy_entry(entry);

    Proc* proc = symbol_table_lookup_proc(table, proc_name);

    if (proc == NULL) {
        proc = init_proc(proc_name);
        list_append(table->procs, proc, sizeof(struct PROC_STRUCT));
        table->size++;
    }

    Entry* curr_entry = proc->entry;

    if (curr_entry == NULL) {
        proc->entry = copy_entry(entry);
        return;
    }

    while (curr_entry->next != NULL) {
        curr_entry = curr_entry->next;
    }

    curr_entry->next = copy_entry(entry);

    free(entry);
}

Proc* symbol_table_lookup_proc(SymbolTable* table, char* proc_name) {
    for (int i = 0; i < table->procs->num_items; i++) {
        if (strcmp(((Proc*)table->procs->arr[i])->name, proc_name) == 0) {
            return table->procs->arr[i];
        }
    }

    return NULL;
}

Entry* symbol_table_lookup(SymbolTable* table, char* symbol_name) { // must find a way of checking if defined in scope
    for (int i = 0; i < table->size; i++) {
        Entry* curr_entry = ((Proc*)table->procs->arr[i])->entry;

        while (curr_entry != NULL) {
            if (strcmp(curr_entry->name, symbol_name) == 0) { // no entries can have the same symbol name
                return curr_entry;
            }
            curr_entry = curr_entry->next;
        }
    }

    return NULL;
}

char* symbol_table_to_string(SymbolTable* table) {
    char* symbol_table_str = NULL;
    const char* tab = "    ";
    for (int i = 0; i < table->size; i++) {
        Entry* curr_entry = ((Proc*)table->procs->arr[i])->entry;
        const char* template = "Proc id=%s {\n";
        char* proc_str = calloc(strlen(template) + sizeof(((Proc*)table->procs->arr[i])->name), sizeof(char));
        sprintf(proc_str, template, ((Proc*)table->procs->arr[i])->name);

        if (symbol_table_str == NULL) {
            symbol_table_str = calloc(strlen(proc_str) + 1, sizeof(char));
        }
        else {
            symbol_table_str = realloc(symbol_table_str, (strlen(symbol_table_str) + strlen(proc_str) + 1) * sizeof(char));
        }
        strcat(symbol_table_str, proc_str);

        while(curr_entry != NULL) {
            char* curr_entry_str = entry_to_string(curr_entry);

            symbol_table_str = realloc(symbol_table_str, (strlen(symbol_table_str) + strlen(curr_entry_str) + strlen(tab) + 2) * sizeof(char));
            strcat(symbol_table_str, tab);
            strcat(symbol_table_str, curr_entry_str);
            strcat(symbol_table_str, "\n");

            curr_entry = curr_entry->next;
        }

        symbol_table_str = realloc(symbol_table_str, (strlen(symbol_table_str) + 4) * sizeof(char));
        strcat(symbol_table_str, "}");
        strcat(symbol_table_str, "\n");
        strcat(symbol_table_str, "\n");
    }

    return symbol_table_str;
}

void free_symbol_table(SymbolTable* table) { // not freeing correctly
    for (int i = 0; i < table->size; i++) {
        Entry* curr_entry = ((Proc*)table->procs->arr[i])->entry;

        while(curr_entry != NULL) {
            curr_entry = curr_entry->next;
            free(curr_entry);
        }
        free(((Proc*)table->procs->arr[i])->name);
        free(table->procs->arr[i]);
    }

}
