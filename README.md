# BetCompiler
Compiler for my own language

### Todo
- [x] Lexer

- [ ] AST
  - [x] Expressions
  - [ ] Functions

### Current Issues

#### Error: List Append
- Issue in list.c

'''c
if (!list_struct->arr) {
        list_struct->arr = calloc(2, list_struct->data_size);
    }
'''

