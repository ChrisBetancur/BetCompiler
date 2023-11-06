#ifndef UTIL_H
#define UTIL_H

#include <stdbool.h>
#include "token.h"
#include "node.h"

// Public functions

bool is_prim_type(char *name);

bool is_bool_val(char *name);

bool is_dec_type(char *name);

bool is_expression(char *name);

bool is_literal(char *name);

bool is_built_in(char *name);

bool is_keyword_type(char *name);

bool is_unique_symbol_name(char *name);

bool is_multdiv_op(char *name);

bool is_addsub_op(char *name);

char type_size(char *type);

char align_stack_frame(char offset);

/*
 * Enum: ParserError
 *
 * Enum defined to represent a parser error
 *
 */

typedef enum {
    UNEXPECTED_TOKEN,
    INVALID_ASSIGNMENT,
    INVALID_RETURN
} ParserError;

/*
 * Enum: FrontendError
 *
 * Enum defined to represent a frontend error
 *
 */
typedef enum {
    UNEXPECTED_NODE,
    UNDEFINED_VAR,
    SYMBOL_NOT_IN_SCOPE,
    UNEXPECTED_ASSIGNMENT,
    INVALID_FUNC_USAGE,
} FrontendError;

// Error handler functions

void parser_error_handler(int error, char *token_value, int expected_type, int line_num);

void frontend_error_handler(int error, node_t* node);


#endif
