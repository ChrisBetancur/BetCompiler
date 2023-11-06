# BetCompiler
Compiler for my own language. Combination of my favorite features of Python, C and Java. Currently able to process basic arithmetic, store 64 bit integer variables an print one expression or integer variable at a time.

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
    - [ ] Pushing char variables to memory
    - [ ] Pushing string variables to memory
    - [x] Printing integer variable
    - [x] Printing string literals
    - [ ] Printing multiple arguments
    - [ ] Printing integer literals 
    - [x] Symbol table created
    - [ ] Bug fixes
- [ ] Translating to ARM Assembly
- [ ] Electron Frontend
    - [x] Create and edit bet files
    - [x] Compile bet file and show output
    - [ ] Make frontend look nice
    - [x] Show tokens
    - [x] Show AST
    - [x] Show Assembly code
    - [ ] Refactor frontend


### Next Steps
- Printing integer literals
- Pushing string variable to memory


### To compile bet files directly

Compile the compiler using

`make`

Compile the bet file using executable

`./output path/to/bet/file`

### To compile through Electron frontend
At bet-compiler-app directory

Install packages with

`npm install`

Start a local development environmnt

`npm run start`

Build the application if you want to get installable files

`npm run build`
