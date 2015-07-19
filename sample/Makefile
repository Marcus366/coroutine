CC=gcc
CFLAGS=-Wall -g -pg -I../src/

LIBCOROUTINE=-L../objs/src/ -lcoroutine -ldl#-levent

MAKEDIR =mkdir -p
SAMPLE_OBJS_PATH=../objs/sample

.PHONY: all

all: $(SAMPLE_OBJS_PATH)				\
	 $(SAMPLE_OBJS_PATH)/echosvr		\
	 $(SAMPLE_OBJS_PATH)/simplehttpsvr

$(SAMPLE_OBJS_PATH):
	$(MAKEDIR) $(SAMPLE_OBJS_PATH)

$(SAMPLE_OBJS_PATH)/echosvr: echosvr.c ../objs/src/libcoroutine.a
	$(CC) $(CFLAGS) $< -o $@ $(LIBCOROUTINE)

$(SAMPLE_OBJS_PATH)/simplehttpsvr: simplehttpsvr.c ../objs/src/libcoroutine.a
	$(CC) $(CFLAGS) $< -o $@ $(LIBCOROUTINE)
