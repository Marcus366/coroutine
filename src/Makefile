CC =gcc
CFLAGS =-Wall -O2 -pg -D_GNU_SOURCE

AR =ar
ARFLAGS =rcs

BUILD_PATH = ../objs/src

SRCS = coroutine.c										\
			 hook.c													\
			 utils.c												\
			 context.o											\
			 sched.o												\
			 waitqueue.o

OBJS = $(BUILD_PATH)/coroutine.o			\
			 $(BUILD_PATH)/hook.o						\
			 $(BUILD_PATH)/utils.o					\
			 $(BUILD_PATH)/context.o				\
			 $(BUILD_PATH)/sched.o					\
			 $(BUILD_PATH)/waitqueue.o


$(BUILD_PATH)/%.o: %.c config.h
	$(CC) $< $(CFLAGS) -c -o $@ -ldl

$(BUILD_PATH)/libcoroutine.a: $(OBJS)
	$(AR) $(ARFLAGS) $@ $(OBJS)
