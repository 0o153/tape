include ../defaults.mk

all: malloc.bench tape.bench
	time -l ./malloc.bench
	time -l ./tape.bench

%.bench: %.o ../tape.o
	cc -g -o $@ $^

clean: 
	rm -f *.o *.bench

.PHONY: all clean