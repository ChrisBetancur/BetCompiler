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

#### 3. Error: Assigning Var to Var (Unsolved)
- When assigning var to another var in a function, parser does not check if var has been defined in the function scope
  since everything in the function has not been inserted into the Abstract Syntax Tree (Ref: parser.c::202 in parse_factor function)

```c
void main() {
    int i = 0;
    int s = i;
    return;
}
```
#### 3. Error: Returning Var (Unsolved)
- In a int function, when returning int, parser is not allowing to return a * b, 2 * 3. Also it incorrectly allows
  return int a = 0;

```c
int mult(int a, int b) {
    return int a = 0; // INCORRECT
}

int mult(int a, int b) {
    return a * b; // WON'T ALLOW
}

int mult(int a, int b) {
    return 2 * 3; // WON'T ALLOW
}
```

 

### Next Steps
- Variables
- Translate AST to assembly to compile code


### How to compile code

1. Open terminal and go to where Bet is stored
2. make
3. ./a.out <bet file name>
