.PHONY: all libco test

all: libco test

libco:
	mkdir -p objs/src
	(cd src && make)

test: libco
	mkdir -p objs/test
	(cd test && make)
