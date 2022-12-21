#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/ast_node.h"

/*
 * Function: init_ASTNode
 *
 * Creates ast node to store value and type of node
 *
 * name: name of node
 * type: type of node
 *
 * returns: ast node ref
 */

ASTNode* init_ASTNode(char* name, int type) {
    ASTNode* node = malloc(sizeof(struct AST_NODE_STRUCT));

    node->name = name;
    node->type = type;
    node->offset = NULL;
    node->children = init_list(sizeof(struct AST_NODE_STRUCT));

    return node;
}

/*
 * Function: astnode_type_to_string
 *
 * Translates ast node type to string
 *
 * type: type of node
 *
 * returns: astnode type in string format
 */

char* astnode_type_to_string(int type) {
    switch (type) {
        case AST_GLOBAL:
            return "GLOBAL SCOPE";
            break;
        case AST_FUNC:
            return "FUNCTION";
            break;
        case AST_DEC_TYPE:
            return "DECLARATION TYPE";
            break;
        case AST_CALL:
            return "SYMBOL CALLED";
            break;
        case AST_COMPOUND:
            return "COMPOUND";
            break;
        case AST_BINARY_OP:
            return "BINARY OPERATION";
            break;
        case AST_OPERATOR:
            return "OPERATOR";
            break;
        case AST_RETURN_TYPE:
            return "RETURN TYPE";
            break;
        case AST_RETURN:
            return "RETURN";
            break;
        case AST_VAR:
            return "VARIABLE";
            break;
        case AST_TERM:
            return "TERM";
            break;
        case AST_EXPR:
            return "EXPRESSION";
            break;
        case AST_NULL:
            return "NULL";
            break;
        case AST_INT:
            return "INTEGER";
            break;
        case AST_BOOL:
            return "BOOLEAN";
            break;
        case AST_LITERAL:
            return "LITERAL";
            break;
         case AST_PARAMS:
            return "PARAMS";
            break;
         case AST_BLOCK:
            return "BLOCK";
            break;
        case AST_BUILT_IN:
            return "BUILT_IN";
        default:
            return "ERROR: AST NODE TYPE TYPE NOT STRINGABLE";
            break;
    }

}

/*
 * Function: astnode_to_string
 *
 * Translates node to string, creates template for string
 *
 * node: node used to translate
 *
 * returns: node as a string
 */

char* astnode_to_string(ASTNode* node) {
    const char* type_str = astnode_type_to_string(node->type);
    char* node_str;
    if (node->name != NULL) {
        const char* template = "Ast Node <name: %s type: %s>\n";
        node_str = calloc(strlen(type_str) + strlen(template) + strlen(node->name), sizeof(char));
        sprintf(node_str, template, node->name, type_str);
    }
    else {
        const char* template = "Ast Node <type: %s>\n";
        node_str = calloc(strlen(type_str) + strlen(template), sizeof(char));
        sprintf(node_str, template, type_str);
    }

    return node_str;
}
