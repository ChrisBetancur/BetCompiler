#include "list.h"

#ifndef AST_NODE_H
#define AST_NODE_H

/*
 * Struct: AST_NODE_STRUCT
 *
 * Struct defined to represent a node in a Abstract Syntax Tree
 *
 * name: represents the name of the current node
 * children: list of children node
 * type: represents the type of node
 */

typedef struct AST_NODE_STRUCT {
    char* name;
    List* children;
    
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
        AST_LITERAL,
        AST_PARAMS,
        AST_BLOCK
    } type;
}ASTNode;

ASTNode* init_ASTNode(char* name, int type);

char* astnode_type_to_string(int type);

char* astnode_to_string(ASTNode* node);

#endif
