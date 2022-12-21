.globl _main
.align 2
_main:

    mov x8, 66
    str x8, [sp]
    adr x0, Lstr
    bl _printf

    push {x8}

    mov x0, #0
    mov x16, #1
    svc 0

Lstr:
    .asciz "test: %x\n"

