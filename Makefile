.PHONY: all libco test

all: libco test sample

libco:
	mkdir -p objs/src
	(cd src && make)

test: libco
	mkdir -p objs/test
	(cd t && make)

sample: libco
	mkdir -p objs/sample
	(cd sample && make)
