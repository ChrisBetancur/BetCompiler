#include "include/built_in.h"
//#include "include/ast_node.h"
#include "include/subroutines.h"
#include "include/parser.h"

#include <string.h>
#include <stdbool.h>

// for now only prints 1 element at a time

char* built_in_print_var(ASTNode* call) {
    if (strcmp(((ASTNode*) call->children->arr[0])->name, "int") == 0) {
        const char* print_var_template = "\tmov x7, x8\n"
                                         "%s"
                                         "\tmov x8, #0\n";
        char* print_int = built_in_print_int(((ASTNode*)((ASTNode*)call->children->arr[1])->children->arr[0])->name);

        char* print_var = calloc(strlen(print_var_template) + strlen(print_int) + 1, sizeof(char));
       sprintf(print_var, print_var_template, print_int);

       return print_var;
    }
    return NULL;
}

char* built_in_print_int(char* integer) {

    const char* print_int_template = "\tmov x7, #%s\n"
                                     "\tstr x7, [sp]\n"
                                     "\tadr x0, ascii\n"
                                     "\tbl _printf\n"
                                     "\tmov x7, #0\n\n";


    char* print_int = calloc(strlen(print_int_template) + strlen(integer) + 1, sizeof(char));

    sprintf(print_int, print_int_template, integer);

    return print_int;
}


char* built_in_print_string(ASTNode* node) {
    return NULL;
}
