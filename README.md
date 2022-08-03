# BetCompiler
Compiler for my own language. Combination of my favorite features of python, C and Java

### Todo
- [x] Lexer

- [x] AST
  - [x] Expressions
  - [x] Functions
  - [x] Symbol declaration 

- [ ] Translating to x86 Assembly

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
- Translate AST to assembly to compile code


### How to compile code

1. Open terminal and go to where Bet is stored
2. make
3. ./a.out <bet file name>
