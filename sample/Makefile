CC=gcc
CFLAGS=-Wall -g -pg -I../src/

LIBCOROUTINE=-L../objs/src/ -lcoroutine -ldl#-levent

SAMPLE_OBJS_PATH=../objs/sample

.PHONY: all

all: $(SAMPLE_OBJS_PATH)/echosvr

$(SAMPLE_OBJS_PATH)/echosvr: echosvr.c ../objs/src/libcoroutine.a
	$(CC) $(CFLAGS) $< -o $@ $(LIBCOROUTINE)
