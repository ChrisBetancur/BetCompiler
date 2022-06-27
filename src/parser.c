#include "include/parser.h"
#include <ctype.h>
#include "include/token.h"
#include "include/ast_node.h"

Parser* init_parser(Lexer* lexer) {
    Parser* parser = malloc(sizeof(struct PARSER_STRUCT));

    parser->root = NULL;
    parser->curr_func = NULL;
    parser->lexer = lexer;
    parser->curr_token = lexer_next_token(parser->lexer);
    parser->curr_line = 1;
    return parser;
}

void parser_eat(Parser* parser, int type) {
    if (parser->curr_token->type != type) {
        printf("Parser: '%s' unexpected token; Expected: '%s' token at line %d\n", parser->curr_token->value, token_type_to_string(type), parser->curr_line);
        printf("Exited with code 1\n");
        exit(1);
    }
    //printf("%s\n", token_to_string(parser->curr_token));
    parser->curr_token = lexer_next_token(parser->lexer);
}

bool is_prim_type(char* name) {
    if (strcmp(name, "int") == 0)
        return true;

    if (strcmp(name, "bool") == 0)
        return true;

    if (strcmp(name, "void") == 0)
        return true;


    return false;
}


bool is_keyword_type(char* name) { // 
    if (is_prim_type(name))
        return true;


    if (strcmp(name, "return") == 0)
        return true;

    if (strcmp(name, "print") == 0)
        return true;

    return false;
}

bool is_unique_symbol_name(char* name) { //future must implement to check if the symbol has already been defined in the scope
    if (is_keyword_type(name))
        return false;
    else
        return true;
}

bool is_multdiv_op(char* name) {
    if (strcmp(name, "*") == 0)
        return true;

    if (strcmp(name, "/") == 0)
        return true;

    return false;
}

bool is_addsub_op(char* name) {
    if (strcmp(name, "+") == 0)
        return true;

    if (strcmp(name, "-") == 0)
        return true;
    
    return false;
}


ASTNode* parse_factor(Parser* parser) {
    ASTNode* symbol;
    if (parser->curr_token->type == TOKEN_INT) {
        symbol = init_ASTNode(AST_INT, parser->curr_token->value);
        parser_eat(parser, TOKEN_INT);
        return symbol;
    }
    
    else if (parser->curr_token->type == TOKEN_LPARAN) {
        symbol = init_ASTNode(AST_TERM, NULL);
        parser_eat(parser, TOKEN_LPARAN);
        list_append(symbol->children, parse_expr(parser), sizeof(struct AST_NODE_STRUCT));
        parser_eat(parser, TOKEN_RPARAN);
        //printf("Term ---> %s\n", astnode_to_string(((ASTNode*)symbol->children->arr[0])->children->arr[2]));
        return symbol;
    }
    return NULL;
}

ASTNode* parse_term(Parser* parser) {
    ASTNode* symbol = parse_factor(parser);
    while (is_multdiv_op(parser->curr_token->value)) {
        ASTNode* prev_symbol = symbol;
        symbol = init_ASTNode(AST_BINARY_OP, NULL);
        char* op = parser->curr_token->value;
        if (parser->curr_token->type == TOKEN_MULT) {
            parser_eat(parser, TOKEN_MULT);
        }
        
        else if (parser->curr_token->type == TOKEN_DIV) {
            parser_eat(parser, TOKEN_DIV);
        }
        list_append(symbol->children, init_ASTNode(AST_OPERATOR, op), sizeof(struct AST_NODE_STRUCT));
        list_append(symbol->children, prev_symbol, sizeof(struct AST_NODE_STRUCT));
        ASTNode* right = parse_factor(parser);
        list_append(symbol->children, right, sizeof(struct AST_NODE_STRUCT));
    }
    //printf("%s\n", astnode_to_string(symbol));
    return symbol;
}

ASTNode* parse_expr(Parser* parser) { 
    //ASTNode* symbol = NULL;

    ASTNode* symbol = parse_term(parser);
   
    //printf("Hello %s\n", astnode_to_string(symbol));
    while (is_addsub_op(parser->curr_token->value)) {
        ASTNode* prev_symbol = symbol;
        symbol = init_ASTNode(AST_BINARY_OP, NULL);
        char* op = parser->curr_token->value;

        if (parser->curr_token->type == TOKEN_ADD) {
            parser_eat(parser, TOKEN_ADD);
        }

        else if (parser->curr_token->type == TOKEN_SUB) {
            parser_eat(parser, TOKEN_SUB);
        }
        list_append(symbol->children, init_ASTNode(AST_OPERATOR, op), sizeof(struct AST_NODE_STRUCT));
        list_append(symbol->children, prev_symbol, sizeof(struct AST_NODE_STRUCT));
        ASTNode* right_term = parse_term(parser);
        list_append(symbol->children, right_term, sizeof(struct AST_NODE_STRUCT));
    }
    
    return symbol;
}


