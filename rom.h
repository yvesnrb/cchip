/* This module contains functions for handling CHIP-8 ROM files. */
#ifndef ROM_H
#define ROM_H

#include "machine.h"

/* Find a ROM file by the path `filename` and load it into the
   `machine` starting at PROGRAM_START. The program counter of
   `machine` is also set to PROGRAM_START.
 */
void load_rom (Machine *machine, char filename[]);

#endif
