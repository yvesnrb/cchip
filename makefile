sources := main.c rom.c debug.c decoder.c machine.c ops.c

chip: $(sources)
	cc $(sources) -o chip

.PHONY: debug
debug: $(sources)
	cc $(sources) -g -O0 -o chip

.PHONY: clean
clean:
	rm -fdr chip chip.dSYM
