sources := main.c rom.c debug.c decoder.c machine.c ops.c sdl.c args.c stack.c buzzer.c
libs := $(shell pkg-config --libs --cflags SDL2)

chip: $(sources)
	cc $(libs) $(sources) -Wall -o chip

.PHONY: debug
debug: $(sources)
	cc $(libs) $(sources) -g -O0 -Wall -o chip

.PHONY: clean
clean:
	rm -fdr chip chip.dSYM
