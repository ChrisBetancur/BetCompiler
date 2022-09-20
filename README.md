# BetCompiler
Compiler for my own language. Combination of my favorite features of python, C and Java

### Todo
- [x] Lexer

- [x] AST
  - [x] Expressions
  - [x] Functions
  - [x] Symbol declaration 
  - [x] Calling functions
  - [ ] Bug fixes

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


#### 2. Error: Symbol scope (Solved)
- Issue in ast_clean.c where its checking if symbol was previously defined in scope but instead it checks in any scope

```c
void main() {
    int i = 0;
    return;
}

i = 99;
```
 - Solution
    - Passed the current scope as param in is_symbol_in_scope function


### Next Steps
- Variables
- Translate AST to assembly to compile code


### How to compile code

1. Open terminal and go to where Bet is stored
2. make
3. ./a.out <bet file name>
