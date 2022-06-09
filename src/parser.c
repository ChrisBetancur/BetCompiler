#include "include/parser.h"
#include "include/token.h"
#include "include/ast_node.h"

Parser* init_parser(Lexer* lexer) {
    Parser* parser = malloc(sizeof(struct PARSER_STRUCT));

    parser->root = NULL;
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

bool is_keyword_type(char* name) {
    if (strcmp(name, "return") == 0)
        return true;

    if (strcmp(name, "print") == 0)
        return true;

    return false;
}

bool is_unique_symbol_name(char* name) { //future must implement to check if the symbol has already been defined in the scope
    if (is_prim_type(name))
        return false;
    else if (is_keyword_type(name))
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
        /*if (symbol == NULL) 
            symbol = init_ASTNode(AST_BINARY_OP, NULL);*/

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
    return symbol;
}


ASTNode* parse_block(Parser* parser) {
    ASTNode* block = init_ASTNode(AST_BLOCK, NULL);
    parser_eat(parser, TOKEN_LBRACE);

    bool block_done = false;
    while (block_done != true) {
        ASTNode* child = parser_parse(parser);
        list_append(block->children, child, sizeof(struct AST_NODE_STRUCT));
        if (parser->curr_token->type == TOKEN_RBRACE) {
            parser_eat(parser, TOKEN_RBRACE);
            block_done = true;
        }
    }

    return block;
}


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
    else
        list_append(symbol->children, parse_block(parser), sizeof(struct AST_NODE_STRUCT));

    //printf("%s\n", astnode_to_string(((ASTNode*)symbol->children->arr[1])->children->arr[1]));
    return symbol;
}


bool is_symbol_declared_global(Parser* parser, char* symbol_name) {
    for (int i = 0; i < parser->root->children->num_items; i++) {
        if (strcmp(((ASTNode*)parser->root->children->arr[i])->name, symbol_name) == 0)
            return true;
    }

    return false;
}

ASTNode* parse_id(Parser* parser) {
    if (!is_prim_type(parser->curr_token->value)) { // TO CHECK IF VAR IS PREV DECLARED, MIGHT HAVE ISSUES WHEN IMPLEMENT FUNCTIONS
        if (is_symbol_declared_global(parser, parser->curr_token->value)) {
            Token* symbol_name = parser->curr_token;
            parser_eat(parser, TOKEN_ID);
            return parse_var(parser, symbol_name, NULL);
        }
        else {
            printf("Cannot find symbol: %s\n", parser->curr_token->value);
            printf("Exited with code 1\n");
            exit(1);
        }
    }
    ASTNode* def_type = init_ASTNode(AST_DEC_TYPE, parser->curr_token->value);
    parser_eat(parser, TOKEN_ID);

    if (is_unique_symbol_name(parser->curr_token->value)) {
        Token* symbol_name = parser->curr_token;
        parser_eat(parser, TOKEN_ID);
            
        if (strcmp(parser->curr_token->value, "(") != 0) {
            return parse_var(parser, symbol_name, def_type);
        }

        if (is_symbol_declared_global(parser, symbol_name->value)) {
            printf("Hey\n");
            return parse_func(parser, symbol_name, NULL);
        }

        ASTNode* ret_type = init_ASTNode(AST_RETURN_TYPE, def_type->name);
        return parse_func(parser, symbol_name, ret_type); //FIND WAY TO CHECK IF IT IS GLOBAL DEF
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


void traverse_ast(ASTNode* node, List* flag, int depth, bool is_last) {
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
        traverse_ast(node->children->arr[i], flag, depth + 1, (i == (node->children->num_items - 1)));
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
    traverse_ast(parser->root, flag, 0, false);
}

