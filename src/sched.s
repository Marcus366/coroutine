	.file	"sched.c"
	.text
	.type	INIT_LIST_HEAD, @function
INIT_LIST_HEAD:
.LFB2:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -8(%rbp)
	movq	-8(%rbp), %rax
	movq	-8(%rbp), %rdx
	movq	%rdx, (%rax)
	movq	-8(%rbp), %rax
	movq	-8(%rbp), %rdx
	movq	%rdx, 8(%rax)
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE2:
	.size	INIT_LIST_HEAD, .-INIT_LIST_HEAD
	.type	__list_add, @function
__list_add:
.LFB3:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movq	%rdx, -24(%rbp)
	movq	-24(%rbp), %rax
	movq	-8(%rbp), %rdx
	movq	%rdx, 8(%rax)
	movq	-8(%rbp), %rax
	movq	-24(%rbp), %rdx
	movq	%rdx, (%rax)
	movq	-8(%rbp), %rax
	movq	-16(%rbp), %rdx
	movq	%rdx, 8(%rax)
	movq	-16(%rbp), %rax
	movq	-8(%rbp), %rdx
	movq	%rdx, (%rax)
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE3:
	.size	__list_add, .-__list_add
	.type	list_add_tail, @function
list_add_tail:
.LFB5:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movq	-16(%rbp), %rax
	movq	8(%rax), %rcx
	movq	-16(%rbp), %rdx
	movq	-8(%rbp), %rax
	movq	%rcx, %rsi
	movq	%rax, %rdi
	call	__list_add
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE5:
	.size	list_add_tail, .-list_add_tail
	.type	__list_del, @function
__list_del:
.LFB6:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movq	-16(%rbp), %rax
	movq	-8(%rbp), %rdx
	movq	%rdx, 8(%rax)
	movq	-8(%rbp), %rax
	movq	-16(%rbp), %rdx
	movq	%rdx, (%rax)
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE6:
	.size	__list_del, .-__list_del
	.type	list_del, @function
list_del:
.LFB8:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$8, %rsp
	movq	%rdi, -8(%rbp)
	movq	-8(%rbp), %rax
	movq	(%rax), %rdx
	movq	-8(%rbp), %rax
	movq	8(%rax), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	__list_del
	movq	-8(%rbp), %rax
	movq	$0, (%rax)
	movq	-8(%rbp), %rax
	movq	$0, 8(%rax)
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE8:
	.size	list_del, .-list_del
	.type	list_is_suspend, @function
list_is_suspend:
.LFB15:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -8(%rbp)
	movq	-8(%rbp), %rax
	movq	8(%rax), %rax
	testq	%rax, %rax
	jne	.L7
	movq	-8(%rbp), %rax
	movq	(%rax), %rax
	testq	%rax, %rax
	jne	.L7
	movl	$1, %eax
	jmp	.L8
.L7:
	movl	$0, %eax
.L8:
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE15:
	.size	list_is_suspend, .-list_is_suspend
	.type	list_empty, @function
list_empty:
.LFB16:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -8(%rbp)
	movq	-8(%rbp), %rax
	movq	(%rax), %rax
	cmpq	-8(%rbp), %rax
	sete	%al
	movzbl	%al, %eax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE16:
	.size	list_empty, .-list_empty
	.comm	g_fds,8,8
	.comm	g_pollfd,4,4
	.comm	g_exit_coroutine_ctx,8,8
	.globl	coroutine_sched_init
	.type	coroutine_sched_init, @function
coroutine_sched_init:
.LFB38:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movl	$0, %eax
	call	coroutine_ctx_new_exit
	movq	%rax, g_exit_coroutine_ctx(%rip)
	movq	g_exit_coroutine_ctx(%rip), %rax
	testq	%rax, %rax
	jne	.L13
	movl	$-1, %eax
	jmp	.L20
.L13:
	leaq	-16(%rbp), %rax
	movq	%rax, %rsi
	movl	$7, %edi
	call	getrlimit
	testl	%eax, %eax
	je	.L15
	movl	$-1, %eax
	jmp	.L20
.L15:
	movq	-8(%rbp), %rax
	salq	$4, %rax
	movq	%rax, %rdi
	call	malloc
	movq	%rax, g_fds(%rip)
	movq	g_fds(%rip), %rax
	testq	%rax, %rax
	jne	.L16
	movl	$-1, %eax
	jmp	.L20