ASTNode* parse_var(Parser* parser, Token* symbol_name_token, ASTNode* def_type) {
    ASTNode* symbol = init_ASTNode(AST_VAR, symbol_name_token->value);

    if (def_type != NULL)
        list_append(symbol->children, def_type, sizeof(struct AST_NODE_STRUCT));

    if (strcmp(parser->curr_token->value, ";") == 0) {
        list_append(symbol->children, init_ASTNode(AST_NULL, NULL), sizeof(struct AST_NODE_STRUCT));
    }
    else {
        parser_eat(parser, TOKEN_EQUAL);
        ASTNode* expr = init_ASTNode(AST_EXPR, NULL);
        list_append(symbol->children, expr, sizeof(struct AST_NODE_STRUCT));
        list_append(expr->children, parse_expr(parser), sizeof(struct AST_NODE_STRUCT));
    }
    parser_eat(parser, TOKEN_EOL);
    parser->curr_line++; //After reading EOL then new line
    
    /*bool dec = is_symbol_declared(parser, symbol);
    printf("%d\n", dec);
    if (!is_symbol_declared(parser, symbol) && def_type != NULL) {
        printf("bet: Cannot find symbol '%s' at line %d\n", symbol->name, parser->curr_line);
        printf("Exited with code 1\n");
        exit(1);
    }*/



    return symbol;
}


ASTNode* parse_block(Parser* parser) {
    ASTNode* block = init_ASTNode(AST_BLOCK, NULL);
    parser_eat(parser, TOKEN_LBRACE);

    bool block_done = false;
    while (block_done != true) {
        ASTNode* child = NULL;
        child = parse_id(parser);

        //printf("Child %s", astnode_to_string(child));
        list_append(block->children, child, sizeof(struct AST_NODE_STRUCT));
    
        if (parser->curr_token->type == TOKEN_RBRACE) {
            parser_eat(parser, TOKEN_RBRACE);
            parser->curr_func = NULL;
            block_done = true;
        } 
    }

    return block;
}

ASTNode* parse_return_st(Parser* parser) {
    ASTNode* return_symbol = init_ASTNode(AST_RETURN, NULL);

    switch(parser->curr_token->type) {
        case TOKEN_INT:
            list_append(return_symbol->children, parse_expr(parser), sizeof(struct AST_NODE_STRUCT));
            parser_eat(parser, TOKEN_EOL);
            return return_symbol;
            break;

        case TOKEN_ID:
            list_append(return_symbol->children, parse_id(parser), sizeof(struct AST_NODE_STRUCT));
            //printf("%s\n", token_to_string(parser->curr_token));
            //parser_eat(parser, TOKEN_EOL);
            return return_symbol;
            break;

        default:
            return NULL;
            break;
    }
    return NULL;
}

ASTNode* parse_keyword(Parser* parser) {
    
    if (strcmp(parser->curr_token->value, "int") == 0) {
        ASTNode* def_type = init_ASTNode(AST_DEC_TYPE, parser->curr_token->value); 
        parser_eat(parser, TOKEN_ID);
        //printf("%s\n", token_to_string(parser->curr_token));
        //printf("%s\n", astnode_to_string(def_type));
        return def_type;
    }
    if (strcmp(parser->curr_token->value, "return") == 0) {
        parser_eat(parser, TOKEN_ID);
        //if (parser->curr_symbol->children->arr[0])
        if (!is_prim_type(parser->curr_token->value)) {
            return parse_return_st(parser);
        }
        return parse_return_st(parser);
    }

    if (strcmp(parser->curr_token->value, "print") == 0)
        return NULL;

    return NULL;
}

/*
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
}*/


ASTNode* parse_func_params(Parser* parser) {
    ASTNode* params = init_ASTNode(AST_PARAMS, NULL);
    parser_eat(parser, TOKEN_LPARAN);
    
    if (parser->curr_token->type == TOKEN_RPARAN) {
        parser_eat(parser, TOKEN_RPARAN);
        return params;
    }

    bool param_done = false;
    while (param_done != true) {
        ASTNode* dec_type = init_ASTNode(AST_DEC_TYPE, parser->curr_token->value);
        parser_eat(parser, TOKEN_ID);
        ASTNode* param_var = init_ASTNode(AST_VAR, parser->curr_token->value);
        list_append(param_var->children, dec_type, sizeof(struct AST_NODE_STRUCT));
        parser_eat(parser, TOKEN_ID); 
        if (parser->curr_token->type == TOKEN_RPARAN)
            param_done = true;
        else
            parser_eat(parser, TOKEN_COMMA);

        list_append(params->children, param_var, sizeof(struct AST_NODE_STRUCT));
    }
    parser_eat(parser, TOKEN_RPARAN);

    return params;
}


