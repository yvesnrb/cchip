#include <stdio.h>
#include <stdlib.h>
#include "machine.h"

int
main (void)
{
  machine* chip = (machine*) malloc (sizeof (machine));

  chip->i = 0b1111111111111111;
  chip->registers[0x0] = 0xff;
  chip->registers[0x1] = 0xaa;
  chip->registers[0x2] = 0xbb;
  chip->registers[0xf] = 0xcc;

  chip->memory[0x000] = 0x11;
  chip->memory[0xFFF] = 0x99;

  chip->display[0][0] = true;
  chip->display[31][63] = false;

  printf ("register i: %X\n", chip->i);
  printf ("register 0x0: %X\n", chip->registers[0x0]);
  printf ("register 0x1: %X\n", chip->registers[0x1]);
  printf ("register 0x2: %X\n", chip->registers[0x2]);
  printf ("register 0xF: %X\n", chip->registers[0xF]);

  printf ("first memory address: %X\n", chip->memory[0x000]);
  printf ("second memory address: %X\n", chip->memory[0x001]);
  printf ("last memory address: %X\n", chip->memory[0xFFF]);

  printf ("first pixel of the display is: %s\n", chip->display[0][0] ? "on" : "off");
  printf ("last pixel of the display is: %s\n", chip->display[31][63] ? "on" : "off");

  free (chip);
  return 0;
}
