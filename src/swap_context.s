# 1 "swap_context.S"
# 1 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 1 "<command-line>" 2
# 1 "swap_context.S"
.text
.globl coroutine__swap_context
.type coroutine__swap_context, @function
coroutine__swap_context:
  pushq %rax
  pushq %rbp
  pushq %rbx
  pushq %r12
  pushq %r13
  pushq %r14
  pushq %r15

  movq %rsp, (%rdi)
  movq (%rsi), %rsp

  popq %r15
  popq %r14
  popq %r13
  popq %r12
  popq %rbx
  popq %rbp
  popq %rax

  ret
