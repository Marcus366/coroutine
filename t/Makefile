CC =clang
CFLAGS =-Wall -g -pg -I../src

LIBCOROUTINE=-L../objs/src/ -lcoroutine -ldl

MAKEDIR =mkdir -p
BUILD_PATH = ../objs/t

SRCS = test.c						\
	   test_coroutine_create.c		\
	   test_coroutine_resume.c

OBJS = $(BUILD_PATH)/test.o						\
	   $(BUILD_PATH)/test_coroutine_create.o		\
	   $(BUILD_PATH)/test_coroutine_resume.o

$(BUILD_PATH)/runtest: $(OBJS) $(BUILD_PATH) ../objs/src/libcoroutine.a
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LIBCOROUTINE)

$(BUILD_PATH)/%.o: %.c $(BUILD_PATH)
	$(CC) $< $(CFLAGS) -c -o $@

$(BUILD_PATH):
	$(MAKEDIR) $(BUILD_PATH)

