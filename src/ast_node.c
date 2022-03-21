#include <stdio.h>
#include <stdlib.h>

#include "include/ast_node.h"

ASTNode* init_ASTNode(int type, char* name) {
    ASTNode* node = malloc(sizeof(struct AST_NODE_STRUCT));

    node->name = name;
    node->type = type;
    node->children = init_list(sizeof(struct AST_NODE_STRUCT));
}
