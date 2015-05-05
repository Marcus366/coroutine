	.file	"test_resume.c"
	.comm	ctx,16,16
	.local	seq
	.comm	seq,8,8
	.local	co1_run
	.comm	co1_run,4,4
	.local	co2_run
	.comm	co2_run,4,4
	.text
	.globl	func1
	.type	func1, @function
func1:
.LFB38:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -8(%rbp)
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE38:
	.size	func1, .-func1
	.globl	func2
	.type	func2, @function
func2:
.LFB39:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movq	%rdi, -24(%rbp)
	movl	$0, -4(%rbp)
	jmp	.L4
.L5:
	movq	seq(%rip), %rax
	addq	$1, %rax
	movq	%rax, seq(%rip)
	movl	co2_run(%rip), %eax
	addl	$1, %eax
	movl	%eax, co2_run(%rip)
	movq	ctx(%rip), %rax
	movq	%rax, %rdi
	call	coroutine_resume
	addl	$1, -4(%rbp)
.L4:
	cmpl	$9, -4(%rbp)
	jle	.L5
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE39:
	.size	func2, .-func2
	.globl	main
	.type	main, @function
main:
.LFB40:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	$0, %eax
	call	coroutine_init
	movl	$10, %edx
	movl	$func1, %esi
	movl	$0, %edi
	call	coroutine_create
	movq	%rax, ctx(%rip)
	movq	ctx(%rip), %rax
	movq	%rax, %rdi
	call	coroutine_resume
	movl	$0, %eax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE40:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 4.8.2-19ubuntu1) 4.8.2"
	.section	.note.GNU-stack,"",@progbits
