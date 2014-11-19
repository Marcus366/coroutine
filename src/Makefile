CC =gcc
CFLAGS =-Wall -g -D_GNU_SOURCE

AR =ar
ARFLAGS =rcs

BUILD_PATH = ../objs/src

SRCS = coroutine.c										\
			 hook.c

OBJS = $(BUILD_PATH)/coroutine.o			\
			 $(BUILD_PATH)/hook.o


$(BUILD_PATH)/%.o: %.c
	$(CC) $< $(CFLAGS) -c -o $@ -ldl

$(BUILD_PATH)/libcoroutine.a: $(OBJS)
	$(AR) $(ARFLAGS) $@ $(OBJS)
