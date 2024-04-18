#include <stdbool.h>

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
