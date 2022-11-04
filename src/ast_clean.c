#include "include/ast_clean.h"

/*
 * Function: is_symbol_declared_global
 *
 * Checks if symbol has been declared global
 *
 * parser: contains the root where we check the global symbols
 * symbol_name: symbol that we need to find that is defined globally
 *
 * returns: boolean if symbol is declared or not
 */

bool is_symbol_declared_global(Parser* parser, char* symbol_name) {
    if (parser->root->children->num_items < 1)
        return false;
    for (int i = 0; i < parser->root->children->num_items - 1; i++) {
        if (strcmp(((ASTNode*)parser->root->children->arr[i])->name, symbol_name) == 0) {
            return true;
        }
    }

    return false;
}

/*
 * Function: verify_symbol_dec
 *
 * Checks if symbol is declared properly
 *
 * symbol: symbol that is being checked if it declared properly
 *
 * returns: true if symbol is declared properly otherwise false
 */

bool verify_symbol_dec(ASTNode* symbol) {
    if (symbol->type == AST_VAR && ((ASTNode*)symbol->children->arr[0])->type != AST_DEC_TYPE) {
        return false; 
    }
     
    if (symbol->type == AST_CALL) {
        return false;
    }

    return true;
}

bool is_symbol_called_in_scope(ASTNode* scope, ASTNode* symbol) {
    for (int i = 0; i < scope->children->num_items; i++) {
        if (strcmp(((ASTNode*) scope->children->arr[i])->name, symbol->name) == 0 
                && ((ASTNode*) scope->children->arr[i])->type != AST_DEC_TYPE) {
            return true;
        }
    }
    return false;
}

bool is_symbol_in_scope(ASTNode* curr_symbol, ASTNode* symbol, ASTNode* curr_scope) { //FIxing scope issues
    if (curr_symbol->type == AST_RETURN) {
        return false;
    }

    ASTNode* prev_scope = NULL;

    if (curr_symbol->type == AST_FUNC) {
        prev_scope = curr_scope;
        curr_scope = curr_symbol;
    }


    if (curr_symbol->type == symbol->type || (curr_symbol->type == AST_FUNC && symbol->type == AST_CALL)) {
        if (!verify_symbol_dec(curr_symbol)) {
            return false;
        }

        if (strcmp(curr_symbol->name, symbol->name) == 0 && is_symbol_called_in_scope(curr_scope, symbol)) {
            return true;
        }
    }

    if (curr_symbol->children->num_items > 0) {
        for (int i = 0; i < curr_symbol->children->num_items; i++) {
            bool in_scope = is_symbol_in_scope(curr_symbol->children->arr[i], symbol, curr_scope);
            if (in_scope)
                return true;

            if (curr_scope->type != AST_GLOBAL) {
                curr_scope = prev_scope;
            }
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
    if (is_symbol_in_scope(symbol_scope, symbol, parser->root) || is_symbol_declared_global(parser, symbol->name))
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
    if (ret_val == NULL) return false;

    if (ret_val->children->num_items == 0) {
        if (strcmp(((ASTNode*) curr_func->children->arr[0])->name, "void") == 0) {
            return true;
        }
        return false;
    }

    if (((ASTNode*) ret_val->children->arr[0])->type == AST_VAR) {
        ASTNode* symbol_def = get_symbol_in_scope(curr_func, ret_val->children->arr[0]); 
        if (symbol_def != NULL) {
            if (strcmp(((ASTNode*) symbol_def->children->arr[0])->name, "int") == 0 
                && strcmp(((ASTNode*) curr_func->children->arr[0])->name, "int") == 0) {
                    return true;
                }
             if (strcmp(((ASTNode*) symbol_def->children->arr[0])->name, "void") == 0 
                && strcmp(((ASTNode*) curr_func->children->arr[0])->name, "void") == 0) {
                    return true;
                }
             if (strcmp(((ASTNode*) symbol_def->children->arr[0])->name, "bool") == 0 
                && strcmp(((ASTNode*) curr_func->children->arr[0])->name, "bool") == 0) {
                    return true;
                }


        }
    }

    if (strcmp(((ASTNode*) curr_func->children->arr[0])->name, "int") == 0) {
        if (ret_val->children->num_items < 1)
            return false;
        if (((ASTNode*)ret_val->children->arr[0])->type == AST_EXPR 
                || ((ASTNode*)ret_val->children->arr[0])->type == AST_BINARY_OP
                || ((ASTNode*)ret_val->children->arr[0])->type == AST_INT) {
            return true;
        }
    }
    
    if (strcmp(((ASTNode*) curr_func->children->arr[0])->name, "bool") == 0) {
        if (((ASTNode*) ret_val->children->arr[0])->type == AST_BOOL) {
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
}

void clean_func(Parser* parser, ASTNode* curr_symbol, ASTNode* symbol_scope, int symbol_line) {
    if (curr_symbol->type == AST_FUNC) {
        symbol_scope = curr_symbol;
        ASTNode* return_val = get_return_val(curr_symbol);
        if (!validate_func(curr_symbol, return_val)) {
            printf("Parser: '%s' symbol has incorrect return value; Expected '%s'::%d\n",
                    curr_symbol->name, ((ASTNode*)curr_symbol->children->arr[0])->name, symbol_line);
            printf("Exited with code 1\n");
            exit(1);
        }

    }
}
