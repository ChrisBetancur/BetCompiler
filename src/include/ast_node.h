#include "list.h"

#ifndef AST_NODE_H
#define AST_NODE_H

typedef struct AST_NODE_STRUCT {
    char* name;
    List* children;
    
    enum {
        AST_GLOBAL,
        AST_FUNC,
        AST_DEF_TYPE,
        AST_CALL,
        AST_COMPOUND,
        AST_BINARY_OP,
        AST_OPERATOR,
        AST_RETURN_ST,
        AST_VAR,
        AST_TERM,
        AST_EXPR,
        AST_NULL,
        AST_INT,
        AST_LITERAL
    } type;
}ASTNode;

ASTNode* init_ASTNode(int type, char* name);

char* astnode_type_to_string(int type);

char* astnode_to_string(ASTNode* node);

#endif
