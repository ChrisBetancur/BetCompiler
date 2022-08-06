#include "include/parser.h"
#include <ctype.h>
#include "include/token.h"
#include "include/ast_node.h"
#include "include/ast_clean.h"

Parser* init_parser(Lexer* lexer) {
    Parser* parser = malloc(sizeof(struct PARSER_STRUCT));

    parser->root = NULL;
    parser->curr_func = NULL;
    parser->lexer = lexer;
    parser->curr_token = lexer_next_token(parser->lexer);
    return parser;
}

void parser_eat(Parser* parser, int type) {

    if (parser->curr_token->type != type) {
        printf("Parser: '%s' unexpected token; Expected: '%s'::%d\n", parser->curr_token->value, token_type_to_string(type), parser->curr_token->line_num);
        printf("Exited with code 1\n");
        exit(1);
    }
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

bool is_dec_type(char* name) {
    if (strcmp(name, "string") == 0)
        return true;

    return is_prim_type(name);
}


bool is_keyword_type(char* name) { // 
    if (is_dec_type(name))
        return true;
    
    if (strcmp(name, "return") == 0)
        return true;

    return false;
}

bool is_unique_symbol_name(char* name) {
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

ASTNode* parse_literal(Parser* parser) {
    ASTNode* symbol = init_ASTNode(parser->curr_token->value, AST_LITERAL);
    parser_eat(parser, TOKEN_STRING);
    return symbol;
}


ASTNode* parse_factor(Parser* parser) {
    ASTNode* symbol;
    if (parser->curr_token->type == TOKEN_INT) {
        symbol = init_ASTNode(parser->curr_token->value, AST_INT);
        parser_eat(parser, TOKEN_INT);
        return symbol;
    }
    
    else if (parser->curr_token->type == TOKEN_LPARAN) {
        symbol = init_ASTNode(NULL, AST_TERM);
        parser_eat(parser, TOKEN_LPARAN);
        list_append(symbol->children, parse_expr(parser), sizeof(struct AST_NODE_STRUCT));
        parser_eat(parser, TOKEN_RPARAN);
        return symbol;
    }

    parser_eat(parser, TOKEN_INT);
    return NULL;
}

ASTNode* parse_term(Parser* parser) {
    ASTNode* symbol = parse_factor(parser);
    while (is_multdiv_op(parser->curr_token->value)) {
        ASTNode* prev_symbol = symbol;
      
        symbol = init_ASTNode(NULL, AST_BINARY_OP);
        char* op = parser->curr_token->value;
        if (parser->curr_token->type == TOKEN_MULT) {
            parser_eat(parser, TOKEN_MULT);
        }
        
        else if (parser->curr_token->type == TOKEN_DIV) {
            parser_eat(parser, TOKEN_DIV);
        }
        list_append(symbol->children, init_ASTNode(op, AST_OPERATOR), sizeof(struct AST_NODE_STRUCT));
        list_append(symbol->children, prev_symbol, sizeof(struct AST_NODE_STRUCT));
        ASTNode* right = parse_factor(parser);
        list_append(symbol->children, right, sizeof(struct AST_NODE_STRUCT));
    }
    return symbol;
}

ASTNode* parse_expr(Parser* parser) { 
    ASTNode* symbol = parse_term(parser);
    while (is_addsub_op(parser->curr_token->value)) {
        ASTNode* prev_symbol = symbol;
        symbol = init_ASTNode(NULL, AST_BINARY_OP);

        if (parser->curr_token->type == TOKEN_ADD) {
            parser_eat(parser, TOKEN_ADD);
        }
        list_append(symbol->children, prev_symbol, sizeof(struct AST_NODE_STRUCT));
        ASTNode* right_term = parse_term(parser);
        list_append(symbol->children, right_term, sizeof(struct AST_NODE_STRUCT));
    }
    
    return symbol;
}


void parse_int_var(Parser* parser, ASTNode* symbol) {
    ASTNode* expr = init_ASTNode(NULL, AST_EXPR);
    list_append(symbol->children, expr, sizeof(struct AST_NODE_STRUCT));
    list_append(expr->children, parse_expr(parser), sizeof(struct AST_NODE_STRUCT));
}

void parse_string_var(Parser* parser, ASTNode* symbol) {
    list_append(symbol->children, parse_literal(parser), sizeof(struct AST_NODE_STRUCT));
}

ASTNode* parse_var(Parser* parser, Token* symbol_name_token, ASTNode* def_type) {
    ASTNode* symbol = init_ASTNode(symbol_name_token->value, AST_VAR);

    if (def_type != NULL)
        list_append(symbol->children, def_type, sizeof(struct AST_NODE_STRUCT));
   
    if (strcmp(parser->curr_token->value, ";") == 0) {
        list_append(symbol->children, init_ASTNode(NULL, AST_NULL), sizeof(struct AST_NODE_STRUCT));
    }
    else {
        
        parser_eat(parser, TOKEN_EQUAL);
        if (def_type == NULL) {
            clean_symbol(parser, symbol, parser->root, parser->curr_token->line_num);
            ASTNode* symbol_def = get_symbol_in_scope(parser->root, symbol);
            def_type = symbol_def->children->arr[0];
        }
        
           
        if (strcmp(def_type->name, "int") == 0) {
            parse_int_var(parser, symbol);
        }

        else if (strcmp(def_type->name, "string") == 0) {
            parse_string_var(parser, symbol);
        }
    }

    parser_eat(parser, TOKEN_EOL);


    return symbol;
}


ASTNode* parse_block(Parser* parser) {
    ASTNode* block = init_ASTNode(NULL, AST_BLOCK);
    parser_eat(parser, TOKEN_LBRACE);
    
    if (parser->curr_token->type == TOKEN_RBRACE) {
        parser_eat(parser, TOKEN_RBRACE);
        parser->curr_func = NULL;
        return block;
    } 

    bool block_done = false;
    while (block_done != true) {
        ASTNode* child = NULL;
        child = parse_id(parser);

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
    ASTNode* return_symbol = init_ASTNode(NULL, AST_RETURN);

    switch(parser->curr_token->type) {
        case TOKEN_INT:
            list_append(return_symbol->children, parse_expr(parser), sizeof(struct AST_NODE_STRUCT));
            parser_eat(parser, TOKEN_EOL);
            return return_symbol;
            break;

        case TOKEN_ID:
            list_append(return_symbol->children, parse_id(parser), sizeof(struct AST_NODE_STRUCT));
            return return_symbol;
            break;

        default:
            parser_eat(parser, TOKEN_EOL);
            return return_symbol;
            break;
    }
    return NULL;
}

ASTNode* parse_keyword(Parser* parser) {
   
    if (is_dec_type(parser->curr_token->value)) {
        ASTNode* def_type = init_ASTNode(parser->curr_token->value, AST_DEC_TYPE); 
        parser_eat(parser, TOKEN_ID);
        return def_type;
    }


    if (strcmp(parser->curr_token->value, "return") == 0) {
        parser_eat(parser, TOKEN_ID);
        if (!is_prim_type(parser->curr_token->value)) {
            return parse_return_st(parser);
        }
        return parse_return_st(parser);
    }

    return NULL;
}


ASTNode* parse_func_params(Parser* parser) {
    ASTNode* params = init_ASTNode(NULL, AST_PARAMS);
    parser_eat(parser, TOKEN_LPARAN);
    
    if (parser->curr_token->type == TOKEN_RPARAN) {
        parser_eat(parser, TOKEN_RPARAN);
        return params;
    }

    bool param_done = false;
    while (param_done != true) {
        ASTNode* type = init_ASTNode(parser->curr_token->value, AST_DEC_TYPE);
        parser_eat(parser, TOKEN_ID);
        ASTNode* param_var = init_ASTNode(parser->curr_token->value, AST_VAR);
        parser_eat(parser, TOKEN_ID); 
        if (parser->curr_token->type == TOKEN_RPARAN)
            param_done = true;
        else
            parser_eat(parser, TOKEN_COMMA);

        list_append(param_var->children, type, sizeof(struct AST_NODE_STRUCT));
        list_append(params->children, param_var, sizeof(struct AST_NODE_STRUCT));
    }
    parser_eat(parser, TOKEN_RPARAN);

    return params;
}


ASTNode* parse_func(Parser* parser, Token* symbol_name_token, ASTNode* ret_type) {
    ASTNode* symbol = init_ASTNode(symbol_name_token->value, AST_FUNC);


    if (ret_type != NULL)
        list_append(symbol->children, ret_type, sizeof(struct AST_NODE_STRUCT));

    ASTNode* params = parse_func_params(parser);

    if (params->children->num_items != 0)
        list_append(symbol->children, params, sizeof(struct AST_NODE_STRUCT));

    if (parser->curr_token->type != TOKEN_LBRACE) {
        parser_eat(parser, TOKEN_EOL);
    }
    else {
        list_append(symbol->children, parse_block(parser), sizeof(struct AST_NODE_STRUCT));

    }
    
    clean_func(parser, symbol, parser->root, parser->curr_token->line_num);
    return symbol;
}

ASTNode* parse_func_call_params(Parser* parser, ASTNode* func_call) { //NOT DONE
    ASTNode* params = init_ASTNode(NULL, AST_PARAMS);
    parser_eat(parser, TOKEN_LPARAN);

    if (parser->curr_token->type == TOKEN_RPARAN) {
        parser_eat(parser, TOKEN_RPARAN);
        parser_eat(parser, TOKEN_EOL);
        return NULL;
    }

    ASTNode* func_def = get_symbol_in_scope(parser->root, func_call);

    for (int i = 0; i < ((ASTNode*) func_def->children->arr[1])->children->num_items; i++) {
        ASTNode* curr_param = ((ASTNode*) func_def->children->arr[1])->children->arr[i];
        
        if (strcmp(((ASTNode*) curr_param->children->arr[0])->name, "int") == 0 && parser->curr_token->type == TOKEN_INT) {
            list_append(params->children, parse_expr(parser), sizeof(struct AST_NODE_STRUCT));
        }

        else if (strcmp(((ASTNode*) curr_param->children->arr[0])->name, "string") == 0) {
            if (parser->curr_token->type == TOKEN_STRING) {
                list_append(params->children, parse_literal(parser), sizeof(struct AST_NODE_STRUCT));
            }
        }
        else {
            printf("Parser: Value '%s' passed as param for function '%s'; Expecting '%s'::'%d'\n",
                    parser->curr_token->value, func_def->name, ((ASTNode*) curr_param->children->arr[0])->name, parser->curr_token->line_num);
            printf("Exited with code 1\n");
            exit(1);
        }

        printf("%s\n", astnode_to_string(curr_param));
        if (parser->curr_token->type == TOKEN_RPARAN) {
            if (i < ((ASTNode*) func_def->children->arr[1])->children->num_items - 1) {
                printf("Parser: Function call '%s' not enough arguments; Expecting %zu::%d\n",
                func_def->name, ((ASTNode*) func_def->children->arr[1])->children->num_items, parser->curr_token->line_num);
                printf("Exited with code 1\n");
                exit(1);
            }
            parser_eat(parser, TOKEN_RPARAN);
            break;
        }
        parser_eat(parser, TOKEN_COMMA);
    }

    if (parser->curr_token->type != TOKEN_EOL) {
        printf("Parser: Function call '%s' too much arguments; Expecting %zu::%d\n",
                func_def->name, ((ASTNode*) func_def->children->arr[1])->children->num_items, parser->curr_token->line_num);
        printf("Exited with code 1\n");
        exit(1);
 
    }
    //parser_eat(parser, TOKEN_EOL);

    return params;
}

ASTNode* parse_func_call(Parser* parser, Token* symbol_name_token) {
    ASTNode* symbol = init_ASTNode(symbol_name_token->value, AST_CALL);

    ASTNode* params = parse_func_call_params(parser, symbol);

    
    if (params->children->num_items != 0)
        list_append(symbol->children, params, sizeof(struct AST_NODE_STRUCT));

    parser_eat(parser, TOKEN_EOL);
 
    return symbol;
}


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

                ASTNode* ret_type = init_ASTNode(def_type->name, AST_RETURN_TYPE);
                ASTNode* func = parse_func(parser, symbol_name, ret_type);
                return func;
            }     
        }
        return keyword;
    }

    if (!is_prim_type(parser->curr_token->value)) {
        Token* symbol_name = parser->curr_token;
        parser_eat(parser, TOKEN_ID);
     
        if (strcmp(parser->curr_token->value, "(") == 0)
            return parse_func_call(parser, symbol_name);

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
    parser->root = init_ASTNode(NULL, AST_GLOBAL);

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


void print_ast_at_node(ASTNode* node) {
    List* flag = init_list(sizeof(bool));
    int count = count_nodes(node);
    for (int i = 0; i < count; i++) {
        list_append(flag, (void*)true, sizeof(bool));
    }
    printf("\n\n\n\t----------\n\n");
    traverse_print_ast(node, flag, 0, false);

    printf("\n\n");
}

void print_ast(Parser* parser) {
     
    List* flag = init_list(sizeof(bool));
    int count = count_nodes(parser->root);
    for (int i = 0; i < count; i++) {
        list_append(flag, (void*)true, sizeof(bool));
    }
    printf("\n\n\n\t-----AST-----\n\n");
    traverse_print_ast(parser->root, flag, 0, false);
}

