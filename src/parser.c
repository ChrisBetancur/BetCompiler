#include "include/parser.h"
#include "include/token.h"
#include "include/node.h"
#include "include/util.h"
#include <string.h>


void parser_eat(parser_t *parser, int type);

bool is_prim_type(char *name);

bool is_bool_val(char *name);

bool is_dec_type(char *name);

bool is_expression(char *name);

bool is_literal(char *name);

bool is_built_in(char *name);

bool is_keyword_type(char *name);

bool is_unique_symbol_name(char *name);

bool is_multdiv_op(char *name);

bool is_addsub_op(char *name);

node_t* parse_literal(parser_t *parser);

node_t* parse_factor(parser_t *parser);

node_t* parse_term(parser_t *parser);

node_t* parse_expr(parser_t *parser);

void parse_int_var(parser_t *parser, node_t *symbol);

void parse_string_var(parser_t *parser, node_t *symbol);

node_t* parse_var(parser_t *parser, token_t *symbol_name_token, node_t *def_type);

node_t* parse_block(parser_t *parser);

node_t* parse_return_st(parser_t* parser);

node_t* parse_built_in(parser_t* parser);

node_t* parse_keyword(parser_t* parser);

node_t* get_return_val(node_t* curr_symbol);

node_t* parse_func_params(parser_t* parser);

node_t* parse_func(parser_t* parser, token_t* symbol_name_token, node_t* def_type);

node_t* parse_func_call_params(parser_t* parser);

node_t* parse_func_call(parser_t* parser, token_t* symbol_name_token);

node_t* parse_id(parser_t* parser);

node_t* parser_parse(parser_t* parser);

int parser_count_nodes(node_t* node, int count);

void traverse_print_ast(node_t* node, list_t *flag, int depth, bool is_last, FILE* file);

void bfs_ast(node_t* root);




/*
 * Function: init_parser
 *
 * Creates parser to convert tokens from lexer to AST
 *
 * lexer: used to get tokens from src code to put into AST
 *
 * returns: parser
 */

parser_t* init_parser(lexer_t* lexer) {
    parser_t* parser = malloc(sizeof(struct PARSER_STRUCT));

    parser->root = NULL;
    parser->tokens = init_list(sizeof(struct TOKEN_STRUCT));
    parser->lexer = lexer;
    parser->curr_token = lexer_stream_token(parser->lexer);
    return parser;
}

/*
 * Function: parser_eat
 *
 * Gets next token in the src code and stores it in the parser
 *
 * parser: eat the current token and get the next token
 *
 * type: the type of token parser is supposed to eat
 */

void parser_eat(parser_t* parser, int type) {

    if (parser->curr_token->type != type) {
        parser_error_handler(UNEXPECTED_TOKEN, parser->curr_token->name, type, parser->curr_token->line_num);
    }
    parser->curr_token = lexer_stream_token(parser->lexer);
    list_append(parser->tokens, parser->curr_token);
}

/*
 * Function: parse_literal
 *
 * Parses current token as literal
 *
 * returns: AST node containing literal
 */

node_t* parse_string(parser_t* parser) {
    node_t* symbol = init_node(parser->curr_token->name, AST_STRING);
    parser_eat(parser, TOKEN_STRING);
    return symbol;
}

/*
 * Function: parse_factor
 *
 * Parses factor and if the curr_token is a parenthesis then parse expression
 *
 * parser: parser used to get the tokens from
 *
 * returns: factor AST node
 */

node_t* parse_factor(parser_t *parser) {
    node_t *symbol;
    if (parser->curr_token->type == TOKEN_INT) {
        symbol = init_node(parser->curr_token->name, AST_INT);
        parser_eat(parser, TOKEN_INT);
        return symbol;
    }

    if (parser->curr_token->type == TOKEN_ID) {
        if (is_keyword_type(parser->curr_token->name)) {
            parser_error_handler(INVALID_ASSIGNMENT, parser->curr_token->name, 0, parser->curr_token->line_num);
        }

        symbol = init_node(parser->curr_token->name, AST_VAR);
        parser_eat(parser, TOKEN_ID);
        return symbol;
    }


    if (parser->curr_token->type == TOKEN_ID && !is_keyword_type(parser->curr_token->name)) {
        symbol = init_node(parser->curr_token->name, AST_VAR);
        return symbol;
    }

    else if (parser->curr_token->type == TOKEN_LPARAN) {
        parser_eat(parser, TOKEN_LPARAN);
        symbol = parse_expr(parser);
        parser_eat(parser, TOKEN_RPARAN);
        return symbol;
    }

    parser_eat(parser, TOKEN_INT);
    return NULL;
}

