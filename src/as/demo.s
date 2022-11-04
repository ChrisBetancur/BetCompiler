//
// Assembler program to print "Hello World!"
// to stdout.
//
// X0-X2 - parameters to linux function services
// X16 - linux function number
//
.global _main             // Provide program starting address to linker
.align 2

_main: 
    mov x0, #1
    adr x1, helloworld
    mov x2, #12
    mov x16, #4
    svc 0

    mov x0, #0
    mov x16, #1
    svc 0

helloworld:
    .ascii "Hello world\n"
