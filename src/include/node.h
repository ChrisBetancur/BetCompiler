#ifndef NODE_H
#define NODE_H

#include "list.h"

/*
 * Struct: AST_NODE_STRUCT
 *
 * Struct defined to represent a node in a Abstract Syntax Tree
 *
 * name: represents the name of the current node
 * children: list of children node
 * type: represents the type of node
 */

typedef struct NODE_STRUCT {
    char *name;
    list_t *children;
    unsigned char offset; // do WE NEED THIS?????

    enum {
        AST_GLOBAL,
        AST_FUNC,
        AST_DEC_TYPE,
        AST_CALL,
        AST_COMPOUND,
        AST_BINARY_OP,
        AST_OPERATOR,
        AST_RETURN_TYPE,
        AST_RETURN,
        AST_VAR,
        AST_TERM,
        AST_EXPR,
        AST_NULL,
        AST_INT,
        AST_BOOL,
        AST_STRING,
        AST_PARAMS,
        AST_BLOCK,
        AST_BUILT_IN
    } type;


} node_t;

node_t* init_node(char *name, int type);

char* astnode_type_to_string(int type);

char* astnode_to_string(node_t* node);

#endif
