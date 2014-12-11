CC=gcc
CFLAGS=-Wall -O2 -pg

LIBCOROUTINE=-L../objs/src/ -lcoroutine -ldl

SAMPLE_OBJS_PATH=../objs/sample

.PHONY: all

all: $(SAMPLE_OBJS_PATH)/http_sample

$(SAMPLE_OBJS_PATH)/http_sample: http_sample.c ../objs/src/libcoroutine.a
	$(CC) $(CFLAGS) $< -o $@ $(LIBCOROUTINE)
