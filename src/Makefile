CC =gcc
CFLAGS =-Wall -g -pg

AR =ar
ARFLAGS =rcs

BUILD_PATH = ../objs/src

SRCS = coroutine.c										\
			 utils.c												\
			 pool.c													\
			 context.c											\
			 swap_context.s									\
			 sched.c												\
			 io.c														\
			 pipe.c													\
			 tcp.c

OBJS = $(BUILD_PATH)/coroutine.o			\
			 $(BUILD_PATH)/utils.o					\
			 $(BUILD_PATH)/pool.o						\
			 $(BUILD_PATH)/context.o				\
			 $(BUILD_PATH)/swap_context.o		\
			 $(BUILD_PATH)/sched.o					\
			 $(BUILD_PATH)/io.o							\
			 $(BUILD_PATH)/pipe.o						\
			 $(BUILD_PATH)/tcp.o


$(BUILD_PATH)/%.o: %.c config.h
	$(CC) $< $(CFLAGS) -c -o $@

$(BUILD_PATH)/%.o: swap_context.s
	$(CC) $< $(CFLAGS) -c -o $@

$(BUILD_PATH)/libcoroutine.a: $(OBJS)
	$(AR) $(ARFLAGS) $@ $(OBJS)
