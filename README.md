# BetCompiler
Compiler for my own language. Combination of lanuages like python, C,

### Todo
- [x] Lexer

- [ ] AST
  - [x] Expressions
  - [ ] Functions

### Current Issues

#### 1. Error: List Append
- Issue in list.c, supposed to be 1 instead of 2 but both causes seg faults if there is certain amount of lines in bet code

```c
if (!list_struct->arr) {
        list_struct->arr = calloc(2, list_struct->data_size);
    }
```

### Next Steps
- Fix Error 1
- Finish AST for functions


### How to compile code

1. Open terminal
2. make
3. ./a.out <bet file name>
