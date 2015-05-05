CC=gcc
CFLAGS=-Wall -g

LIBCOROUTINE=-L../objs/src -lcoroutine

TEST_OBJS_PATH=../objs/test
TEST_SRC=test.c 							\
		 test_tcp_open.c					\
		 test_coroutine_create.c	\
		 test_coroutine_resume.c

.PHONY: all

all: $(TEST_OBJS_PATH)/test $(TEST_OBJS_PATH)/test_resume \
     $(TEST_OBJS_PATH)/test_pool

$(TEST_OBJS_PATH)/test: $(TEST_SRC) ../objs/src/libcoroutine.a
	$(CC) $(CFLAGS) $(TEST_SRC) -o $@ $(LIBCOROUTINE)

$(TEST_OBJS_PATH)/test_resume: test_resume.c ../objs/src/libcoroutine.a
	$(CC) $(CFLAGS) test_resume.c -o $@ $(LIBCOROUTINE)

$(TEST_OBJS_PATH)/test_pool: test_pool.c ../objs/src/libcoroutine.a
	$(CC) $(CFLAGS) test_pool.c -o $@ $(LIBCOROUTINE)
