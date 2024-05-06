/* This module contains definitions for the `machine` - the thing that
   is being emulated, as well as functions for interacting with it.
 */
#ifndef MACHINE_H
#define MACHINE_H

#include <stdbool.h>

#define MAX_PROGRAM_LENGTH 3584
#define PROGRAM_START 0x200
#define SP_START -1
#define DISPLAY_LINES 32
#define DISPLAY_COLUMNS 64
#define SPRITE_WIDTH 8
#define STACK_SIZE 16
#define ADDRESS_MAX 0xFFF
#define WORD_MAX 0xFF
#define FONT_SPRITE_LENGTH 5

typedef struct Machine Machine;
typedef enum machine_state machine_state;
typedef unsigned short address;
typedef unsigned char word;

enum machine_state {
  /* The machine has finished operation. */
  MACHINE_HALTED,
  /* The machine is currently executing. */
  MACHINE_RUNNING,
  /* The machine is waiting for the display to refresh before
     continuing. */
  MACHINE_WAITING_DSP_INTERRUPT,
  /* The machine is waiting for a key up event before continuing. */
  MACHINE_WAITING_KB_INTERRUPT
};

struct Machine
{
  word memory[4096];
  word registers[16];
  word delay;
  word sound;
  word key_register;
  signed char sp;
  address i;
  address pc;
  address stack[STACK_SIZE];
  bool display[DISPLAY_LINES][DISPLAY_COLUMNS];
  bool keypad[16];
  machine_state state;
};

/* Take a machine `machine` and reset all fields to their initial
   values. */
void machine_reset (Machine *machine);

/* Take a machine `machine` and do the necessary operations to
   complete one clock cycle.
 */
void machine_step (Machine *machine);

/* Take a machine `machine` and do the necessary operations to
   decrement all timers.
 */
void machine_step_timers (Machine *machine);

/* Take a machine `machine` and do the necessary operations to handle
   a display interrupt.
 */
void machine_display_interrupt (Machine *machine);

/* Take a machine `machine` and a key code `key_up` and do the
   necessary operations to handle a keyboard interrupt.
 */
void machine_keyboard_interrupt (Machine *machine, word key_up);

#endif