/*
 * Function: parse_term
 *
 * Parses term, parses factors that are multiplying or dividing together first
 *
 * parser: parser struct used to get the tokens
 *
 * returns: term AST Node
 */

node_t* parse_term(parser_t *parser) {
    node_t *symbol = parse_factor(parser);
    while (is_multdiv_op(parser->curr_token->name)) {
        node_t *prev_symbol = symbol;

        symbol = init_node(NULL, AST_BINARY_OP);
        char* op = parser->curr_token->name;
        if (parser->curr_token->type == TOKEN_MULT) {
            parser_eat(parser, TOKEN_MULT);
        }

        else if (parser->curr_token->type == TOKEN_DIV) {
            parser_eat(parser, TOKEN_DIV);
        }
        list_append(symbol->children, init_node(op, AST_OPERATOR));
        list_append(symbol->children, prev_symbol);
        node_t *right = parse_factor(parser);
        list_append(symbol->children, right);
    }
    return symbol;
}

/*
 * Function: parse_expr
 *
 * Parses expression, parses terms or factors that are added or subtracted together
 *
 * parser: parser struct to get the tokens from
 *
 * returns: expression AST node
 */

node_t *parse_expr(parser_t *parser) {
    node_t *symbol = parse_term(parser);
    while (is_addsub_op(parser->curr_token->name)) {
        node_t *prev_symbol = symbol;
        symbol = init_node(NULL, AST_BINARY_OP);

        if (parser->curr_token->type == TOKEN_ADD) {
            list_append(symbol->children, init_node(parser->curr_token->name, AST_OPERATOR));
            parser_eat(parser, TOKEN_ADD);
        }
        if (parser->curr_token->type == TOKEN_SUB) {
            list_append(symbol->children, init_node(parser->curr_token->name, AST_OPERATOR));
            parser_eat(parser, TOKEN_SUB);
        }

        list_append(symbol->children, prev_symbol);
        node_t *right_term = parse_term(parser);
        list_append(symbol->children, right_term);
    }

    return symbol;
}

/*
 * Function: parse_int_var
 *
 * Parses int by appending the assignment to the symbol
 *
 * parser: used to parse expression assignment
 * symbol: appending expression to symbol
 */

void parse_int_var(parser_t *parser, node_t *symbol) {
    node_t *expr = init_node(NULL, AST_EXPR);
    list_append(symbol->children, expr);
    list_append(expr->children, parse_expr(parser));
}

/*
 * Function: parse_bool_var
 *
 * Parses bool by appending the assignment to the symbol
 *
 * parser: used to parse expression assignment
 * symbol: appending expression to symbol
 */

void parse_bool_var(parser_t *parser, node_t *symbol) { // not able to parse boolean expression (|| && ==)
    node_t *bool_val = init_node(parser->curr_token->name, AST_BOOL);
    list_append(symbol->children, bool_val);
    parser_eat(parser, TOKEN_ID);
}


/*
 * Function: parse_string_var
 *
 * Parses string by appending literal to symbol
 *
 * parser: used to parse literal
 * symbol: appending literal to symbol
 */

void parse_string_var(parser_t *parser, node_t *symbol) {
    list_append(symbol->children, parse_string(parser));
}

/*
 * Function: parse_var
 *
 * Parses var with or without assignment
 *
 * parser: used to parse var
 * symbol_name_token: name of var as a token
 * def_type: def type as a node
 *
 * returns: var as a symbol
 */

node_t* parse_var(parser_t *parser, token_t *symbol_name_token, node_t *def_type) {
    node_t* symbol = init_node(symbol_name_token->name, AST_VAR);

    if (def_type != NULL)
        list_append(symbol->children, def_type);

    if (strcmp(parser->curr_token->name, ";") == 0) {
        list_append(symbol->children, init_node(NULL, AST_NULL));
    }
    else {
        parser_eat(parser, TOKEN_EQUAL);
        if (def_type == NULL) {

            if (is_expression(parser->curr_token->name)) {
                parse_int_var(parser, symbol);
            }
            else if (is_literal(parser->curr_token->name)) {
                parse_string_var(parser, symbol);
            }
            else if (is_bool_val(parser->curr_token->name))
                parse_bool_var(parser, symbol);

            parser_eat(parser, TOKEN_EOL);
            return symbol;
        }


        if (parser->curr_token->type == TOKEN_GT) {
            parser_eat(parser, TOKEN_GT);
            token_t *symbol_name_token = parser->curr_token;
            parser_eat(parser, TOKEN_ID);
            node_t *call = parse_func_call(parser, symbol_name_token);
            list_append(symbol->children, call);

            return symbol;
        }
        else if (strcmp(def_type->name, "i64") == 0 || strcmp(def_type->name, "i32") == 0) {
            parse_int_var(parser, symbol);
        }
        else if (strcmp(def_type->name, "string") == 0 ) {
            parse_string_var(parser, symbol);
        }

        else if (strcmp(def_type->name, "bool") == 0)
            parse_bool_var(parser, symbol);

    }

    parser_eat(parser, TOKEN_EOL);

    return symbol;
}