.L16:
	movl	$0, -20(%rbp)
	jmp	.L17
.L18:
	movq	g_fds(%rip), %rax
	movl	-20(%rbp), %edx
	movslq	%edx, %rdx
	salq	$4, %rdx
	addq	%rdx, %rax
	movq	%rax, %rdi
	call	INIT_LIST_HEAD
	addl	$1, -20(%rbp)
.L17:
	movq	-8(%rbp), %rax
	cmpl	-20(%rbp), %eax
	jg	.L18
	movl	$10240, %edi
	call	epoll_create
	movl	%eax, g_pollfd(%rip)
	movl	g_pollfd(%rip), %eax
	cmpl	$-1, %eax
	jne	.L19
	movl	$-1, %eax
	jmp	.L20
.L19:
	movl	$0, %eax
.L20:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE38:
	.size	coroutine_sched_init, .-coroutine_sched_init
	.globl	coroutine_register_fd
	.type	coroutine_register_fd, @function
coroutine_register_fd:
.LFB39:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	%edi, -4(%rbp)
	movl	$0, %eax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE39:
	.size	coroutine_register_fd, .-coroutine_register_fd
	.globl	coroutine_unregister_fd
	.type	coroutine_unregister_fd, @function
coroutine_unregister_fd:
.LFB40:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	%edi, -4(%rbp)
	movl	$0, %eax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE40:
	.size	coroutine_unregister_fd, .-coroutine_unregister_fd
	.section	.rodata
.LC0:
	.string	"sched.c"
.LC1:
	.string	"list_is_suspend(&ctx->queue)"
	.text
	.globl	coroutine_block
	.type	coroutine_block, @function
coroutine_block:
.LFB41:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$48, %rsp
	movl	%edi, -36(%rbp)
	movl	%esi, -40(%rbp)
	movq	g_coroutine_running_ctx(%rip), %rax
	movq	%rax, -24(%rbp)
	movq	-24(%rbp), %rax
	movl	$2, 8(%rax)
	movq	-24(%rbp), %rax
	addq	$64, %rax
	movq	%rax, %rdi
	call	list_is_suspend
	testl	%eax, %eax
	jne	.L26
	movl	$__PRETTY_FUNCTION__.3997, %ecx
	movl	$79, %edx
	movl	$.LC0, %esi
	movl	$.LC1, %edi
	call	__assert_fail
.L26:
	movq	g_fds(%rip), %rax
	movl	-36(%rbp), %edx
	movslq	%edx, %rdx
	salq	$4, %rdx
	addq	%rdx, %rax
	movq	-24(%rbp), %rdx
	addq	$64, %rdx
	movq	%rax, %rsi
	movq	%rdx, %rdi
	call	list_add_tail
	movl	-40(%rbp), %eax
	movl	%eax, -16(%rbp)
	movl	-36(%rbp), %eax
	movl	%eax, -12(%rbp)
	movl	g_pollfd(%rip), %eax
	leaq	-16(%rbp), %rcx
	movl	-36(%rbp), %edx
	movl	$1, %esi
	movl	%eax, %edi
	call	epoll_ctl
	movl	$0, %eax
	call	coroutine_sched
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE41:
	.size	coroutine_block, .-coroutine_block
	.section	.rodata
.LC2:
	.string	"epoll del error"
.LC3:
	.string	"list_empty(&g_fds[fd].wq)"
.LC4:
	.string	"list:"
.LC5:
	.string	"%llu "
.LC6:
	.string	"running"
	.align 8
.LC7:
	.string	"state not consistent, running but in queue"
.LC8:
	.string	"blocking"
	.align 8
.LC9:
	.string	"state not consistent blocking but not in queue"
.LC10:
	.string	"ready"
	.align 8
.LC11:
	.string	"state not consistent ready but not in queue"
.LC12:
	.string	"ready list:"
.LC13:
	.string	"state not consistent running"
.LC14:
	.string	"state not consistent blocking"
.LC15:
	.string	"state not consistent"
.LC16:
	.string	"sched coroutine cid: %llu\n"
	.text
	.globl	coroutine_sched
	.type	coroutine_sched, @function
coroutine_sched:
.LFB42:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$122992, %rsp
.L28:
	movq	g_coroutine_ready_list(%rip), %rax
	movq	%rax, -122952(%rbp)
	movq	-122952(%rbp), %rax
	subq	$64, %rax
	movq	%rax, -122968(%rbp)
	jmp	.L29
