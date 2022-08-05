#include "include/ast_clean.h"


bool is_symbol_declared_global(Parser* parser, char* symbol_name) {
    if (parser->root->children->num_items < 1)
        return false;
    for (int i = 0; i < parser->root->children->num_items - 1; i++) {
        printf("%s\n", astnode_to_string(parser->root->children->arr[i]));
        if (strcmp(((ASTNode*)parser->root->children->arr[i])->name, symbol_name) == 0) {
            return true;
        }
        printf("Bye\n");
    }

    return false;
}


bool verify_symbol_dec(ASTNode* symbol) {
    if (symbol->type == AST_VAR && ((ASTNode*)symbol->children->arr[0])->type != AST_DEC_TYPE) {
        return false; 
    }
     
    if (symbol->type == AST_CALL) {
        return false;
    }

    return true;
}

bool is_symbol_in_scope(ASTNode* curr_symbol, ASTNode* symbol) {

    if (curr_symbol->type == AST_RETURN) {
        return false;
    }

    if (curr_symbol->type == symbol->type || curr_symbol->type == AST_FUNC && (symbol->type == AST_CALL)) {
        printf("%d\n", curr_symbol->children->num_items);
        //printf("%s\n", astnode_to_string(curr_symbol->children->arr[1]));
        if (!verify_symbol_dec(curr_symbol)) {
            return false;
        }

        if (strcmp(curr_symbol->name, symbol->name) == 0) {
            return true;
        }
    }

    if (curr_symbol->children->num_items > 0) {
        for (int i = 0; i < curr_symbol->children->num_items; i++) {
            bool in_scope = is_symbol_in_scope(curr_symbol->children->arr[i], symbol);
            if (in_scope)
                return true;
        }
    }

    return false;
}

ASTNode* get_symbol_in_scope(ASTNode* curr_symbol, ASTNode* symbol) {
    if (curr_symbol->type == AST_RETURN) {
        return NULL;
    }

    if (curr_symbol->type == symbol->type || curr_symbol->type == AST_FUNC && (symbol->type == AST_CALL)) {
        if (!verify_symbol_dec(curr_symbol)) {
            return NULL;
        }

        if (strcmp(curr_symbol->name, symbol->name) == 0) {
            return curr_symbol;
        }
    }

    if (curr_symbol->children->num_items > 0) {
        for (int i = 0; i < curr_symbol->children->num_items; i++) {
            ASTNode* in_scope = get_symbol_in_scope(curr_symbol->children->arr[i], symbol);
            if (in_scope != NULL) {
                return in_scope;
            }
        }
    }

    return NULL;
}



bool is_symbol_declared(Parser* parser, ASTNode* symbol, ASTNode* symbol_scope) {
    if (is_symbol_in_scope(symbol_scope, symbol) || is_symbol_declared_global(parser, symbol->name))
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


bool validate_func(ASTNode* curr_func, ASTNode* ret_val) {
    if (ret_val == NULL)
        return false;

    if (strcmp(((ASTNode*) curr_func->children->arr[0])->name, "int") == 0) {
        if (ret_val->children->num_items < 1)
            return false;
        if (((ASTNode*)ret_val->children->arr[0])->type == AST_EXPR 
                || ((ASTNode*)ret_val->children->arr[0])->type == AST_INT) {
            return true;
        }
    }

    
    if (strcmp(((ASTNode*)curr_func->children->arr[0])->name, "void") == 0) {
        if (ret_val == NULL || ret_val->children->num_items < 1) {
            return true;
        }
    }

    return false;
}

void clean_symbol(Parser* parser, ASTNode* curr_symbol, ASTNode* symbol_scope, int symbol_line) {
    if (!is_symbol_declared(parser, curr_symbol, symbol_scope)) {
        printf("Parser: cannot find symbol '%s'::%d\n", curr_symbol->name, symbol_line);
        printf("Exited with code 1\n");
        exit(1);

    }
    printf("\n\n\n");
}


/* void clean(Parser* parser, ASTNode* curr_symbol, ASTNode* symbol_scope) {
    if (curr_symbol->type == AST_FUNC) {
        symbol_scope = curr_symbol;
        ASTNode* return_val = get_return_val(curr_symbol);
        if (!validate_func(curr_symbol, return_val)) {
            printf("Parser: '%s' symbol has incorrect return value; Expected '%s'\n",
                    curr_symbol->name, ((ASTNode*)curr_symbol->children->arr[0])->name);
            printf("Exited with code 1\n");
            exit(1);
        }

    }

    if (curr_symbol->type == AST_VAR || curr_symbol->type == AST_CALL) {
        if (!verify_symbol_dec(curr_symbol)) {
            clean_symbol(parser, curr_symbol, symbol_scope);
        }

    }

    //printf("\n\nParent %s", astnode_to_string(curr_symbol));

    if (curr_symbol->children->num_items > 0) {
        for (int i = 0; i < curr_symbol->children->num_items; i++) {
            // printf("Index %d->\n", i);
            //printf("-> %s", astnode_to_string(curr_symbol->children->arr[i]));
            clean(parser, curr_symbol->children->arr[i], symbol_scope);
            //printf("end\n");
        }
    }
    symbol_scope = parser->root;
}*/
