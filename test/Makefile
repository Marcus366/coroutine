CC =gcc
CFLAGS =-Wall -g

LIBCOROUTINE =-L../objs/src/ -lcoroutine -ldl

.PHONY: all

all: ../objs/test/switch_test
	../objs/test/switch_test

../objs/test/switch_test: switch_test.c 
	$(CC) $< -o $@ $(LIBCOROUTINE)
