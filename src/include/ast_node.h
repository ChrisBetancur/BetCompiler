#include "list.h"

#ifndef AST_NODE_H
#define AST_NODE_H

typedef struct AST_NODE_STRUCT {
    char* name;
    List* children;
    
    enum {
        AST_FUNC_DEF,
        AST_TYPE,
        AST_COMPOUND,
        AST_STATEMENT,
        AST_VAR_DECLARATION,
        AST_EXPRESSION,
        AST_ASSIGNMENT,
        AST_LITERAL
    } type;
}ASTNode;

ASTNode* init_ASTNode(int type, char* name);

#endif
