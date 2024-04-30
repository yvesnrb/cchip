#include <stdbool.h>
#include "ops.h"
#include "machine.h"

void
cls (Machine *machine, word nibbles[4])
{
  for (int i = 0; i < DISPLAY_LINES; i++)
    for (int j = 0; j < DISPLAY_COLUMNS; j++)
      machine->display[i][j] = false;

  machine->pc += 2;
}

void
jp_addr (Machine *machine, word nibbles[4])
{
  unsigned short nnn = (nibbles[1] << 8) | (nibbles[2] << 4) | nibbles[3];

  machine->pc = nnn;
}

void
ld_vx_byte (Machine *machine, word nibbles[4])
{
  unsigned char vx = nibbles[1], kk = (nibbles[2] << 4) | nibbles[3];

  machine->registers[vx] = kk;
  machine->pc += 2;
}

void
ld_i_addr (Machine *machine, word nibbles[4])
{
  unsigned short nnn = (nibbles[1] << 8) | (nibbles[2] << 4) | nibbles[3];

  machine->i = nnn;
  machine->pc += 2;
}

void
drw_vx_vy_nibble (Machine *machine, word nibbles[4])
{
  word vx = nibbles[1], vy = nibbles[2], n = nibbles[3],
    x = machine->registers[vx], y = machine->registers[vy], sprite_byte;
  address sprite_start = machine->i, sprite_end = machine->i + n;
  bool sprite_pixel, collision = false;

  for (address i = sprite_start; i < sprite_end; i++)
    {
      sprite_byte = machine->memory[i];

      for (int j = 0; j < SPRITE_WIDTH; j++)
	{
	  sprite_pixel = (sprite_byte & (0b10000000 >> j)) != 0;

	  if (machine->display[y][x + j] && sprite_pixel)
	    collision = true;

	  machine->display[y][x + j] ^= sprite_pixel;
	}

      if (y < DISPLAY_LINES) y++;
    }

  machine->registers[0xF] = collision;
  machine->pc += 2;
}

void
add_vx_byte (Machine *machine, word nibbles[4])
{
  word vx = nibbles[1], kk = (nibbles[2] << 4) | (nibbles[3]);

  machine->registers[vx] += kk;
  machine->pc += 2;
}

void
nop (Machine *machine, word nibbles[4])
{
  machine->pc += 2;
}
