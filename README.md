coroutine
========================================
Coroutine is a cooperative multitasking based network library in pure C.

This library mainly focus on network programming. It has the same performance compared to event-driven model with *more graceful code organization*.

Status
----------------------------------------
The project is under development. Any issue or pull request is welcomed.

How to Use?
----------------------------------------
Interfaces are all easy to understand and you can read the document in [coroutine.h](https://github.com/sysu2012zzp/coroutine/blob/master/src/coroutine.h)

We regard a stream of instruction flow as *coroutine*. The only differences between coroutine and thread are that coroutine is cooperative while thread is preemitive.
