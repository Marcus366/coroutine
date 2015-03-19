	.text
	.globl	coroutine__swap_context
	.type	coroutine__swap_context, @function
coroutine__swap_context:
#APP
# 7 "swap_context.s" 1
  pushq %rbp
	pushq %r12
  pushq %r13
  pushq %r14
  pushq %r15
  pushq %rbx

	movq %rsp, (%rdi)
	movq (%rsi), %rsp

  popq %rbx
  popq %r15
	popq %r14
  popq %r13
  popq %r12
	popq %rbp

#NO_APP
	ret
