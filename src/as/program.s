.global main
main:
    ldr x16, =printf  // load the address of _printf into x16
    mov x0, #10        // move the number 10 into x0
    mov x1, fmt        // move the address of the format string into x1
    blr x16            // call _printf
    mov x0, #0         // move 0 into x0 (return value for main)
    ret                // return from main

