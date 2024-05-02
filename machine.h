/* This module contains definitions for the `machine` - the thing that
   is being emulated, as well as functions for interacting with it.
 */
#ifndef MACHINE_H
#define MACHINE_H

#include <stdbool.h>

#define MAX_PROGRAM_LENGTH 3584
#define PROGRAM_START 0x200
#define DISPLAY_LINES 32
#define DISPLAY_COLUMNS 64
#define SPRITE_WIDTH 8
#define STACK_SIZE 16
#define ADDRESS_MAX 0xFFF
#define WORD_MAX 0xFF

typedef struct Machine Machine;
typedef unsigned short address;
typedef unsigned char word;

struct Machine
{
  word memory[4096];
  word registers[16];
  word delay;
  word sound;
  signed char sp;
  address i;
  address pc;
  address stack[STACK_SIZE];
  bool display[DISPLAY_LINES][DISPLAY_COLUMNS];
};

/* Take a machine `machine` and do the necessary operations to
   complete one clock cycle.
 */
void machine_step (Machine* machine);

/* Take a machine `machine` and do the necessary operations to
   decrement all timers.
 */
void machine_step_timers (Machine* machine);

#endif
