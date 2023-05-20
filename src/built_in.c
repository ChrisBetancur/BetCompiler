#include "include/built_in.h"
//#include "include/ast_node.h"
#include "include/subroutines.h"
#include "include/parser.h"

#include <string.h>
#include <stdbool.h>

// for now only prints 1 element at a time

char* built_in_print_var(ASTNode* call) {

    /*const char* print_var_template = "\tmov x7, x8\n"
                                         "%s"
                                         "\tmov x8, #0\n";*/

    if (strcmp(((ASTNode*) call->children->arr[0])->name, "int") == 0) {


        const char* print_int_template = "\tldr x7, [sp, #%x]\n"
                                     "\tstr x7, [sp]\n"
                                     "\tadr x0, ascii\n"
                                     "\tbl _printf\n"
                                     "\tmov x7, #0\n\n";


        char* print_int = calloc(strlen(print_int_template) + 1 + 1, sizeof(char));
    //char* print_int = calloc(strlen(print_int_template) + strlen(integer) + 1, sizeof(char));

        sprintf(print_int, print_int_template, call->offset);

        /*char* print_var = calloc(strlen(print_var_template) + strlen(print_int) + 1, sizeof(char));
        sprintf(print_var, print_var_template, print_int);

        printf("entered \n");
        printf("%s\n", print_var);*/
        print_ast_at_node(call);
        printf("%s\n", print_int);

        return print_int;
    }
    return NULL;
}

char* built_in_print_int(char* integer) {

    /*const char* print_int_template = "\tldr x7, [sp, #%x]\n"
                                     "\tstr x7, [sp]\n"
                                     "\tadr x0, ascii\n"
                                     "\tbl _printf\n"
                                     "\tmov x7, #0\n\n";*/

    const char* print_int_template = "\tmov x7, #%s\n"
                                     "\tstr x7, [sp]\n"
                                     "\tadr x0, ascii\n"
                                     "\tbl _printf\n"
                                     "\tmov x7, #0\n";
                                     //"\tstr x7, [sp]\n\n";


    //char* print_int = calloc(strlen(print_int_template) + 1 + 1, sizeof(char));
    char* print_int = calloc(strlen(print_int_template) + strlen(integer) + 1, sizeof(char));

    sprintf(print_int, print_int_template, integer);

    //strcpy(print_int, print_int_template);


    return print_int;
}


char* built_in_print_string(ASTNode* node) {
    return NULL;
}
