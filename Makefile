CC =gcc
CFLAGS =-Wall -g -fsplit-stack

.PHONY: all libco test


test/main: test/main.c
	$(CC) src/coroutine.c src/hook.c test/main.c -I../src $(CFLAGS) -o test/main
	test/main
