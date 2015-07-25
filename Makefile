.PHONY: all libco test sample clean

all: libco sample

libco:
	@(cd src && make)

test: libco
	@(cd t && make)

sample: libco
	@(cd sample && make)

clean:
	@rm -rf objs/
	@echo CLEAN DONE

