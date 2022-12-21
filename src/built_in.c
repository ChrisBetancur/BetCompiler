#include "include/built_in.h"
//#include "include/ast_node.h"
#include "include/subroutines.h"
#include "include/parser.h"

#include <string.h>
#include <stdbool.h>

// Will use C printf .. Not a clue of how to print anything in aarch assembly
char* built_in_print(ASTNode* node, Stack* stack_frame) {
    return built_in_print_int(node, stack_frame);
}

char* built_in_print_int(ASTNode* node, Stack* stack_frame) {
    ASTNode* value = ((ASTNode*)node->children->arr[0])->children->arr[0];
    printf("value name %s\n", value->name);
    printf("Hello %d\n", stack_frame->top_offset);

    if (stack_frame->top_offset == 0x0) {

        const char* print_int_template = "\tmov x8, #%s\n"
                                     "\tstr x8, [sp]\n"
                                     "\tadr x0, ascii\n"
                                     "\tbl _printf\n";


        char* print_int = calloc(strlen(print_int_template) + strlen(value->name), sizeof(char));
        sprintf(print_int, print_int_template, value->name);
        return print_int;
    }
    const char* print_int_template = "\tmov x8, #%s\n"
                                     "\tstr x8, [sp]\n"
                                     "\tadr x0, ascii\n"
                                     "\tbl _printf\n\n";


    char* print_int = calloc(strlen(print_int_template) + strlen(value->name), sizeof(char));

    int new_offset = stack_frame->top_offset + 0x8;

    sprintf(print_int, print_int_template, value->name);

    //strcpy(print_int, print_int_template);

    return print_int;
}

/*char* built_in_print_digit(ASTNode* node) {

    const char* print_digit_template = "_printDigit:\n";

    char* print_digit_output = calloc(strlen(print_digit_template), sizeof(char));
    strcpy(print_digit_output, print_digit_template);

    bool end = false;

        const char* digit_divide_template = "divide_digits:\n"
                                           "\tmov x1, #10 ;divide value by 10 and get remainder\n"
                                           "\t%s;divide intruction\n"
                                           "\tstr x9, [x0] ;store remainder in memory, ref x0\n"
                                           "\tadd x0, x0, #0x4\n"
                                           "\tcmp x3, #0;\n"
                                           "bgt divide_digits\n";

        char* div_output = divide("x3", "x10", "x1");

        char* digit_div_output = calloc(strlen(digit_divide_template) + strlen(div_output), sizeof(char));

        sprintf(digit_div_output, digit_divide_template, div_output);

        print_digit_output = realloc(print_digit_output, (strlen(print_digit_output) + strlen(digit_div_output)) * sizeof(char));

        strcat(print_digit_output, digit_div_output);


    //char* print_digit = calloc(strlen(print_digit_template) +

    return print_digit_output;
}*/

char* built_in_print_string(ASTNode* node) {
    return NULL;
}
