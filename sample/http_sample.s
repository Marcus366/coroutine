	.file	"http_sample.c"
	.globl	uuid
	.bss
	.align 4
	.type	uuid, @object
	.size	uuid, 4
uuid:
	.zero	4
	.globl	out
	.section	.rodata
	.align 8
.LC0:
	.string	"HTTP/1.1 200 OK\r\nServer: COROUTINE/0.1\r\nDate: Sat, 31 Dec 2014 23:59:59\r\nContent-Type: text/html\r\nContent-Length: 0\r\n\r\n"
	.data
	.align 8
	.type	out, @object
	.size	out, 8
out:
	.quad	.LC0
	.text
	.globl	co_listen
	.type	co_listen, @function
co_listen:
.LFB38:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$1072, %rsp
	movq	%rdi, -1064(%rbp)
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	movl	$0, -1052(%rbp)
	movq	-1064(%rbp), %rax
	movl	%eax, -1048(%rbp)
	jmp	.L2
.L20:
	movl	$0, -1056(%rbp)
	jmp	.L3
.L18:
	movl	-1052(%rbp), %eax
	cmpl	$1, %eax
	je	.L5
	cmpl	$1, %eax
	jg	.L6
	testl	%eax, %eax
	je	.L7
	jmp	.L11
.L6:
	cmpl	$2, %eax
	je	.L8
	cmpl	$3, %eax
	je	.L9
	jmp	.L11
.L7:
	movl	-1056(%rbp), %eax
	cltq
	movzbl	-1040(%rbp,%rax), %eax
	cmpb	$13, %al
	jne	.L10
	movl	$1, -1052(%rbp)
	jmp	.L11
.L10:
	jmp	.L11
.L5:
	movl	-1056(%rbp), %eax
	cltq
	movzbl	-1040(%rbp,%rax), %eax
	cmpb	$10, %al
	jne	.L12
	movl	$2, -1052(%rbp)
	jmp	.L11
.L12:
	movl	$0, -1052(%rbp)
	jmp	.L11
.L8:
	movl	-1056(%rbp), %eax
	cltq
	movzbl	-1040(%rbp,%rax), %eax
	cmpb	$13, %al
	jne	.L14
	movl	$3, -1052(%rbp)
	jmp	.L11
.L14:
	movl	$0, -1052(%rbp)
	jmp	.L11
.L9:
	movl	-1056(%rbp), %eax
	cltq
	movzbl	-1040(%rbp,%rax), %eax
	cmpb	$10, %al
	jne	.L16
	movl	$4, -1052(%rbp)
	jmp	.L24
.L16:
	movl	$0, -1052(%rbp)
.L24:
	nop
.L11:
	addl	$1, -1056(%rbp)
.L3:
	movl	-1056(%rbp), %eax
	cmpl	-1044(%rbp), %eax
	jl	.L18
.L2:
	cmpl	$4, -1052(%rbp)
	je	.L19
	leaq	-1040(%rbp), %rcx
	movl	-1048(%rbp), %eax
	movl	$1024, %edx
	movq	%rcx, %rsi
	movl	%eax, %edi
	call	read
	movl	%eax, -1044(%rbp)
	cmpl	$0, -1044(%rbp)
	jg	.L20
.L19:
	cmpl	$4, -1052(%rbp)
	jne	.L21
	movq	out(%rip), %rax
	movq	%rax, %rdi
	call	strlen
	movq	%rax, %rdx
	movq	out(%rip), %rcx
	movl	-1048(%rbp), %eax
	movq	%rcx, %rsi
	movl	%eax, %edi
	call	co_write
	movl	-1048(%rbp), %eax
	movl	%eax, %edi
	call	close
.L21:
	nop
	movq	-8(%rbp), %rax
	xorq	%fs:40, %rax
	je	.L23
	call	__stack_chk_fail
.L23:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE38:
	.size	co_listen, .-co_listen
	.section	.rodata
.LC1:
	.string	"co_tcp_open error"
.LC2:
	.string	"0.0.0.0"
.LC3:
	.string	"ipaddr init error"
.LC4:
	.string	"bind error"
.LC5:
	.string	"listen error"
.LC6:
	.string	"accept error"
.LC7:
	.string	"coroutine_create error"
	.text
	.globl	main
	.type	main, @function
main:
.LFB39:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%rbx
	subq	$72, %rsp
	.cfi_offset 3, -24
	movq	%fs:40, %rax
	movq	%rax, -24(%rbp)
	xorl	%eax, %eax
	movl	$0, %eax
	call	coroutine_init
	leaq	-80(%rbp), %rax
	movq	%rax, %rdi
	call	co_tcp4_open
	cmpl	$-1, %eax
	jne	.L26
	movl	$.LC1, %edi
	call	perror
	movl	$-1, %edi
	call	exit
.L26:
	leaq	-64(%rbp), %rax
	movl	$8080, %edx
	movl	$.LC2, %esi
	movq	%rax, %rdi
	call	co_ip4_addr_init
	cmpl	$-1, %eax
	jne	.L27
	movl	$.LC3, %edi
	call	perror
	movl	$-1, %edi
	call	exit
.L27:
	leaq	-64(%rbp), %rdx
	leaq	-80(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	co_tcp_bind
	cmpl	$-1, %eax
	jne	.L28
	movl	$.LC4, %edi
	call	perror
	movl	$-1, %edi
	call	exit
.L28:
	leaq	-80(%rbp), %rax
	movl	$1024, %esi
	movq	%rax, %rdi
	call	co_tcp_listen
	cmpl	$-1, %eax
	jne	.L29
	movl	$.LC5, %edi
	call	perror
	movl	$-1, %edi
	call	exit
.L29:
	movl	$16, -72(%rbp)
	leaq	-72(%rbp), %rdx
	leaq	-48(%rbp), %rcx
	leaq	-80(%rbp), %rax
	movq	%rcx, %rsi
	movq	%rax, %rdi
	call	co_tcp_accept
	movl	%eax, -68(%rbp)
	cmpl	$-1, -68(%rbp)
	jne	.L30
	movl	$.LC6, %edi
	call	perror
	movl	$-1, %edi
	call	exit
.L30:
	movl	uuid(%rip), %eax
	addl	$1, %eax
	movl	%eax, uuid(%rip)
	movl	uuid(%rip), %eax
	cmpl	$60000, %eax
	jne	.L31
	movl	$0, %eax
	movq	-24(%rbp), %rbx
	xorq	%fs:40, %rbx
	je	.L34
	jmp	.L35
.L31:
	movl	-68(%rbp), %eax
	cltq
	movq	%rax, %rdx
	movl	$co_listen, %esi
	movl	$0, %edi
	call	coroutine_create
	testq	%rax, %rax
	jne	.L33
	movl	$.LC7, %edi
	movl	$0, %eax
	call	printf
	movl	$-1, %edi
	call	exit
.L33:
	jmp	.L29
.L35:
	call	__stack_chk_fail
.L34:
	addq	$72, %rsp
	popq	%rbx
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE39:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 4.8.2-19ubuntu1) 4.8.2"
	.section	.note.GNU-stack,"",@progbits
