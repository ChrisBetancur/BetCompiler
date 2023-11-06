#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "include/util.h"

/*
 * Function: is_prim_type
 *
 * Checks is string value is primitive type
 *
 * name: Checks if name is primitive type or not
 *
 * returns: boolean
 */

bool is_prim_type(char *name) {
    if (strcmp(name, "i32") == 0)
        return true;

    if (strcmp(name, "i64") == 0)
        return true;

    if (strcmp(name, "bool") == 0)
        return true;

    if (strcmp(name, "void") == 0)
        return true;


    return false;
}

/*
 * Function: is_bool_val
 *
 * Checks is string value is boolean value
 *
 * name: string value to check
 *
 * returns: boolean
 */

bool is_bool_val(char *name) {

    if (strcmp(name, "true") == 0)
        return true;

    if (strcmp(name, "false") == 0)
        return true;

    return false;
}

/*
 * Function: is_dec_type
 *
 * Checks if name is declaration type
 *
 * name: string value to check
 *
 * returns: boolean
 */

bool is_dec_type(char *name) {
    if (strcmp(name, "string") == 0)
        return true;

    return is_prim_type(name);
}

bool is_expression(char *name) {
    char *curr_char = name;

    while (curr_char != NULL && *curr_char != '\0') {
        if (!isdigit(*curr_char) && !is_multdiv_op(curr_char) && !is_addsub_op(curr_char)) {
            return false;
        }
        curr_char++;
    }
    return true;
}

bool is_literal(char* name) {
    char *curr_char = name;
    while (curr_char != NULL && *curr_char != '\0')
    {
        if (!isalpha(*curr_char)) {
            return false;
        }
        curr_char++;
    }
    return true;

}

/*
 * Function: is_built_int
 *
 * Checks if symbol is a built in symbol within language
 *
 * nae: string value to check
 *
 * return: boolean
 */

bool is_built_in(char *name) {
    if (strcmp(name, "puts") == 0)
        return true;

    if (strcmp(name, "print") == 0)
        return true;

    return false;
}

/*
 * Function: is_keyword_type
 *
 * Checks if name is keyword
 *
 * name: string value to check
 *
 * returns: boolean
 */

bool is_keyword_type(char *name) {
    if (is_dec_type(name))
        return true;

    if (is_bool_val(name))
        return true;

    if (strcmp(name, "return") == 0)
        return true;

    return false;
}

/*
 * Function: is_unique_name
 *
 * Checks if name is a unique symbol name, not keyword type
 *
 * name: string to check
 *
 * returns: boolean
 */

bool is_unique_symbol_name(char *name) {
    if (is_keyword_type(name))
        return false;
    else
        return true;
}

/*
 * Function: is_multdive_op
 *
 * Checks if name is a mult op or div
 *
 * name: string value to check
 */

bool is_multdiv_op(char *name) {
    if (strcmp(name, "*") == 0)
        return true;

    if (strcmp(name, "/") == 0)
        return true;

    return false;
}

/*
 * Function: is_addsub_op
 *
 * Checks if name is add op or sub op
 *
 * name: string value to check
 *
 * returns: boolean
 */

bool is_addsub_op(char *name) {
    if (strcmp(name, "+") == 0)
        return true;

    if (strcmp(name, "-") == 0)
        return true;
    return false;
}

/*
 * Function: type_size
 *
 * Returns the size of the variable type defined in language
 *
 * type: string value to check
 *
 * returns: hex
 */

char type_size(char *type) {
    if (strcmp(type, "i64") == 0)
        return 0x8;

    if (strcmp(type, "i32") == 0)
        return 0x4;

    printf("Unable to get size of %s\n", type);
    exit(1);
}

/*
 * Function: align_stack_frame
 *
 * Calculates new offset aligned at 16 bytes from the offset
 *
 * offset: offset value to check
 *
 * returns: hex
 */


char align_stack_frame(char offset) {
    if (offset % 16 == 0)
        return offset;

    return ((offset + 15) / 16) * 16;
}

/*
 * Function: parser_error_handler
 *
 * Prints appropriate error message for each error code
 *
 * error: error code associated to each type of error
 */

void parser_error_handler(int error, char* token_value, int expected_type, int line_num) {
    switch (error) {
        case UNEXPECTED_TOKEN:
            printf("Parser: '%s' unexpected token; Expected: '%s' :: %d\n", token_value, token_type_to_string(expected_type), line_num);
            printf("Exited with code 1\n");
            exit(1);
            break;

        case INVALID_ASSIGNMENT:
            printf("Parser: '%s' invalid assignment for var :: %d\n", token_value, line_num);
            printf("Exited with code 1\n");
            exit(1);
            break;

        case INVALID_RETURN:
            printf("Parser:'%s' invalid return value for function :: %d\n", token_value, line_num);
            printf("Exited with code 1\n");
            exit(1);
            break;

        default:
            printf("wEIRd\n");
            break;
    };
}

/*
 * Function: frontend_error_handler
 *
 * Prints appropriate error message for each error code
 *
 * error: error code associated to each type of error
 */

void frontend_error_handler(int error, node_t *node) {
    switch (error) {
        case UNEXPECTED_NODE:
            printf("[DEBUG] Assembler: Unexpected node type '%s' for '%s'\n", astnode_type_to_string(node->type), node->name);
            exit(1);
            break;

        case UNDEFINED_VAR:
            printf("[DEBUG] Assembler: Variable '%s' not defined\n", node->name);
            exit(1);
            break;

        case SYMBOL_NOT_IN_SCOPE:
            printf("[DEBUG] Assembler: Variable '%s' not defined in scope\n", node->name);
            exit(1);
            break;

        case UNEXPECTED_ASSIGNMENT:
            printf("[DEBUG] Assembler: Unexpected assignment for variable '%s'\n", node->name);
            exit(1);
            break;

        case INVALID_FUNC_USAGE:
            printf("[DEBUG] Assembler: Function '%s' used", node->name);
            exit(1);
            break;


        default:
            printf("Error Handler: Unexpected error code %d", error);
            exit(1);
            break;
    };
}