/*
 * Function: parse_block
 *
 * Parses block of code belonging to a function
 *
 * parser: used to parse block
 *
 * returns: block node
 */

node_t* parse_block(parser_t *parser) {
    node_t *block = init_node(NULL, AST_BLOCK);
    parser_eat(parser, TOKEN_LBRACE);

    if (parser->curr_token->type == TOKEN_RBRACE) {
        parser_eat(parser, TOKEN_RBRACE);
        return block;
    }

    bool block_done = false;
    while (block_done != true) {
        node_t *child = NULL;
        child = parse_id(parser);
        list_append(block->children, child);

        if (parser->curr_token->type == TOKEN_RBRACE) {
            parser_eat(parser, TOKEN_RBRACE);
            block_done = true;
        }
    }

    return block;
}

/*
 * Function: parse_return_st
 *
 * Parses return statement which includes the return value
 *
 * parser: used to parse the return statement
 *
 * returns: return symbol node
 */

node_t* parse_return_st(parser_t* parser) {
    node_t* return_symbol = init_node(NULL, AST_RETURN);

    if (parser->curr_token->type == TOKEN_EOL) {
        parser_eat(parser, TOKEN_EOL);
        return return_symbol;
    }
    if (is_keyword_type(parser->curr_token->name)) {
            if (is_bool_val(parser->curr_token->name)) {
                parse_bool_var(parser, return_symbol);
                parser_eat(parser, TOKEN_EOL);
                return return_symbol;
            }

            parser_error_handler(INVALID_RETURN, parser->curr_token->name, 0, parser->curr_token->line_num);
    }

    list_append(return_symbol->children, parse_expr(parser));
    parser_eat(parser, TOKEN_EOL);
    return return_symbol;
}

node_t* parse_built_in(parser_t *parser) {
    if (strcmp(parser->curr_token->name, "print") == 0) { // puts should only have one param, might remove this in the future
        node_t *symbol = init_node(parser->curr_token->name, AST_BUILT_IN);
        parser_eat(parser, TOKEN_ID);
        list_append(symbol->children, parse_func_call_params(parser));

        return symbol;
    }

    if (strcmp(parser->curr_token->name, "puts") == 0) {
        node_t *symbol = init_node(parser->curr_token->name, AST_BUILT_IN);
        parser_eat(parser, TOKEN_ID);
        list_append(symbol->children, parse_func_call_params(parser));

        return symbol;
    }

    return NULL;
}

/*
 * Function: parse_keyword
 *
 * Parses keyword
 *
 * parser: parser used the keyword
 *
 * returns: keyword symbol
 */

node_t* parse_keyword(parser_t *parser) {
    if (is_dec_type(parser->curr_token->name)) {
        node_t *def_type = init_node(parser->curr_token->name, AST_DEC_TYPE);
        parser_eat(parser, TOKEN_ID);
        return def_type;
    }


    if (strcmp(parser->curr_token->name, "return") == 0) {
        parser_eat(parser, TOKEN_ID);
        if (!is_prim_type(parser->curr_token->name)) {
            return parse_return_st(parser);
        }
        return parse_return_st(parser);
    }

    return NULL;
}

/*
 * Function: parse_func_params
 *
 * Parse params of a function
 *
 * parser: used to parse func params
 *
 * returns: Function params symbol
 */

