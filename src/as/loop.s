
.global _main
.align 2

_main:
    mov x3, #2

.loop:
    cmp x3, #0
    ble .endloop
    sub x3, x3, #1
    b .loop

.endloop:
    mov x0, #0
    mov x16, #1
    svc 0