ASTNode* parse_func(Parser* parser, Token* symbol_name_token, ASTNode* ret_type) {
    ASTNode* symbol = init_ASTNode(AST_FUNC, symbol_name_token->value);
    //printf("HEYYYYYYYYYYY %s\n", parser->curr_token->value);
    if (ret_type != NULL)
        list_append(symbol->children, ret_type, sizeof(struct AST_NODE_STRUCT));
    
    ASTNode* params = parse_func_params(parser);

    
    if (params->children->num_items == 0)
        list_append(symbol->children, NULL, sizeof(struct AST_NODE_STRUCT));
    else
        list_append(symbol->children, params, sizeof(struct AST_NODE_STRUCT));

    if (parser->curr_token->type != TOKEN_LBRACE) {
        parser_eat(parser, TOKEN_EOL);
        parser->curr_line++; //After reading EOL then new line
    }
    else {
        list_append(symbol->children, parse_block(parser), sizeof(struct AST_NODE_STRUCT));

    }

            /*
     * GET RETURN VAR AND CHECK IF THE VAR IS REFERENCED IN THE FUNC SYMBOL OR REFERENCED GLOBALLY
     * 
     * */
    /*
    parser->curr_func = symbol;


    ASTNode* return_val = get_return_val(symbol);

    if (is_symbol_declared(parser, return_val->children->arr[0])) {
        ((ASTNode*)return_val->children->arr[0])->children->arr[0] = NULL;
    }
    else {
        printf("bet: Cannot find symbol '%s' at line %d\n", ((ASTNode*)return_val->children->arr[0])->name, parser->curr_line);
        printf("Exited with code 1\n");
        exit(1);
    }

    parser->curr_func = NULL;*/

    return symbol;
}

/*
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

//bool is_correct_type (Parser* parser, )


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
}*/



ASTNode* parse_id(Parser* parser) {
    if (is_keyword_type(parser->curr_token->value)) {
        ASTNode* keyword = parse_keyword(parser);   

        if (keyword->type == AST_DEC_TYPE) {
            ASTNode* def_type = keyword;
            if (is_unique_symbol_name(parser->curr_token->value)) {
                Token* symbol_name = parser->curr_token;
                parser_eat(parser, TOKEN_ID);
                if (strcmp(parser->curr_token->value, "(") != 0) {
                    return parse_var(parser, symbol_name, def_type);
                }

                /*if (is_symbol_declared_global(parser, symbol_name->value)) {
                    return parse_func(parser, symbol_name, NULL);
                }*/

                ASTNode* ret_type = init_ASTNode(AST_RETURN_TYPE, def_type->name);
                ASTNode* func = parse_func(parser, symbol_name, ret_type); //FIND WAY TO CHECK IF IT IS GLOBAL DEF
                //printf("%s\n", astnode_to_string(func));
                return func;
            }     
        }
        return keyword;
    }

    if (!is_prim_type(parser->curr_token->value)) { // TO CHECK IF VAR IS PREV DECLARED, MIGHT HAVE ISSUES WHEN IMPLEMENT FUNCTIONS
        Token* symbol_name = parser->curr_token;
        parser_eat(parser, TOKEN_ID);
        
        return parse_var(parser, symbol_name, NULL);
       

    }
    return NULL;
}

ASTNode* parser_parse(Parser* parser) {
    switch (parser->curr_token->type) {
        case TOKEN_ID:
            return parse_id(parser);
            break;
        default:
            return NULL;
            break;
    };
}

void parser_parse_tokens(Parser* parser) {
    parser->root = init_ASTNode(AST_GLOBAL, NULL); //FILE NAME

    //printf("\n\n\n\t-----TOKENS-----\n\n");
    
    while (parser->curr_token->type != TOKEN_EOF) {
        ASTNode* child = parser_parse(parser);
        list_append(parser->root->children, child, sizeof(struct AST_NODE_STRUCT));
    }
}

int count_nodes(ASTNode* curr) { 
    static int count = 0;
    if (curr != NULL) {
        count++;
        for (size_t i = 0; i < curr->children->num_items; i++) {
            count_nodes(curr->children->arr[i]);
        }
    }
    return count;
}


void traverse_print_ast(ASTNode* node, List* flag, int depth, bool is_last) {
    if (node == NULL)
        return;

    for (int i = 1; i < depth; i++) {
        if (flag->arr[i] == (void*)true) {
            printf("|     ");
        }

        else
            printf("      ");
    }

    if (depth == 0)
        printf("%s\n", astnode_to_string(node));

    else if (is_last) {
        printf("+--- %s\n", astnode_to_string(node));
        flag->arr[depth] = false;
    }
    else {
        printf("+--- %s\n", astnode_to_string(node));
    }
    
    for (int i = 0; i < node->children->num_items; i++) {
        traverse_print_ast(node->children->arr[i], flag, depth + 1, (i == (node->children->num_items - 1)));
    }
    flag->arr[depth] = (void*)true;
}


void print_ast(Parser* parser) {
     
    List* flag = init_list(sizeof(bool));
    int count = count_nodes(parser->root);
    for (int i = 0; i < count; i++) {
        list_append(flag, (void*)true, sizeof(bool));
    }
    //printf("%s\n", astnode_to_string(((ASTNode*)((ASTNode*)((ASTNode*)((ASTNode*)((ASTNode*)((ASTNode*)parser->root->children->arr[0])->children->arr[1])->children->arr[0])->children->arr[2])->children->arr[2])->children->arr[0])->children->arr[0]));
    printf("\n\n\n\t-----AST-----\n\n");
    traverse_print_ast(parser->root, flag, 0, false);
}

