#include "include/symbol_table.h"
#include "include/list.h"
#include "include/node.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


entry_t* init_entry_mem(char *name, int type, char *base, int mem_addr) {
    entry_t *entry = calloc(1, sizeof(struct ENTRY_STRUCT));

    entry->name = name;
    entry->type = type;
    entry->base = base;
    entry->mem_addr = mem_addr;
    entry->label = NULL;
    entry->next = NULL;

    return entry;
}

entry_t* init_entry_label(char *name, int type, unsigned int *label_id) {
    entry_t *entry = calloc(1, sizeof(struct ENTRY_STRUCT));

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

entry_t* copy_entry(entry_t *entry) {
    entry_t *entry_copy = calloc(1, sizeof(struct ENTRY_STRUCT));

    entry_copy->name = calloc(strlen(entry->name) + 1, sizeof(char));
    strcpy(entry_copy->name, entry->name);
    entry_copy->type = entry->type;
    entry_copy->base = entry->base;
    entry_copy->mem_addr = entry->mem_addr;
    if (entry->label != NULL) {
        entry_copy->label = calloc(strlen(entry->label) + 1, sizeof(char));
        strcpy(entry_copy->label, entry->label);
    }
    else {
        entry_copy->label = NULL;
    }

    if (entry->next  == NULL) {
        entry_copy->next = NULL;
    }
    else
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


char* entry_to_string(entry_t *entry) {
    const char *type_str = entry_type_to_string(entry->type);

    char *entry_str = NULL;

    if (entry->label == NULL) {
        const char *template = "Entry <name: %s type: %s base:%s addr: 0x%x>";
        entry_str = calloc(strlen(template) + strlen(entry->name) + strlen(type_str) + strlen(entry->base) + sizeof(entry->mem_addr)  + 1, sizeof(char));
        sprintf(entry_str, template, entry->name, type_str, entry->base, entry->mem_addr);
    }
    else {
        const char *template = "Entry <name: %s type: %s label: %s>";
        entry_str = calloc(strlen(template) + strlen(entry->name) + strlen(type_str) + strlen(entry->label)  + 1, sizeof(char));
        sprintf(entry_str, template, entry->name, type_str, entry->label);
    }


    return entry_str;
}

proc_t* init_proc(node_t *node) {
    proc_t *proc = calloc(1, sizeof(struct PROC_STRUCT));
    proc->name = calloc(strlen(node->name) + 1, sizeof(char));
    proc->def = node;
    proc->offset = 0x0;
    strcpy(proc->name, node->name);
    proc->entry = NULL;

    return proc;
}

char* proc_to_string(proc_t *proc) {
    const char *proc_template = "Proc id=%s {%s}\n";

    char *proc_str = calloc(1, sizeof(char));

    if (proc->entry == NULL) {
        proc_str = realloc(proc_str, (strlen(proc_template) + strlen(proc->name) + 2) * sizeof(char));
        sprintf(proc_str, proc_template, proc->name, "");
        return proc_str;
    }

    entry_t *curr_entry = proc->entry;

    char *proc_entries_str = calloc(2, sizeof(char));
    strcpy(proc_entries_str, "\n");

    while (curr_entry != NULL) {
        char *entry_str = entry_to_string(curr_entry);
        proc_entries_str = realloc(proc_entries_str, (strlen(proc_entries_str) + strlen(entry_str) + 3) * sizeof(char));
        strcat(proc_entries_str, "\t");
        strcat(proc_entries_str, entry_str);
        strcat(proc_entries_str, "\n");
        curr_entry = curr_entry->next;
    }

    proc_str = realloc(proc_str, (strlen(proc_template) + strlen(proc->name) + strlen(proc_entries_str) + 1) * sizeof(char));
    sprintf(proc_str, proc_template, proc->name, proc_entries_str);

    return proc_str;
}

symbol_table_t* init_symbol_table() {
    symbol_table_t *table = calloc(1, sizeof(struct SYMBOL_TABLE_STRUCT));

    table->procs = init_list(sizeof(struct PROC_STRUCT));

    table->size = 0;
    table->num_symbols = 0;

    return table;
}

bool symbol_in_scope(symbol_table_t* table, proc_t *proc, char *name) { // FUNCS WONT SEE GLOBAL VARS SINCE FUNCS ARE CREATED BEFORE THE GLOBAL VAL THROUGH IDENTIFY
    entry_t *curr_entry = proc->entry;

    while (curr_entry != NULL) {
        if (strcmp(curr_entry->name, name) == 0) { // no entries can have the same symbol name
            return true;
        }
        curr_entry = curr_entry->next;
    }

    if (strcmp(proc->name, "global") == 0)
        return false;

    curr_entry = ((proc_t*)table->procs->arr[0])->entry; // check if its global

    while (curr_entry != NULL) {
        if (strcmp(curr_entry->name, name) == 0) { // no entries can have the same symbol name
            if (strcmp(curr_entry->name, proc->name) == 0) {
                return true;
            }
        }
        curr_entry = curr_entry->next;
    }

    return false;
}

void symbol_table_insert(symbol_table_t *table, proc_t *proc, entry_t *entry) {
    if (symbol_table_lookup_proc(table, proc->name) == NULL) {
        printf("proc %s does not exist\n", proc->name);
        exit(1);
    }

    entry_t *curr_entry = proc->entry;

    if (curr_entry == NULL) {
        proc->entry = copy_entry(entry);
        return;
    }

    while (curr_entry->next != NULL) {
        curr_entry = curr_entry->next;
    }

    curr_entry->next = copy_entry(entry);
}

void symbol_table_insert_proc(symbol_table_t *table, proc_t *proc) {
    list_append(table->procs, proc);
    table->size++;
}

proc_t* symbol_table_lookup_proc(symbol_table_t *table, char *proc_name) {
    for (int i = 0; i < table->procs->num_items; i++) {
        if (strcmp(((proc_t*)table->procs->arr[i])->name, proc_name) == 0) {
            return table->procs->arr[i];
        }
    }

    return NULL;
}

entry_t* symbol_table_lookup(symbol_table_t *table, char *symbol_name) { // must find a way of checking if defined in scope -> copy symbol_in_scope
    for (int i = 0; i < table->size; i++) {
        entry_t *curr_entry = ((proc_t*)table->procs->arr[i])->entry;
        while (curr_entry != NULL) {
            if (strcmp(curr_entry->name, symbol_name) == 0) { // no entries can have the same symbol name
                return curr_entry;
            }
            curr_entry = curr_entry->next;
        }
    }

    return NULL;
}

char* symbol_table_to_string(symbol_table_t *table) {
    char *symbol_table_str = calloc(1, sizeof(char));

    for (int i = 0; i < table->procs->num_items; i++) {
        proc_t *curr_proc = table->procs->arr[i];
        char *proc_str = proc_to_string(curr_proc);
        symbol_table_str = realloc(symbol_table_str, (strlen(symbol_table_str) + strlen(proc_str) + 1) * sizeof(char));
        strcat(symbol_table_str, proc_str);
    }
    return symbol_table_str;
}

void free_symbol_table(symbol_table_t *table) { // not freeing correctly
    for (int i = 0; i < table->size; i++) {
        entry_t *curr_entry = ((proc_t*)table->procs->arr[i])->entry;

        while(curr_entry != NULL) {
            curr_entry = curr_entry->next;
            free(curr_entry);
        }
        free(((proc_t*)table->procs->arr[i])->name);
        free(table->procs->arr[i]);
    }

}
