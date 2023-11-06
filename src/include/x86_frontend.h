#ifndef BET_x86_FRONTEND_H
#define BET_x86_FRONTEND_H

#include "node.h"
#include "symbol_table.h"

char* x86_assemble(node_t *node, proc_t *proc);

// define symbol table here

symbol_table_t *table;

#endif
