#include "include/parser.h"
#include "include/token.h"
#include "include/stack.h"
#include "include/ast_node.h"

Parser* init_parser(Lexer* lexer, int num_tokens) {
    Parser* parser = malloc(sizeof(struct PARSER_STRUCT));

    parser->root = NULL;
    parser->lexer = lexer;
    parser->num_tokens = num_tokens;
    parser->curr_token = lexer_next_token(parser->lexer);

    return parser;
}

void parser_eat(Parser* parser, int type) {
    if (parser->curr_token->type != type) {
        printf("Parser: %s unexpected token; Expected %s token", parser->curr_token->value, token_type_to_string(type));
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
    //printf("HEYYYYY %s\n", parser->curr_token->value);
    if (parser->curr_token->type == TOKEN_INT) {
        symbol = init_ASTNode(AST_INT, parser->curr_token->value);
        parser_eat(parser, TOKEN_INT);
        //printf("Factor %s\n", astnode_to_string(symbol));
        return symbol;
    }
    
    else if (parser->curr_token->type == TOKEN_LPARAN) {
        symbol = init_ASTNode(AST_TERM, NULL);
        parser_eat(parser, TOKEN_LPARAN);
        list_append(symbol->children, parse_expr(parser), sizeof(struct AST_NODE_STRUCT));
        return symbol;
    }
    return NULL;
}

ASTNode* parse_term(Parser* parser) {
    ASTNode* symbol = parse_factor(parser);
    
    while (is_multdiv_op(parser->curr_token->value)) {
        ASTNode* prev_symbol = symbol;
        //printf("Prev %s\n", astnode_to_string(prev_symbol));
        symbol = init_ASTNode(AST_BINARY_OP, NULL);
        char* op = parser->curr_token->value;
        //printf("Hello\n");
        if (parser->curr_token->type == TOKEN_MULT) {
            parser_eat(parser, TOKEN_MULT);
        }
        
        else if (parser->curr_token->type == TOKEN_DIV) {
            parser_eat(parser, TOKEN_DIV);
        }
        list_append(symbol->children, init_ASTNode(AST_OPERATOR, op), sizeof(struct AST_NODE_STRUCT));
        list_append(symbol->children, prev_symbol, sizeof(struct AST_NODE_STRUCT));
        ASTNode* right = parse_factor(parser);
        /*
        printf("Right %s\n", astnode_to_string(right));
        printf("Symbol %s\n", astnode_to_string(symbol));*/
        list_append(symbol->children, right, sizeof(struct AST_NODE_STRUCT));
        //printf("Symbol child %s\n", astnode_to_string(symbol->children->arr[2]));
 
    }

    //printf("%s\n", astnode_to_string(symbol));
    //printf("%s\n", astnode_type_to_string(symbol->type));
    //printf("%s\n", astnode_to_string(symbol->children->arr[0]));

    
    /* 
    if (symbol->children->num_items != 0) {
    printf("Term %s\n", astnode_to_string(symbol));
    printf("Term %s\n", astnode_to_string(symbol->children->arr[1])); 
    }*/

    return symbol;
}

ASTNode* parse_expr(Parser* parser) { 
    //ASTNode* symbol = NULL;

    ASTNode* symbol = parse_term(parser);
   
    //printf("Hello %s\n", astnode_to_string(term));
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

    /* 
    printf("Start------\n");

    printf("%s\n", astnode_to_string(symbol));

    //printf("%d\n", symbol->children->num_items);

    printf("Left %s\n", astnode_to_string(((ASTNode*)((ASTNode*)symbol->children->arr[1])->children->arr[2])->children->arr[1]));
    
    printf("Right %s\n", astnode_to_string(((ASTNode*)((ASTNode*)symbol->children->arr[1])->children->arr[2])->children->arr[2]));

    printf("Done\n\n");*/
    
    return symbol;
}
/*
ASTNode* parse_assignment(Parser* parser, int def_type) {
    ASTNode* prev_val = init_ASTNode(def_type, parser->curr_token->value);

    while (strcmp(parser->curr_token->value, ";") != 0) {
        
    }
}*/

ASTNode* parse_var(Parser* parser, ASTNode* def_type) {
    ASTNode* symbol = init_ASTNode(AST_VAR, parser->curr_token->value);
    //printf("%s\n", token_to_string(parser->curr_token));
    list_append(symbol->children, def_type, sizeof(struct AST_NODE_STRUCT));
    //printf("hello\n");
    //printf("%s\n", astnode_to_string(symbol->children->arr[0]));
    parser_eat(parser, TOKEN_ID);    
    if (strcmp(parser->curr_token->value, ";") == 0) {
        list_append(symbol->children, init_ASTNode(AST_NULL, NULL), sizeof(struct AST_NODE_STRUCT));
    }
    else {
        parser_eat(parser, TOKEN_EQUAL);
        ASTNode* expr = init_ASTNode(AST_EXPR, NULL);
        list_append(symbol->children, expr, sizeof(struct AST_NODE_STRUCT));
        list_append(expr->children, parse_expr(parser), sizeof(struct AST_NODE_STRUCT));
    }
    //printf("%s\n", astnode_to_string(((ASTNode*)symbol->children->arr[1])->children->arr[1]->children));
    return symbol;
}

/*ASTNode* parse_func_params(Parser* parser, ASTNode* func_def) {
    while (strcmp(parser->curr_token->value, ")") != 0) {
        
    }
}


ASTNode* parse_func_def(Parser* parser, ASTNode* curr, Token* symbol_name) {
    if (strcmp(parser->curr_token->value, "(") == 0) {
        list_append(curr->children, init_ASTNode(AST_FUNC, symbol_name->value), sizeof(struct AST_NODE_STRUCT));
    }
    return NULL;
}*/

ASTNode* parse_id(Parser* parser) {
    if (is_prim_type(parser->curr_token->value)) {
        ASTNode* def_type = init_ASTNode(AST_DEF_TYPE, parser->curr_token->value);
        parser_eat(parser, TOKEN_ID);

        if (is_unique_symbol_name(parser->curr_token->value)) {
            return parse_var(parser, def_type);
            /*Token* symbol_name = parser->curr_token;
            parser_eat(parser);
            if (strcmp(parser->curr_token->value, "(") != 0)
                parse_var(parser, def_type, symbol_name);
            parse_func_def(parser, def_type, symbol_name);*/
        }

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
    Stack* stack = init_stack(sizeof(struct AST_NODE_STRUCT));
    parser->root = init_ASTNode(AST_GLOBAL, NULL); //FILE NAME
    
    //printf("%d\n", parser->curr_token->type == NULL);
    ASTNode* child = parser_parse(parser);

    list_append(parser->root->children, child, sizeof(struct AST_NODE_STRUCT));

    //printf(parser->root->children[0]);
    //printf("%s\n", astnode_to_string(parser->root->children->arr[0]->children->arr[0]));
}

void print_tabs(int tabs) {
    for (int i = 0; i < tabs; i++) {
        printf("\t");
    }
}

int parser_count_nodes(ASTNode* node, int count) {
    if (count == 9)
        exit(0);
    if (node != NULL) {
        count++;
        printf("%s\n", astnode_to_string(node));
        for (int i = 0; node->children->num_items; i++) {
            parser_count_nodes(node->children->arr[i], count);
        }
    }
}

void bfs_ast(ASTNode* root) {
    List* visited = init_list(sizeof(struct AST_NODE_STRUCT));
    Stack* stack = init_stack(sizeof(struct AST_NODE_STRUCT));

    stack_push(stack, root, sizeof(struct AST_NODE_STRUCT));
    list_append(visited, root, sizeof(struct AST_NODE_STRUCT));
    int counter = 0;
    while (!stack_is_empty(stack)) {
        ASTNode* curr = stack_pop_first(stack);
        print_tabs(counter);
        printf("%s", astnode_to_string(curr));
        /*
        if (curr->children->num_items == 0)
            counter = pivot;
        if (curr->children->num_items > 0) {
            pivot = counter;
            counter++;
        }*/

        for (int i = 0; i < curr->children->num_items; i++) {
            //printf("Child %s", astnode_to_string(curr->children->arr[i]));
            stack_push(stack, curr->children->arr[i], sizeof(struct AST_NODE_STRUCT));
            list_append(visited, curr->children->arr[i], sizeof(struct AST_NODE_STRUCT));
        }
        printf("\n\n");
        counter++;
    }
}

void traverse_ast(ASTNode* node, List* flag, int depth, bool is_last) {
    if (node == NULL)
        return;

    for (int i = 1; i < depth; i++) {
        if (flag->arr[i] == true)
            printf("|     ");

        else
            printf("      ");
    }

    if (depth == 0)
        printf("%s", astnode_to_string(node));

    else if (is_last) {
        printf("+--- %s\n", astnode_to_string(node));

        flag->arr[depth] = false;
    }
    else 
        printf("+--- %s\n", astnode_to_string(node));

    int counter = 0;

    for (int i = 0; i < node->children->num_items; i++) {
        traverse_ast(node->children->arr[i], flag, depth + 1, (i == (node->children->num_items) - 1));
    }

    flag->arr[depth] = true;
}

void print_ast(Parser* parser) {
    //traverse_ast(parser->root, init_list);
    /* 
    static int count = 0;
    parser_count_nodes(parser->root, count);

    printf("Nodes %d\n", count);
  */
    //printf("%s", astnode_to_string(((ASTNode*)((ASTNode*)parser->root->children->arr[0])->children->arr[1])->children->arr[0]));
    
    bfs_ast(parser->root);

    printf("\n");
}