.L32:
	movq	-122968(%rbp), %rax
	movl	8(%rax), %eax
	testl	%eax, %eax
	jne	.L30
	nop
.L31:
	movl	$.LC4, %edi
	call	puts
	movq	g_coroutine_list(%rip), %rax
	movq	%rax, -122944(%rbp)
	movq	-122944(%rbp), %rax
	subq	$48, %rax
	movq	%rax, -122960(%rbp)
	jmp	.L40
.L30:
	movq	-122968(%rbp), %rax
	movq	64(%rax), %rax
	movq	%rax, -122912(%rbp)
	movq	-122912(%rbp), %rax
	subq	$64, %rax
	movq	%rax, -122968(%rbp)
.L29:
	movq	-122968(%rbp), %rax
	addq	$64, %rax
	cmpq	$g_coroutine_ready_list, %rax
	jne	.L32
	movl	g_pollfd(%rip), %eax
	leaq	-122880(%rbp), %rsi
	movl	$-1, %ecx
	movl	$10240, %edx
	movl	%eax, %edi
	call	epoll_wait
	movl	%eax, -122976(%rbp)
	movl	$0, -122980(%rbp)
	jmp	.L33
.L39:
	movl	-122980(%rbp), %eax
	movslq	%eax, %rdx
	movq	%rdx, %rax
	addq	%rax, %rax
	addq	%rdx, %rax
	salq	$2, %rax
	addq	%rbp, %rax
	subq	$122880, %rax
	movl	4(%rax), %eax
	movl	%eax, -122972(%rbp)
	movq	g_fds(%rip), %rax
	movl	-122972(%rbp), %edx
	movslq	%edx, %rdx
	salq	$4, %rdx
	addq	%rdx, %rax
	movq	(%rax), %rax
	movq	%rax, -122904(%rbp)
	movq	-122904(%rbp), %rax
	subq	$64, %rax
	movq	%rax, -122968(%rbp)
	movq	-122968(%rbp), %rax
	movq	64(%rax), %rax
	movq	%rax, -122896(%rbp)
	movq	-122896(%rbp), %rax
	subq	$64, %rax
	movq	%rax, -122960(%rbp)
	jmp	.L34
.L36:
	movq	-122968(%rbp), %rax
	movl	8(%rax), %eax
	cmpl	$2, %eax
	jne	.L35
	movq	-122968(%rbp), %rax
	movl	$0, 8(%rax)
	movq	-122968(%rbp), %rax
	addq	$64, %rax
	movq	%rax, %rdi
	call	list_del
	movq	-122968(%rbp), %rax
	addq	$64, %rax
	movl	$g_coroutine_ready_list, %esi
	movq	%rax, %rdi
	call	list_add_tail
.L35:
	movq	-122960(%rbp), %rax
	movq	%rax, -122968(%rbp)
	movq	-122960(%rbp), %rax
	movq	64(%rax), %rax
	movq	%rax, -122888(%rbp)
	movq	-122888(%rbp), %rax
	subq	$64, %rax
	movq	%rax, -122960(%rbp)
.L34:
	movq	-122968(%rbp), %rax
	leaq	64(%rax), %rcx
	movq	g_fds(%rip), %rax
	movl	-122972(%rbp), %edx
	movslq	%edx, %rdx
	salq	$4, %rdx
	addq	%rdx, %rax
	cmpq	%rax, %rcx
	jne	.L36
	movl	g_pollfd(%rip), %eax
	leaq	-122880(%rbp), %rcx
	movl	-122972(%rbp), %edx
	movl	$2, %esi
	movl	%eax, %edi
	call	epoll_ctl
	cmpl	$-1, %eax
	jne	.L37
	movl	$.LC2, %edi
	call	perror
	movl	$-1, %edi
	call	exit
.L37:
	movq	g_fds(%rip), %rax
	movl	-122972(%rbp), %edx
	movslq	%edx, %rdx
	salq	$4, %rdx
	addq	%rdx, %rax
	movq	%rax, %rdi
	call	list_empty
	testl	%eax, %eax
	jne	.L38
	movl	$__PRETTY_FUNCTION__.4024, %ecx
	movl	$123, %edx
	movl	$.LC0, %esi
	movl	$.LC3, %edi
	call	__assert_fail
.L38:
	addl	$1, -122980(%rbp)
.L33:
	movl	-122980(%rbp), %eax
	cmpl	-122976(%rbp), %eax
	jl	.L39
	jmp	.L28
