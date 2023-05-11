
    pop rbx
    pop rax
    ; mov values into rax and rbx
    mov rdx, 0
    cqo
    idiv rbx
    push rax

