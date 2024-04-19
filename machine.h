#pragma once
#include <stdbool.h>

#define MAX_PROGRAM_LENGTH 3584
#define PROGRAM_START 0x200

typedef struct machine machine;

struct machine
{
  unsigned char memory[4096];
  unsigned char registers[16];
  unsigned char delay;
  unsigned char sound;
  unsigned char sp;
  unsigned short i;
  unsigned short pc;
  unsigned short stack[16];
  bool display[32][64];
};
