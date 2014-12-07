CC=gcc
CFLAGS=-Wall -g

LIBCOROUTINE=-L../objs/src/ -lcoroutine -ldl

TEST_OBJS_PATH=../objs/test

.PHONY: all

all: $(TEST_OBJS_PATH)/switch_test				\
	$(TEST_OBJS_PATH)/iowait_test

$(TEST_OBJS_PATH)/switch_test: switch_test.c ../objs/src/libcoroutine.a
	$(CC) $(CFLAGS) $< -o $@ $(LIBCOROUTINE)

$(TEST_OBJS_PATH)/iowait_test: iowait_test.c ../objs/src/libcoroutine.a
	$(CC) $(CFLAGS) $< -o $@ $(LIBCOROUTINE)