node_t* parse_func_params(parser_t* parser) {
    node_t *params = init_node(NULL, AST_PARAMS);
    parser_eat(parser, TOKEN_LPARAN);

    if (parser->curr_token->type == TOKEN_RPARAN) {
        parser_eat(parser, TOKEN_RPARAN);
        return params;
    }

    bool param_done = false;
    while (param_done != true) {
        node_t *type = init_node(parser->curr_token->name, AST_DEC_TYPE);
        parser_eat(parser, TOKEN_ID);
        node_t *param_var = init_node(parser->curr_token->name, AST_VAR);
        parser_eat(parser, TOKEN_ID);
        if (parser->curr_token->type == TOKEN_RPARAN)
            param_done = true;
        else
            parser_eat(parser, TOKEN_COMMA);

        list_append(param_var->children, type);
        list_append(params->children, param_var);
    }
    parser_eat(parser, TOKEN_RPARAN);

    return params;
}
/*
 * Function: parse_func
 *
 * Parse function
 *
 * parser: used to parse function
 * symbol_name_token: token of the name of the symbol being parsed
 * ret_type: return type in ast node of the function
 *
 * returns: function returns as a ast symbol
 */

node_t* parse_func(parser_t *parser, token_t *symbol_name_token, node_t *ret_type) {
    node_t *symbol = init_node(symbol_name_token->name, AST_FUNC);

    if (ret_type != NULL)
        list_append(symbol->children, ret_type);

    node_t *params = parse_func_params(parser);

    if (params->children->num_items != 0)
        list_append(symbol->children, params);

    if (parser->curr_token->type != TOKEN_LBRACE) {
        parser_eat(parser, TOKEN_EOL);
    }
    else {
        list_append(symbol->children, parse_block(parser));
    }

    return symbol;
}

/*
 * Function: parse_func_call_params
 *
 * Parse function call params
 *
 * parser: used to parse function call params
 * funct_call: the function that was called
 *
 * returns: params symbol as node
 */

node_t* parse_func_call_params(parser_t *parser) { // USES FUNC DEF (GET SYMBOL IN SCOPE) TO CHECK IF NUMBER OF PARAMS AND TYPE MATCHES, WILL CHECK THAT WHEN PUT INTO STACK AND CONVERTING TO ASSEMBLY
    node_t* params = init_node(NULL, AST_PARAMS);

    parser_eat(parser, TOKEN_LPARAN);

    if (parser->curr_token->type == TOKEN_RPARAN) {
        parser_eat(parser, TOKEN_RPARAN);
        parser_eat(parser, TOKEN_EOL);
        return NULL;
    }

    while (parser->curr_token->type != TOKEN_RPARAN) {
        if (parser->curr_token->type == TOKEN_ID && !is_keyword_type(parser->curr_token->name)) {
            node_t *symbol = init_node(parser->curr_token->name, AST_VAR);
            list_append(params->children, symbol);
            parser_eat(parser, TOKEN_ID);
        }

        else if(is_expression(parser->curr_token->name) || strcmp(parser->curr_token->name, "(") == 0) {
            list_append(params->children, parse_expr(parser));
        }

        /*else if(is_literal(parser->curr_token->value)) {
            list_append(params->children, parse_literal(parser), sizeof(struct AST_NODE_STRUCT));
        }*/
        else if(parser->curr_token->type == TOKEN_STRING) {
            list_append(params->children, parse_string(parser));
        }


        if (parser->curr_token->type != TOKEN_RPARAN) {
            parser_eat(parser, TOKEN_COMMA);

        }

    }

    parser_eat(parser, TOKEN_RPARAN);

    parser_eat(parser, TOKEN_EOL);

    return params;
}

/*
 * Function: parse_func_call
 *
 * Parse function call
 *
 * parser: used to parse function call params
 * symbol_name_token: name of symbol that was called
 *
 * returns: func call
 */


node_t* parse_func_call(parser_t *parser, token_t *symbol_name_token) {
    node_t *symbol = init_node(symbol_name_token->name, AST_CALL);

    node_t *params = parse_func_call_params(parser);

    if (params->children->num_items != 0)
        list_append(symbol->children, params);

    return symbol;
}

/*
 * Function: parse_id
 *
 * Parse identifier token to node
 *
 * parser: used to parse id
 *
 * returns: parsed id
 */

node_t* parse_id(parser_t *parser) {
    if (is_built_in(parser->curr_token->name)) {
        return parse_built_in(parser);
    }
    if (is_keyword_type(parser->curr_token->name)) {
        node_t *keyword = parse_keyword(parser);

        if (keyword->type == AST_DEC_TYPE) {
            node_t *def_type = keyword;
            if (is_unique_symbol_name(parser->curr_token->name)) {
                token_t *symbol_name = parser->curr_token;
                parser_eat(parser, TOKEN_ID);
                if (strcmp(parser->curr_token->name, "(") != 0) {
                    return parse_var(parser, symbol_name, def_type);
                }

                node_t *ret_type = init_node(def_type->name, AST_RETURN_TYPE);
                node_t *func = parse_func(parser, symbol_name, ret_type);
                return func;
            }
        }
        return keyword;
    }

    if (!is_prim_type(parser->curr_token->name)) {
        token_t *symbol_name = parser->curr_token;
        parser_eat(parser, TOKEN_ID);

        if (strcmp(parser->curr_token->name, "(") == 0)
            return parse_func_call(parser, symbol_name);

        return parse_var(parser, symbol_name, NULL);


    }
    return NULL;
}

