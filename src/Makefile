CC =gcc
CFLAGS =-Wall -g -pg

AR =ar
ARFLAGS =rcs

MAKEDIR =mkdir -p
BUILD_PATH = ../objs/src

SRCS = context.c				\
			 coroutine.c		\
			 io.c				\
			 log.c				\
			 pipe.c				\
			 pool.c				\
			 sched.c			\
			 stack.c			\
			 swap_context.s		\
			 tcp.c				\
			 utils.c

OBJS = $(BUILD_PATH)/context.o					\
			 $(BUILD_PATH)/coroutine.o			\
			 $(BUILD_PATH)/io.o					\
			 $(BUILD_PATH)/log.o				\
			 $(BUILD_PATH)/pipe.o				\
			 $(BUILD_PATH)/pool.o				\
			 $(BUILD_PATH)/sched.o				\
			 $(BUILD_PATH)/stack.o				\
			 $(BUILD_PATH)/swap_context.o		\
			 $(BUILD_PATH)/tcp.o				\
			 $(BUILD_PATH)/utils.o

$(BUILD_PATH)/%.o: %.c $(BUILD_PATH)
	$(CC) $< $(CFLAGS) -c -o $@

$(BUILD_PATH)/%.o: swap_context.s $(BUILD_PATH)
	$(CC) $< $(CFLAGS) -c -o $@

$(BUILD_PATH)/libcoroutine.a: $(OBJS) $(BUILD_PATH)
	$(AR) $(ARFLAGS) $@ $(OBJS)

$(BUILD_PATH):
	$(MAKEDIR) $(BUILD_PATH)


