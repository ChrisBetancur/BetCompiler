global _start

section .text

_start:

    mov rsi, 8
    mov rdi, 5

    call add
    mov rsi, rax
    mov QWORD [var_stack + 0x8], rsi

    mov rdi, [var_stack + 0x8]
    push rdi

    pop rax
    call print_int
    mov rax, SYS_EXIT
    xor rdi, rdi
    syscall
add:
    push rbp
    mov rbp, rsp
    sub rsp, 0x18

    mov QWORD [rbp - 0x8], rdi
    mov QWORD [rbp - 0x10], rsi


    mov rdi, [rbp - 0x8]
    push rdi

    mov rdi, [rbp - 0x10]
    push rdi

    pop rdx
    pop rax
    ; mov values into rax and rdx
    add rax, rdx
    push rax

    pop rsi
    mov QWORD [rbp - 0x18], rsi

    mov rsp, rbp
    pop rbp
    ret

%include "target/print_int.asm"

SYS_WRITE equ 1
SYS_EXIT equ 60
STDOUT equ 1

section .data
    var_stack_size equ 400

section .bss
    var_stack resb var_stack_size
