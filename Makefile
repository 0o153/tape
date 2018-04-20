include ./defaults.mk

all: tape.o
	@:

clean:
	rm -f *.o
	cd bench && make clean

bench: all
	cd bench && make

.PHONY: all bench clean