.L49:
	movq	-122960(%rbp), %rax
	movq	(%rax), %rax
	movq	%rax, %rsi
	movl	$.LC5, %edi
	movl	$0, %eax
	call	printf
	movq	-122960(%rbp), %rax
	movl	8(%rax), %eax
	cmpl	$1, %eax
	je	.L42
	cmpl	$1, %eax
	jb	.L43
	cmpl	$2, %eax
	je	.L44
	jmp	.L60
.L42:
	movl	$.LC6, %edi
	call	puts
	movq	-122960(%rbp), %rax
	addq	$64, %rax
	movq	%rax, %rdi
	call	list_is_suspend
	testl	%eax, %eax
	jne	.L45
	movl	$.LC7, %edi
	call	puts
	movl	$-1, %edi
	call	exit
.L45:
	jmp	.L46
.L44:
	movl	$.LC8, %edi
	call	puts
	movq	-122960(%rbp), %rax
	addq	$64, %rax
	movq	%rax, %rdi
	call	list_is_suspend
	testl	%eax, %eax
	je	.L47
	movl	$.LC9, %edi
	call	puts
	movl	$-1, %edi
	call	exit
.L47:
	jmp	.L46
.L43:
	movl	$.LC10, %edi
	call	puts
	movq	-122960(%rbp), %rax
	movq	(%rax), %rdx
	movq	g_exit_coroutine_ctx(%rip), %rax
	movq	(%rax), %rax
	cmpq	%rax, %rdx
	je	.L48
	movq	-122968(%rbp), %rax
	addq	$64, %rax
	movq	%rax, %rdi
	call	list_is_suspend
	testl	%eax, %eax
	je	.L48
	movl	$.LC11, %edi
	call	puts
	movl	$-1, %edi
	call	exit
.L48:
	jmp	.L46
.L60:
	movl	$10, %edi
	call	putchar
.L46:
	movq	-122960(%rbp), %rax
	movq	48(%rax), %rax
	movq	%rax, -122936(%rbp)
	movq	-122936(%rbp), %rax
	subq	$48, %rax
	movq	%rax, -122960(%rbp)
.L40:
	movq	-122960(%rbp), %rax
	addq	$48, %rax
	cmpq	$g_coroutine_list, %rax
	jne	.L49
	movl	$.LC12, %edi
	call	puts
	movq	g_coroutine_ready_list(%rip), %rax
	movq	%rax, -122928(%rbp)
	movq	-122928(%rbp), %rax
	subq	$64, %rax
	movq	%rax, -122960(%rbp)
	jmp	.L50
.L56:
	movq	-122960(%rbp), %rax
	movq	(%rax), %rax
	movq	%rax, %rsi
	movl	$.LC5, %edi
	movl	$0, %eax
	call	printf
	movq	-122960(%rbp), %rax
	movl	8(%rax), %eax
	cmpl	$1, %eax
	je	.L52
	cmpl	$1, %eax
	jb	.L53
	cmpl	$2, %eax
	je	.L54
	jmp	.L51
.L52:
	movl	$.LC13, %edi
	call	puts
	movl	$-1, %edi
	call	exit
.L54:
	movl	$.LC14, %edi
	call	puts
	movl	$-1, %edi
	call	exit
.L53:
	movq	-122960(%rbp), %rax
	movq	(%rax), %rdx
	movq	g_exit_coroutine_ctx(%rip), %rax
	movq	(%rax), %rax
	cmpq	%rax, %rdx
	jne	.L55
	movl	$.LC15, %edi
	call	puts
	movl	$-1, %edi
	call	exit
.L55:
	nop
.L51:
	movq	-122960(%rbp), %rax
	movq	64(%rax), %rax
	movq	%rax, -122920(%rbp)
	movq	-122920(%rbp), %rax
	subq	$64, %rax
	movq	%rax, -122960(%rbp)
