CC=gcc
CFLAGS=-Wall -g

LIBCOROUTINE=-L../objs/src -lcoroutine

TEST_OBJS_PATH=../objs/test
TEST_SRC=test.c 							\
		 test_tcp_open.c					\
		 test_coroutine_create.c	\
		 test_coroutine_resume.c

.PHONY: all

all: $(TEST_OBJS_PATH)/test

$(TEST_OBJS_PATH)/test: $(TEST_SRC) ../objs/src/libcoroutine.a
	$(CC) $(CFLAGS) $(TEST_SRC) -o $@ $(LIBCOROUTINE)
