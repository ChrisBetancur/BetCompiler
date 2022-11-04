
.global _main
.align 2

_main:
    mov x3, #0
    ldr x0, num
    str x3, [x0]

loop:
    add x3, x3, #1
    adr x1, num
    mov x2, #1
    mov x16, #4
    svc 0
    
    cmp x3, #9
    bne loop

exit:
    mov x0, #0
    mov x16, #1
    svc 0

num:
    .word
