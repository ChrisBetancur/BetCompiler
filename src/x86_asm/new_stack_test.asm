section .data
stack_size equ 100 ; size of stack in bytes

section .bss
stack_space resb stack_size ; allocate space for the stack

section .text
global _start

_start:
  ; initialize the stack pointer to the top of the stack
  mov rsp, stack_space + stack_size

  ; push an integer value onto the stack
  mov dword [rsp], 42 ; push 42 onto the stack

  ; pop the top value off the stack and store it in a register
  pop rax ; rax now contains the value 42

  ; push another integer value onto the stack
  mov dword [rsp], 1337 ; push 1337 onto the stack

  ; pop the top value off the stack and store it in a register
  pop rbx ; rbx now contains the value 1337

  ; exit the program
  mov eax, 60 ; exit syscall number
  xor edi, edi ; exit status code of 0
  syscall ; invoke the syscall

