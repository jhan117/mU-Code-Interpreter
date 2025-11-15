test:
	$(MAKE) -C test test

clean:
	$(MAKE) -C test clean

.PHONY: test clean