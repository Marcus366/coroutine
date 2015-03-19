.text
.globl	coroutine__swap_context
.type	coroutine__swap_context, @function
coroutine__swap_context:
  pushq %r12
  pushq %r13
  pushq %r14
  pushq %r15
  pushq %rbx
  pushq %rbp

  movq %rsp, 160(%rdi)
  movq 160(%rsi), %rsp

  popq %rbp
  popq %rbx
  popq %r15
  popq %r14
  popq %r13
  popq %r12

  ret

