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

bool is_prim_type(char* name) {
    if (strcmp(name, "int") == 0)
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

bool is_bool_val(char* name) {

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

bool is_dec_type(char* name) {
    if (strcmp(name, "string") == 0)
        return true;

    return is_prim_type(name);
}


bool is_expression(char* name) {
    char* curr_char = name;

    if (strlen(name) == 1) {
        return isdigit(*name);
    }

    while (curr_char != NULL && *curr_char != '\0') {
        if (!isdigit(*curr_char) && !is_multdiv_op(curr_char) && !is_addsub_op(curr_char)) {
            return false;
        }
        curr_char++;
    }
    return true;
}

bool is_literal(char* name) {
    char* curr_char = name;
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

bool is_built_in(char* name) {
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

bool is_keyword_type(char* name) {
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

bool is_unique_symbol_name(char* name) {
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

bool is_multdiv_op(char* name) {
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

bool is_addsub_op(char* name) {
    if (strcmp(name, "+") == 0)
        return true;

    if (strcmp(name, "-") == 0)
        return true;
    return false;
}

