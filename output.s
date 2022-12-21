.global _main
.align 2
_main:
	mov x3, #98
	mov x8, x3
	str x8, [sp, #4]
	mov x8, #0

	mov x8, #66
	str x8, [sp, #0]
	adr x0, ascii
	bl _printf

	mov x8, #22
	str x8, [sp, #0]
	adr x0, ascii
	bl _printf

	mov x8, #22
	str x8, [sp, #0]
	adr x0, ascii
	bl _printf

	mov x8, #22
	str x8, [sp, #0]
	adr x0, ascii
	bl _printf

	mov x8, #22
	str x8, [sp, #0]
	adr x0, ascii
	bl _printf

	mov x8, #22
	str x8, [sp, #0]
	adr x0, ascii
	bl _printf

	mov x8, #22
	str x8, [sp, #0]
	adr x0, ascii
	bl _printf

	mov x8, #22
	str x8, [sp, #0]
	adr x0, ascii
	bl _printf

	mov x8, #13
	str x8, [sp, #0]
	adr x0, ascii
	bl _printf

	mov x0, #0
	mov x16, #1
	svc 0
ascii:
	.asciz "%d\n"
