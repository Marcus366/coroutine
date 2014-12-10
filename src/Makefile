CC =gcc
CFLAGS =-Wall -g -D_GNU_SOURCE

AR =ar
ARFLAGS =rcs

BUILD_PATH = ../objs/src

SRCS = coroutine.c										\
			 hook.c													\
			 cidmap.c												\
			 utils.c												\
			 context.o											\
			 sched.o

OBJS = $(BUILD_PATH)/coroutine.o			\
			 $(BUILD_PATH)/hook.o						\
			 $(BUILD_PATH)/cidmap.o					\
			 $(BUILD_PATH)/utils.o					\
			 $(BUILD_PATH)/context.o				\
			 $(BUILD_PATH)/sched.o


$(BUILD_PATH)/%.o: %.c config.h
	$(CC) $< $(CFLAGS) -c -o $@ -ldl

$(BUILD_PATH)/libcoroutine.a: $(OBJS)
	$(AR) $(ARFLAGS) $@ $(OBJS)