.L50:
	movq	-122960(%rbp), %rax
	addq	$64, %rax
	cmpq	$g_coroutine_ready_list, %rax
	jne	.L56
	movl	$10, %edi
	call	putchar
	movq	-122968(%rbp), %rax
	movq	(%rax), %rax
	movq	%rax, %rsi
	movl	$.LC16, %edi
	movl	$0, %eax
	call	printf
	movq	g_coroutine_running_ctx(%rip), %rax
	movq	%rax, -122960(%rbp)
	movq	-122960(%rbp), %rax
	movq	(%rax), %rdx
	movq	-122968(%rbp), %rax
	movq	(%rax), %rax
	cmpq	%rax, %rdx
	je	.L57
	movq	-122968(%rbp), %rax
	movq	%rax, g_coroutine_running_ctx(%rip)
	movq	-122968(%rbp), %rax
	addq	$64, %rax
	movq	%rax, %rdi
	call	list_del
	movq	-122968(%rbp), %rax
	movl	$1, 8(%rax)
	movq	-122968(%rbp), %rdx
	movq	-122960(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	coroutine_sched_swap_context
	jmp	.L58
.L57:
	movq	-122960(%rbp), %rax
	addq	$64, %rax
	movq	%rax, %rdi
	call	list_del
.L58:
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE42:
	.size	coroutine_sched, .-coroutine_sched
	.globl	coroutine_sched_swap_context
	.type	coroutine_sched_swap_context, @function
coroutine_sched_swap_context:
.LFB43:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movq	-16(%rbp), %rax
	leaq	32(%rax), %rdx
	movq	-8(%rbp), %rax
	addq	$32, %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	coroutine__swap_context
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE43:
	.size	coroutine_sched_swap_context, .-coroutine_sched_swap_context
	.section	.rodata
.LC17:
	.string	"coroutine exit: %llu\n"
	.text
	.globl	coroutine_exit
	.type	coroutine_exit, @function
coroutine_exit:
.LFB44:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movq	%rdi, -24(%rbp)
.L66:
	movq	g_coroutine_running_ctx(%rip), %rax
	movq	%rax, -16(%rbp)
	movq	-16(%rbp), %rax
	movq	16(%rax), %rax
	movq	%rax, -8(%rbp)
	movq	g_exit_coroutine_ctx(%rip), %rax
	movq	%rax, g_coroutine_running_ctx(%rip)
	movq	-16(%rbp), %rax
	addq	$64, %rax
	movq	%rax, %rdi
	call	list_is_suspend
	testl	%eax, %eax
	jne	.L63
	movl	$__PRETTY_FUNCTION__.4060, %ecx
	movl	$223, %edx
	movl	$.LC0, %esi
	movl	$.LC1, %edi
	call	__assert_fail
.L63:
	movq	-16(%rbp), %rax
	addq	$48, %rax
	movq	%rax, %rdi
	call	list_del
	movq	-16(%rbp), %rax
	movq	%rax, %rdi
	call	coroutine_ctx_free
	movq	-16(%rbp), %rax
	movq	(%rax), %rax
	movq	%rax, %rsi
	movl	$.LC17, %edi
	movl	$0, %eax
	call	printf
	cmpq	$0, -8(%rbp)
	je	.L64
	movq	-8(%rbp), %rax
	movl	$1, 8(%rax)
	movq	-8(%rbp), %rax
	movq	%rax, g_coroutine_running_ctx(%rip)
	movq	-8(%rbp), %rax
	addq	$64, %rax
	movq	%rax, %rdi
	call	list_is_suspend
	testl	%eax, %eax
	jne	.L65
	movq	-8(%rbp), %rax
	addq	$64, %rax
	movq	%rax, %rdi
	call	list_del
.L65:
	movq	g_exit_coroutine_ctx(%rip), %rax
	movq	-8(%rbp), %rdx
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	coroutine_sched_swap_context
.L64:
	movl	$0, %eax
	call	coroutine_sched
	jmp	.L66
	.cfi_endproc
.LFE44:
	.size	coroutine_exit, .-coroutine_exit
	.section	.rodata
	.align 16
	.type	__PRETTY_FUNCTION__.3997, @object
	.size	__PRETTY_FUNCTION__.3997, 16
__PRETTY_FUNCTION__.3997:
	.string	"coroutine_block"
	.align 16
	.type	__PRETTY_FUNCTION__.4024, @object
	.size	__PRETTY_FUNCTION__.4024, 16
__PRETTY_FUNCTION__.4024:
	.string	"coroutine_sched"
	.type	__PRETTY_FUNCTION__.4060, @object
	.size	__PRETTY_FUNCTION__.4060, 15
__PRETTY_FUNCTION__.4060:
	.string	"coroutine_exit"
	.ident	"GCC: (Ubuntu 4.8.2-19ubuntu1) 4.8.2"
	.section	.note.GNU-stack,"",@progbits
