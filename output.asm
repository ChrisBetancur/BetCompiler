global _start

section .text

_start:

    push 4
    push 3
    mov rsi, 2
    mov rdi, 1

    call main
    mov rsi, rax
    mov [var_stack - 0x10], rsi

    mov rdi, [var_stack - 0x10]
    push rdi

    pop rax
     call print_int
    mov rax, SYS_EXIT
    xor rdi, rdi
    syscall
main:
    push rbp
    mov rbp, rsp
    sub rsp, 0x30

    mov QWORD [rbp - 0x10], rdi
    mov QWORD [rbp - 0x18], rsi
    mov r8, QWORD [rbp + 0x10]
    mov QWORD [rbp - 0x20], r8
    mov r8, QWORD [rbp + 0x18]
    mov QWORD [rbp - 0x28], r8


    mov rdi, [rbp - 0x10]
    push rdi

    mov rdi, [rbp - 0x18]
    push rdi

    mov rdi, [rbp - 0x20]
    push rdi

    pop rdx
    pop rax
    ; move value into rax and rdx
    mul rdx
    push rax

    mov rdi, [rbp - 0x28]
    push rdi

    pop rdx
    pop rax
    ; mov values into rax and rdx
    add rax, rdx
    push rax


    mov rdi, [rbp - 0x18]
    push rdi

    pop rbx
    pop rax
    ; mov values into rax and rbx
    mov rdx, 0
    cqo
    idiv rbx
    push rax


    pop rdx
    pop rax
    ; mov values into rax and rdx
    add rax, rdx
    push rax

    pop rsi
    mov [rbp - 0x30], rsi

    mov rax, QWORD [rbp - 0x30]
    mov rsp, rbp
    pop rbp
    ret

; print_int
; converts unsigned int to string
; params:
; rax: stores the number to convert

print_int:
    test rax, rax
    js print_negate


finish_neg:

    mov rcx, str_buf ; pointer to str_buf
    mov rbx, 10 ; new line char
    mov [rcx], rbx ; first value at pointer rcx is the new line char
    inc rcx ; inc rcx pointer
    mov [str_buf_pos], rcx ; let str_buf_pos pointer to the current position of the str_buf

print_int_loop:
    mov rdx, 0 ; rdx set 0 so it doesn't get concentanted to div
    mov rbx, 10
    div rbx ; divides rax by rbx (10)
    push rax
    add rdx, 48 ; remainder is stored in rdx then add 48 to convert char to int which is the int we need to store into the buffer\

    mov rcx, [str_buf_pos]
    mov [rcx], dl ; get the lower character of rcx
    inc rcx ; increment index
    mov [str_buf_pos], rcx

    pop rax
    cmp rax, 0
    jne print_int_loop

print_reverse:
    mov r10, [is_str_buf_neg]
    cmp r10, 1
    je print_sign

print_resume:
    mov rcx, [str_buf_pos]

    mov rax, SYS_WRITE
    mov rdi, STDOUT
    mov rsi, rcx
    mov rdx, 1
    syscall

    mov rcx, [str_buf_pos]
    dec rcx
    mov [str_buf_pos], rcx

    cmp rcx, str_buf
    jge print_reverse

    ret

print_negate:
    mov word [is_str_buf_neg], 1 ; check if integer is a negative
    neg rax

    jmp finish_neg

print_sign:
    mov rax, SYS_WRITE
    mov rdi, STDOUT
    mov rsi, hyphen
    mov rdx, 1
    syscall
    mov word [is_str_buf_neg], 0
    jmp print_resume

SYS_WRITE equ 1
SYS_EXIT equ 60
STDOUT equ 1

section .data
    hyphen db '-', 0
    var_stack_size equ 400

section .bss
    str_buf resb 100
    str_buf_pos resb 8
    is_str_buf_neg resb 1

    var_stack resb var_stack_size
