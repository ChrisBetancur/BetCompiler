#include "include/parser.h"
#include "include/token.h"
#include "include/ast_node.h"
#include "include/util.h"

//#include "include/ast_clean.h"

/*
 * Function: init_parser
 *
 * Creates parser to convert tokens from lexer to AST
 *
 * lexer: used to get tokens from src code to put into AST
 *
 * returns: parser
 */

Parser* init_parser(Lexer* lexer) {
    Parser* parser = malloc(sizeof(struct PARSER_STRUCT));

    parser->root = NULL;
    parser->tokens = init_list(sizeof(struct TOKEN_STRUCT));
    parser->lexer = lexer;
    parser->curr_token = lexer_next_token(parser->lexer);
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

void parser_eat(Parser* parser, int type) {

    if (parser->curr_token->type != type) {
        /*printf("Parser: '%s' unexpected token; Expected: '%s'::%d\n",
                parser->curr_token->value, token_type_to_string(type), parser->curr_token->line_num);
        printf("Exited with code 1\n");
        exit(1);*/
        parser_error_handler(UNEXPECTED_TOKEN, parser->curr_token->value, type, parser->curr_token->line_num);
    }
    parser->curr_token = lexer_next_token(parser->lexer);
    list_append(parser->tokens, parser->curr_token, sizeof(struct TOKEN_STRUCT));
}


/*
 * Function: parse_literal
 *
 * Parses current token as literal
 *
 * returns: AST node containing literal
 */

ASTNode* parse_string(Parser* parser) {
    ASTNode* symbol = init_ASTNode(parser->curr_token->value, AST_STRING);
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

ASTNode* parse_factor(Parser* parser) {
    ASTNode* symbol;
    if (parser->curr_token->type == TOKEN_INT) {
        symbol = init_ASTNode(parser->curr_token->value, AST_INT);
        parser_eat(parser, TOKEN_INT);
        return symbol;
    }

    if (parser->curr_token->type == TOKEN_ID) {
        if (is_keyword_type(parser->curr_token->value)) {
            parser_error_handler(INVALID_ASSIGNMENT, parser->curr_token->value, 0, parser->curr_token->line_num);
        }

        symbol = init_ASTNode(parser->curr_token->value, AST_VAR); // MUST FIND WAY TO CLEAN SYMBOL THAT IS BEING ASSIGNED TO VAR IF SYMBOL DEFINED IN FUNCTION
                                                                   // might have to fix at runtime while translating to assembly
        //clean_symbol(parser, symbol, parser->root, parser->curr_token->line_num);

        parser_eat(parser, TOKEN_ID);
        return symbol;
    }


    if (parser->curr_token->type == TOKEN_ID && !is_keyword_type(parser->curr_token->value)) {
        symbol = init_ASTNode(parser->curr_token->value, AST_VAR); // Figure out a way to check if previously defined
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

/*
 * Function: parse_expr
 *
 * Parses expression, parses terms or factors that are added or subtracted together
 *
 * parser: parser struct to get the tokens from
 *
 * returns: expression AST node
 */

ASTNode* parse_expr(Parser* parser) {
    ASTNode* symbol = parse_term(parser);
    while (is_addsub_op(parser->curr_token->value)) {
        ASTNode* prev_symbol = symbol;
        symbol = init_ASTNode(NULL, AST_BINARY_OP);

        if (parser->curr_token->type == TOKEN_ADD) {
            list_append(symbol->children, init_ASTNode(parser->curr_token->value, AST_OPERATOR), sizeof(struct AST_NODE_STRUCT));
            parser_eat(parser, TOKEN_ADD);
        }
        if (parser->curr_token->type == TOKEN_SUB) {
            list_append(symbol->children, init_ASTNode(parser->curr_token->value, AST_OPERATOR), sizeof(struct AST_NODE_STRUCT));
            parser_eat(parser, TOKEN_SUB);
        }

        list_append(symbol->children, prev_symbol, sizeof(struct AST_NODE_STRUCT));
        ASTNode* right_term = parse_term(parser);
        list_append(symbol->children, right_term, sizeof(struct AST_NODE_STRUCT));
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

void parse_int_var(Parser* parser, ASTNode* symbol) {
    ASTNode* expr = init_ASTNode(NULL, AST_EXPR);
    list_append(symbol->children, expr, sizeof(struct AST_NODE_STRUCT));
    list_append(expr->children, parse_expr(parser), sizeof(struct AST_NODE_STRUCT));
}

/*
 * Function: parse_bool_var
 *
 * Parses bool by appending the assignment to the symbol
 *
 * parser: used to parse expression assignment
 * symbol: appending expression to symbol
 */

void parse_bool_var(Parser* parser, ASTNode* symbol) { // not able to parse boolean expression (|| && ==)
    ASTNode* bool_val = init_ASTNode(parser->curr_token->value, AST_BOOL);
    list_append(symbol->children, bool_val, sizeof(struct AST_NODE_STRUCT));
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

void parse_string_var(Parser* parser, ASTNode* symbol) {
    list_append(symbol->children, parse_string(parser), sizeof(struct AST_NODE_STRUCT));
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

            if (is_expression(parser->curr_token->value)) {
                parse_int_var(parser, symbol);
            }
            else if (is_literal(parser->curr_token->value)) {
                parse_string_var(parser, symbol);
            }
            else if (is_bool_val(parser->curr_token->value))
                parse_bool_var(parser, symbol);

            parser_eat(parser, TOKEN_EOL);
            return symbol;
        }


        if (parser->curr_token->type == TOKEN_GT) {
            parser_eat(parser, TOKEN_GT);
            Token* symbol_name_token = parser->curr_token;
            parser_eat(parser, TOKEN_ID);
            ASTNode* call = parse_func_call(parser, symbol_name_token);
            list_append(symbol->children, call, sizeof(struct AST_NODE_STRUCT));

            return symbol;
        }
        else if (strcmp(def_type->name, "int") == 0 ) {
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

ASTNode* parse_block(Parser* parser) {
    ASTNode* block = init_ASTNode(NULL, AST_BLOCK);
    parser_eat(parser, TOKEN_LBRACE);

    if (parser->curr_token->type == TOKEN_RBRACE) {
        parser_eat(parser, TOKEN_RBRACE);
        return block;
    }

    bool block_done = false;
    while (block_done != true) {
        ASTNode* child = NULL;
        child = parse_id(parser);
        list_append(block->children, child, sizeof(struct AST_NODE_STRUCT));

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

ASTNode* parse_return_st(Parser* parser) {
    ASTNode* return_symbol = init_ASTNode(NULL, AST_RETURN);

    if (parser->curr_token->type == TOKEN_EOL) {
        parser_eat(parser, TOKEN_EOL);
        return return_symbol;
    }
    if (is_keyword_type(parser->curr_token->value)) {
            if (is_bool_val(parser->curr_token->value)) {
                parse_bool_var(parser, return_symbol);
                parser_eat(parser, TOKEN_EOL);
                return return_symbol;
            }
            /*printf("Parser: cannot return keyword '%s'::%d\n", parser->curr_token->value, parser->curr_token->line_num);
            printf("Exited with code 1\n");
            exit(1);*/
            parser_error_handler(INVALID_RETURN, parser->curr_token->value, 0, parser->curr_token->line_num);
    }

    list_append(return_symbol->children, parse_expr(parser), sizeof(struct AST_NODE_STRUCT));
    parser_eat(parser, TOKEN_EOL);
    return return_symbol;

    //return NULL;
}

ASTNode* parse_built_in(Parser* parser) {
    if (strcmp(parser->curr_token->value, "print") == 0) { // puts should only have one param, might remove this in the future
        ASTNode* symbol = init_ASTNode(parser->curr_token->value, AST_BUILT_IN);
        parser_eat(parser, TOKEN_ID);
        list_append(symbol->children, parse_func_call_params(parser), sizeof(struct AST_NODE_STRUCT));
        //parser_eat(parser, TOKEN_EOL);
        return symbol;
    }

    if (strcmp(parser->curr_token->value, "puts") == 0) {
        ASTNode* symbol = init_ASTNode(parser->curr_token->value, AST_BUILT_IN);
        parser_eat(parser, TOKEN_ID);
        list_append(symbol->children, parse_func_call_params(parser), sizeof(struct AST_NODE_STRUCT));
        //parser_eat(parser, TOKEN_EOL);
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

/*
 * Function: parse_func_params
 *
 * Parse params of a function
 *
 * parser: used to parse func params
 *
 * returns: Function params symbol
 */

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

ASTNode* parse_func_call_params(Parser* parser) { // USES FUNC DEF (GET SYMBOL IN SCOPE) TO CHECK IF NUMBER OF PARAMS AND TYPE MATCHES, WILL CHECK THAT WHEN PUT INTO STACK AND CONVERTING TO ASSEMBLY
    ASTNode* params = init_ASTNode(NULL, AST_PARAMS);

    parser_eat(parser, TOKEN_LPARAN);

    if (parser->curr_token->type == TOKEN_RPARAN) {
        parser_eat(parser, TOKEN_RPARAN);
        parser_eat(parser, TOKEN_EOL);
        return NULL;
    }

    while (parser->curr_token->type != TOKEN_RPARAN) {
        if (parser->curr_token->type == TOKEN_ID && !is_keyword_type(parser->curr_token->value)) {
            ASTNode* symbol = init_ASTNode(parser->curr_token->value, AST_VAR);
            list_append(params->children, symbol, sizeof(struct AST_NODE_STRUCT));
            parser_eat(parser, TOKEN_ID);
        }

        else if(is_expression(parser->curr_token->value) || strcmp(parser->curr_token->value, "(") == 0) {
            list_append(params->children, parse_expr(parser), sizeof(struct AST_NODE_STRUCT));
        }

        /*else if(is_literal(parser->curr_token->value)) {
            list_append(params->children, parse_literal(parser), sizeof(struct AST_NODE_STRUCT));
        }*/
        else if(parser->curr_token->type == TOKEN_STRING) {
            list_append(params->children, parse_string(parser), sizeof(struct AST_NODE_STRUCT));
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


ASTNode* parse_func_call(Parser* parser, Token* symbol_name_token) {
    ASTNode* symbol = init_ASTNode(symbol_name_token->value, AST_CALL);

    ASTNode* params = parse_func_call_params(parser);

    if (params->children->num_items != 0)
        list_append(symbol->children, params, sizeof(struct AST_NODE_STRUCT));

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

ASTNode* parse_id(Parser* parser) {
    if (is_built_in(parser->curr_token->value)) {
        return parse_built_in(parser);
    }
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

/*
 * Function: parser_parse
 *
 * Parses id
 *
 * parser: parser use to parse id
 *
 * returns: parse_id
 */

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

/*
 * Function: parser_parse_tokens
 *
 * Parses all tokens processed by lexer until it reaches the end of the file
 *
 * parser: parser_used to parse tokens
 */

void parser_parse_tokens(Parser* parser) {
    parser->root = init_ASTNode("global", AST_GLOBAL);

    //printf("\n\n\n\t-----TOKENS-----\n\n");

    while (parser->curr_token->type != TOKEN_EOF) {
        ASTNode* child = parser_parse(parser);
        list_append(parser->root->children, child, sizeof(struct AST_NODE_STRUCT));
    }
}

/*
 * Function: count_nodes
 *
 * Counts all nodes in tree starting from the current point
 *
 * returns: num of nodes
 */

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

void traverse_print_ast(ASTNode* node, List* flag, int depth, bool is_last, FILE* file) {
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

void print_ast_at_node(ASTNode* node) {
    List* flag = init_list(sizeof(bool));
    int count = count_nodes(node);
    for (int i = 0; i < count; i++) {
        list_append(flag, (void*)true, sizeof(bool));
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

void print_ast(Parser* parser) {
    List* flag = init_list(sizeof(bool));
    int count = count_nodes(parser->root);
    for (int i = 0; i < count; i++) {
        list_append(flag, (void*)true, sizeof(bool));
    }
    printf("\n\n\n\t-----AST-----\n\n");
    traverse_print_ast(parser->root, flag, 0, false, stdout);
}

void ast_to_file(Parser* parser, FILE* file) {
    List* flag = init_list(sizeof(bool));
    int count = count_nodes(parser->root);
    for (int i = 0; i < count; i++) {
        list_append(flag, (void*)true, sizeof(bool));
    }
    fprintf(file, "\n");
    traverse_print_ast(parser->root, flag, 0, false, file);

}

void tokens_to_file(Parser* parser, FILE* file) {
    fprintf(file, "\n");
    for (int i = 0; i < parser->tokens->num_items; i++) {
        fprintf(file, "%s\n", token_to_string(parser->tokens->arr[i]));
    }
}

