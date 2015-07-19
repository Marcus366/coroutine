.PHONY: all libco test sample clean

all: libco sample

libco: $(SRC_OUT_DIR)
	$(MAKE) -f src/Makefile

#test: libco $(TEST_OUT_DIR)
#	(cd t && make)

sample: libco $(SAMPLE_OUT_DIR)
	$(MAKE) -f src/sample

clean:
	@rm -rf objs/
	@echo CLEAN DONE

