#include <stdbool.h>
#include "token.h"
#include "ast_node.h"

#ifndef UTIL_H
#define UTIL_H

bool is_prim_type(char* name);

bool is_bool_val(char* name);

bool is_dec_type(char* name);

bool is_expression(char* name);

bool is_literal(char* name);

bool is_built_in(char* name);

bool is_keyword_type(char* name);

bool is_unique_symbol_name(char* name);

bool is_multdiv_op(char* name);

bool is_addsub_op(char* name);

char* str_to_asm_usage(char* str);

typedef enum {
    UNEXPECTED_TOKEN,
    INVALID_ASSIGNMENT,
    INVALID_RETURN
} ParserError;

typedef enum {
    UNEXPECTED_NODE,
    UNDEFINED_VAR,
    SYMBOL_NOT_IN_SCOPE,
    UNEXPECTED_ASSIGNMENT,
    INVALID_FUNC_USAGE,
} x86_ERROR;

void parser_error_handler(int error, char* token_value, int expected_type, int line_num);

void x86_error_handler(int error, ASTNode* node);

#endif
