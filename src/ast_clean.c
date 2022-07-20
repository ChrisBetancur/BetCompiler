#include "include/ast_clean.h"


bool is_symbol_declared_global(Parser* parser, char* symbol_name) {
    for (int i = 0; i < parser->root->children->num_items - 1; i++) {
        if (strcmp(((ASTNode*)parser->root->children->arr[i])->name, symbol_name) == 0) {
            return true;
        }
    }

    return false;
}


bool is_symbol_in_scope(ASTNode* curr_symbol, ASTNode* symbol) {
    printf("%s\n", astnode_to_string(curr_symbol));
    if (curr_symbol->type == AST_RETURN) {
        return false;
    }

    if (curr_symbol->type == symbol->type) {
        if (strcmp(curr_symbol->name, symbol->name) == 0) {
            return true;
        }
    }

    if (curr_symbol->children->num_items > 0) {
        for (int i = 0; i < curr_symbol->children->num_items; i++) {
            return is_symbol_in_scope(curr_symbol->children->arr[i], symbol);
        }
    }

    return false;
}


bool is_symbol_declared(Parser* parser, ASTNode* symbol) {
    if (is_symbol_in_scope(parser->root, symbol))
        return true;    

    return false;
}


ASTNode* get_return_val(ASTNode* curr_symbol) {
    if (curr_symbol->type == AST_RETURN) {
        return curr_symbol;
    }

    for (int i = 0; i < curr_symbol->children->num_items; i++) {
        ASTNode* child = get_return_val(curr_symbol->children->arr[i]);

        if (child != NULL)
            return child;
    }

    return NULL;
}

void clean_symbol(Parser* parser, ASTNode* curr_symbol) {
    if (curr_symbol->type == AST_VAR) {
        printf("ehll\n");
        if (((ASTNode*)curr_symbol->children->arr[0])->type == AST_DEC_TYPE) {
            return;
        }
    }
    printf("Clean symbol -> %s\n", astnode_to_string(curr_symbol));
    if (!is_symbol_declared(parser, curr_symbol)) {
        printf("Parser: cannot find symbol '%s'\n", curr_symbol->name);
        printf("Exited with code 1\n");
        exit(1);

    }
    printf("\n\n\n");
}


void clean(Parser* parser, ASTNode* curr_symbol) {
    if (curr_symbol->type == AST_VAR || curr_symbol->type == AST_FUNC) {
        clean_symbol(parser, curr_symbol);
    }

    //printf("\n\nParent %s", astnode_to_string(curr_symbol));

    if (curr_symbol->children->num_items > 0) {
        for (int i = 0; i < curr_symbol->children->num_items; i++) {
            // printf("Index %d->\n", i);
            //printf("-> %s", astnode_to_string(curr_symbol->children->arr[i]));
            clean(parser, curr_symbol->children->arr[i]);
            //printf("end\n");
        }
    }
}



