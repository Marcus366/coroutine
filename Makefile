CC =gcc
CFLAGS =-Wall -g -fsplit-stack

.PHONY: all libco test

all: libco test

libco:
	mkdir -p objs/src
	(cd src && make)

test:
	mkdir -p objs/test
	(cd test && make)
