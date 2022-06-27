#include "include/ast_clean.h"


bool is_symbol_declared_global(Parser* parser, char* symbol_name) {
    for (int i = 0; i < parser->root->children->num_items; i++) {
        if (strcmp(((ASTNode*)parser->root->children->arr[i])->name, symbol_name) == 0)
            return true;
    }

    return false;
}


bool is_symbol_in_scope(ASTNode* curr_symbol, ASTNode* symbol) {
    if (curr_symbol->type == AST_RETURN) {
        return false;
    }

    if (curr_symbol->name != NULL) {

        if (strcmp(curr_symbol->name, symbol->name) == 0) {
            return true;
        }
    }

    //printf("%s---%s\n", curr_symbol->name, curr_token->value);
    for (int i = 0; i < curr_symbol->children->num_items; i++) {
        //printf("%s\n", astnode_to_string(curr_symbol->children->arr[i]));
        return is_symbol_in_scope(curr_symbol->children->arr[i], symbol);
    }
    return false;
}


bool is_symbol_declared(Parser* parser, ASTNode* symbol) {
    if (is_symbol_declared_global(parser, symbol->name))
        return true;


    if (parser->curr_func != NULL) {

        for (int i = 0; i <  ((ASTNode*)parser->curr_func->children->arr[1])->children->num_items; i++) {
            ASTNode* child = ((ASTNode*)parser->curr_func->children->arr[1])->children->arr[i];
            printf("Child %s\n", astnode_to_string(child)); 
            if (child->name != NULL) {
                if (strcmp(child->name, symbol->name) == 0) {
                    return true;
                }
            }

        }

        for (int i = 0; i < ((ASTNode*)parser->curr_func->children->arr[2])->children->num_items; i++) {
            ASTNode* child = ((ASTNode*)parser->curr_func->children->arr[2])->children->arr[i];
            printf("Child2 %s\n", astnode_to_string(child)); 
            if (child->name != NULL) {
                if (strcmp(child->name, symbol->name) == 0) {
                    return true;
                }
            }
        }

    }
    

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

void clean_vars(Parser* parser, ASTNode* curr_symbol) {
    if (curr_symbol->type == AST_DEC_TYPE) {
    }
}

void clean(Parser* parser) {
    List* flag = init_list(sizeof(bool));
    int count = count_nodes(parser->root);
    for (int i = 0; i < count; i++) {
        list_append(flag, (void*)true, sizeof(bool));
    }

    traverse_ast(parser->root, flag, 0, false);
}



