/* This module contains debug functions for use in development. */
#ifndef DEBUG_H
#define DEBUG_H

#include "machine.h"

#define HEXP_LINE_LENGTH 0x10
#define ASCII_PRINTABLE_START 32
#define ASCII_PRINTABLE_END 127

/* Print an array of words `bytes` in a hex dump format, starting at
   address `start` and ending at address `end`.
 */
void hex_print (word *bytes, address start, address end);

/* Print the display of `machine` to the terminal using a simple ASCII
   representation of each pixel.
 */
void display_print (Machine *machine);

#endif