/*
 * Function: parser_parse
 *
 * Parses id
 *
 * parser: parser use to parse id
 *
 * returns: parse_id
 */

node_t* parser_parse(parser_t *parser) {
    switch (parser->curr_token->type) {
        case TOKEN_ID:
            return parse_id(parser);
            break;
        default:
            return NULL;
            break;
    };
}

/*
 * Function: parser_parse_tokens
 *
 * Parses all tokens processed by lexer until it reaches the end of the file
 *
 * parser: parser_used to parse tokens
 */

void parser_parse_tokens(parser_t *parser) {
    parser->root = init_node("global", AST_GLOBAL);

    //printf("\n\n\n\t-----TOKENS-----\n\n");

    while (parser->curr_token->type != TOKEN_EOF) {
        node_t *child = parser_parse(parser);
        list_append(parser->root->children, child);
    }
}

/*
 * Function: count_nodes
 *
 * Counts all nodes in tree starting from the current point
 *
 * returns: num of nodes
 */

int count_nodes(node_t *curr) {
    static int count = 0;
    if (curr != NULL) {
        count++;
        for (size_t i = 0; i < curr->children->num_items; i++) {
            count_nodes(curr->children->arr[i]);
        }
    }
    return count;
}

/*
 * Function: traverse_print_ast
 *
 * Prints the AST in user friendly fashion by traversing throught the tree
 *
 * node: the current node it is at
 * flag: the list of bool value to keep track of visited nodes
 * depth: the current level the abstract tree is on currently
 * is_last: keeps track if current node is on the last node
 */

void traverse_print_ast(node_t *node, list_t *flag, int depth, bool is_last, FILE* file) {
    if (node == NULL)
        return;

    for (int i = 1; i < depth; i++) {
        if (flag->arr[i] == (void*)true) {
            fprintf(file, "|     ");
        }

        else
            fprintf(file, "      ");
    }

    if (depth == 0)
        fprintf(file, "%s\n", astnode_to_string(node));

    else if (is_last) {
        fprintf(file, "+--- %s\n", astnode_to_string(node));
        flag->arr[depth] = false;
    }
    else {
        fprintf(file, "+--- %s\n", astnode_to_string(node));
    }

    for (int i = 0; i < node->children->num_items; i++) {
        traverse_print_ast(node->children->arr[i], flag, depth + 1, (i == (node->children->num_items - 1)), file);
    }
    flag->arr[depth] = (void*)true;
}

/*
 * Function: print_ast_at_node
 *
 * Prints AST at the specified node, traverses all nodes underneath
 *
 * node: root of the tree to be printed
 */

void print_ast_at_node(node_t *node) {
    list_t* flag = init_list(sizeof(bool));
    int count = count_nodes(node);
    for (int i = 0; i < count; i++) {
        list_append(flag, (void*)true);
    }
    printf("\n\n\n\t----------\n\n");
    traverse_print_ast(node, flag, 0, false, stdout);

    printf("\n\n");
}

/*
 * Function: print_ast
 *
 * Prints AST from root in parser
 *
 * parser: contains the root
 */

void print_ast(parser_t *parser) {
    list_t *flag = init_list(sizeof(bool));
    int count = count_nodes(parser->root);
    for (int i = 0; i < count; i++) {
        list_append(flag, (void*)true);
    }
    printf("\n\n\n\t-----AST-----\n\n");
    traverse_print_ast(parser->root, flag, 0, false, stdout);
}

void ast_to_file(parser_t *parser, FILE *file) {
    list_t *flag = init_list(sizeof(bool));
    int count = count_nodes(parser->root);
    for (int i = 0; i < count; i++) {
        list_append(flag, (void*)true);
    }
    fprintf(file, "\n");
    traverse_print_ast(parser->root, flag, 0, false, file);

}

void tokens_to_file(parser_t *parser, FILE *file) {
    fprintf(file, "\n");
    for (int i = 0; i < parser->tokens->num_items; i++) {
        fprintf(file, "%s\n", token_to_string(parser->tokens->arr[i]));
    }
}

