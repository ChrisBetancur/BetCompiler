.global _main
.align 2
_main:
	mov x0, #10
	mov x1, #2
	add x3, x0, x1
	mov x0, #2
	add x3, x3, x0
	mov x0, #33
	add x3, x3, x0
	mov x0, #31
	add x3, x3, x0
	mov x8, x3
	str x8, [sp, #4]
	mov x0, #0
	mov x16, #1
	svc 0
