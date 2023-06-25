#include <stdio.h>
#include <time.h>

#include "include/bet_compiler.h"
#include "include/ast_node.h"
#include "include/stack.h"
#include "include/symbol_table.h"

int main(int argc, char* argv[]) {
    srand(time(NULL));

    //bet_compile(argv[1]);
    bet_ide_compile(argv[1]);

    /*SymbolTable* table = init_symbol_table();

    Entry* entry1 = init_entry_mem("value", ENTRY_INT, 0x45);
    Entry* entry2 = init_entry_label("Hello World", ENTRY_STRING, &table->num_labels);
    Entry* entry3 = init_entry_label("func", ENTRY_INT, &table->num_labels);
    Entry* entry4 = init_entry_mem("var", ENTRY_INT, 0x41);
    Entry* entry5 = init_entry_mem("value", ENTRY_INT, 0x22);

    symbol_table_insert(table, "global", entry1);
    symbol_table_insert(table, "global", entry2);
    symbol_table_insert(table, "global", entry3);
    symbol_table_insert(table, "global", entry4);
    symbol_table_insert(table, "global", entry5);

    for (int i = 0; i < 10; i++) {
        char test[100];
        sprintf(test, "value%d", i);

        Entry* entry6 = init_entry_mem(test, ENTRY_INT, i);

        if (i % 3)
            symbol_table_insert(table, "func", entry6);
        else
            symbol_table_insert(table, "func4", entry6);

    }

    puts(symbol_table_to_string(table));

    free_symbol_table(table);*/
}
