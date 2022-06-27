# BetCompiler
Compiler for my own language. Combination of my favorite features of python, C and Java

### Todo
- [x] Lexer

- [ ] AST
  - [x] Expressions
  - [x] Functions
  - [ ] Symbol declaration 

### Current Issues

#### 1. Error: List Append (SOLVED)
- Issue in list.c, supposed to be 1 instead of 2 but both causes seg faults if there is certain amount of lines in bet code

```c
if (!list_struct->arr) {
        list_struct->arr = calloc(2, list_struct->data_size);
    }
```
-  Solution
    - Created new list append where I copied the list to a new array and free the old one

### Next Steps
- Clean up AST
    - check if var or function is defined properly


### How to compile code

1. Open terminal
2. make
3. ./a.out <bet file name>
