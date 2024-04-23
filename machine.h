#pragma once

#include <stdbool.h>

#define MAX_PROGRAM_LENGTH 3584
#define PROGRAM_START 0x200
#define DISPLAY_LINES 32
#define DISPLAY_COLUMNS 64
#define SPRITE_WIDTH 8

typedef struct Machine Machine;
typedef unsigned short address;
typedef unsigned char byte;

struct Machine
{
  byte memory[4096];
  byte registers[16];
  byte delay;
  byte sound;
  byte sp;
  address i;
  address pc;
  address stack[16];
  bool display[DISPLAY_LINES][DISPLAY_COLUMNS];
};

void step (Machine* machine);
