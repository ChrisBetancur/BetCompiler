# BetCompiler
Compiler for my own language. Combination of my favorite features of python, C and Java. Currently able to process basic arithmetic, store 64 bit integer variables and print one expression or integer variable at a time.

### Road Map
- [x] Lexer

- [x] AST
  - [x] Expressions
  - [x] Functions
  - [x] Symbol declaration 
  - [x] Calling functions
  - [x] Bug fixes

- [ ] Translating to x86 Assembly
    - [x] Pushing integer variables to memory
    - [x] Printing one variable to terminal
    - [ ] Bug fixes
- [ ] Translating to ARM Assembly

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


#### 2. Error: Symbol scope (SOLVED)
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

#### 3. Error: Assigning Var to Var (UNSOLVED)
- When assigning var to another var in a function, parser does not check if var has been defined in the function scope
  since everything in the function has not been inserted into the Abstract Syntax Tree (Ref: parser.c::202 in parse_factor function)

```c
void main() {
    int i = 0;
    int s = i;
    return;
}
```

#### 4. Error: Returning Var (Solved)
- In a int function, when returning int, parser is not allowing to return a * b, 2 * 3. Also it incorrectly allows
  return int a = 0;

```c
int mult(int a, int b) {
    return int a = 0; // ALLOWED BUT INCORRECT
}

int mult(int a, int b) {
    return a * b; // WON'T ALLOW
}

int mult(int a, int b) {
    return 2 * 3; // WON'T ALLOW
}

int mult(int a, int b) {
    return (2 * 9) * 3; // WON'T ALLOW
}
```

- Solution
    - For funcs that return int it will parse return statement as expressions where parsing factors will be able to parse
      vars as a factor (IT DOES NOT CHECK IF VAR HAS BEEN DEFINED AS A INT AND DOES NOT CHECK IF IT HAS BEEN ASSIGNED VALUE)
      all other cases will have a if statement specifying how to deal with return statement

#### 5. Error: Operations on intergers no assembling (UNSOLVED)
- 
```c

```
 - Solution
    - Passed the current scope as param in is_symbol_in_scope function

### Next Steps
- Handling Errors
- Handling multiple params in print function
- Handling functions


### How to compile code

1. Open terminal and go to where Bet is stored
2. make
3. ./a.out <bet file name>
