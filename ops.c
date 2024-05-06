#include <stdio.h>
#include <limits.h>
#include <stdbool.h>
#include <stdlib.h>
#include "ops.h"
#include "machine.h"
#include "stack.h"

void
cls (Machine *machine, word nibbles[4])
{
  for (int i = 0; i < DISPLAY_LINES; i++)
    for (int j = 0; j < DISPLAY_COLUMNS; j++)
      machine->display[i][j] = false;

  machine->pc += 2;
}

void
ret (Machine *machine, word nibbles[4])
{
  machine->pc = stack_pop (machine->stack, &machine->sp);
  machine->pc += 2;
}

void
jp_addr (Machine *machine, word nibbles[4])
{
  address nnn = (nibbles[1] << 8) | (nibbles[2] << 4) | nibbles[3];

  machine->pc = nnn;
}

void
call_addr (Machine *machine, word nibbles[4])
{
  address nnn = (nibbles[1] << 8) | (nibbles[2] << 4) | nibbles[3];

  stack_push (machine->stack, &machine->sp, machine->pc);
  machine->pc = nnn;
}

void
se_vx_byte (Machine *machine, word nibbles[4])
{
  word vx = nibbles[1], kk = (nibbles[2] << 4) | (nibbles[3]),
    vx_v = machine->registers[vx];

  if (vx_v == kk)
    machine->pc += 4;
  else
    machine->pc += 2;
}

void
sne_vx_byte (Machine *machine, word nibbles[4])
{
  word vx = nibbles[1], kk = (nibbles[2] << 4) | (nibbles[3]),
    vx_v = machine->registers[vx];

  if (vx_v != kk)
    machine->pc += 4;
  else
    machine->pc += 2;
}

void
se_vx_vy (Machine *machine, word nibbles[4])
{
  word vx = nibbles[1], vy = nibbles[2], vx_v = machine->registers[vx],
    vy_v = machine->registers[vy];

  if (vx_v == vy_v)
    machine->pc += 4;
  else
    machine->pc += 2;
}

void
ld_vx_byte (Machine *machine, word nibbles[4])
{
  unsigned char vx = nibbles[1], kk = (nibbles[2] << 4) | nibbles[3];

  machine->registers[vx] = kk;
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
ld_vx_vy (Machine *machine, word nibbles[4])
{
  word vx = nibbles[1], vy = nibbles[2];

  machine->registers[vx] = machine->registers[vy];
  machine->pc += 2;
}

void
or_vx_vy (Machine *machine, word nibbles[4])
{
  word vx = nibbles[1], vy = nibbles[2], vx_v = machine->registers[vx],
    vy_v = machine->registers[vy], or;

  or = vx_v | vy_v;
  machine->registers[vx] = or;
  machine->registers[0xF] = 0;
  machine->pc += 2;
}

void
and_vx_vy (Machine *machine, word nibbles[4])
{
  word vx = nibbles[1], vy = nibbles[2], vx_v = machine->registers[vx],
    vy_v = machine->registers[vy], and;

  and = vx_v & vy_v;
  machine->registers[vx] = and;
  machine->registers[0xF] = 0;
  machine->pc += 2;
}

void
xor_vx_vy (Machine *machine, word nibbles[4])
{
  word vx = nibbles[1], vy = nibbles[2], vx_v = machine->registers[vx],
    vy_v = machine->registers[vy], xor;

  xor = vx_v ^ vy_v;
  machine->registers[vx] = xor;
  machine->registers[0xF] = 0;
  machine->pc += 2;
}

void
add_vx_vy (Machine *machine, word nibbles[4])
{
  word vx = nibbles[1], vy = nibbles[2], vx_v = machine->registers[vx],
    vy_v = machine->registers[vy];
  unsigned short sum = vx_v + vy_v;

  machine->registers[vx] = (word) sum;

  if (sum > WORD_MAX)
    machine->registers[0xF] = 1;
  else
    machine->registers[0xF] = 0;

  machine->pc += 2;
}

void
sub_vx_vy (Machine *machine, word nibbles[4])
{
  word vx = nibbles[1], vy = nibbles[2], vx_v = machine->registers[vx],
    vy_v = machine->registers[vy];

  machine->registers[vx] = vx_v - vy_v;

  if (vx_v >= vy_v)
    machine->registers[0xF] = 1;
  else
    machine->registers[0xF] = 0;

  machine->pc += 2;
}

void
shr_vx_vy (Machine *machine, word nibbles[4])
{
  word vx = nibbles[1], vy = nibbles[2], vy_v = machine->registers[vy];
    
  machine->registers[vx] = vy_v >> 1;

  if (vy_v & 0b00000001)
    machine->registers[0xF] = 1;
  else
    machine->registers[0xF] = 0;

  machine->pc += 2;
}

void
subn_vx_vy (Machine *machine, word nibbles[4])
{
  word vx = nibbles[1], vy = nibbles[2], vx_v = machine->registers[vx],
    vy_v = machine->registers[vy];

  machine->registers[vx] = vy_v - vx_v;

  if (vy_v >= vx_v)
    machine->registers[0xF] = 1;
  else
    machine->registers[0xF] = 0;

  machine->pc += 2;
}

void
shl_vx_vy (Machine *machine, word nibbles[4])
{
  word vx = nibbles[1], vy = nibbles[2], vy_v = machine->registers[vy];
    
  machine->registers[vx] = vy_v << 1;

  if (vy_v & 0b10000000)
    machine->registers[0xF] = 1;
  else
    machine->registers[0xF] = 0;

  machine->pc += 2;
}

void
sne_vx_vy (Machine *machine, word nibbles[4])
{
  word vx = nibbles[1], vy = nibbles[2], vx_v = machine->registers[vx],
    vy_v = machine->registers[vy];

  if (vx_v != vy_v)
    machine->pc += 4;
  else
    machine->pc += 2;
}

void
ld_i_addr (Machine *machine, word nibbles[4])
{
  address nnn = (nibbles[1] << 8) | (nibbles[2] << 4) | nibbles[3];

  machine->i = nnn;
  machine->pc += 2;
}

void
jp_v0_addr (Machine *machine, word nibbles[4])
{
  word v0_v = machine->registers[0x0];
  address nnn = (nibbles[1] << 8) | (nibbles[2] << 4) | nibbles[3];

  machine->pc = nnn + v0_v;
}

void
rnd_vx_byte (Machine *machine, word nibbles[4])
{
  word vx = nibbles[1], kk = (nibbles[2] << 4) | (nibbles[3]),
    rnd = rand () % 256;

  machine->registers[vx] = rnd & kk;
  machine->pc += 2;
}

void
drw_vx_vy_nibble (Machine *machine, word nibbles[4])
{
  word vx = nibbles[1], vy = nibbles[2], n = nibbles[3],
    x = machine->registers[vx] % DISPLAY_COLUMNS,
    y = machine->registers[vy] % DISPLAY_LINES;
  address sprite_start = machine->i, sprite_end = machine->i + n;
  bool pixel, collision = false;

  for (address i = sprite_start; i < sprite_end; i++)
    {
      if (y >= DISPLAY_LINES) break;

      for (int j = 0; j < SPRITE_WIDTH; j++)
	{
	  if ((x + j) >= DISPLAY_COLUMNS) break;
	  pixel = (machine->memory[i] & (0b10000000 >> j)) != 0;

	  if (machine->display[y][x + j] && pixel) collision = true;
	  machine->display[y][x + j] ^= pixel;
	}

      y++;
    }

  machine->registers[0xF] = collision;
  machine->pc += 2;
  machine->state = MACHINE_WAITING_DSP_INTERRUPT;
}

void
skp_vx (Machine *machine, word nibbles[4])
{
  word vx = nibbles[1], vx_v = machine->registers[vx];

  if (machine->keypad[vx_v])
    machine->pc += 4;
  else
    machine->pc += 2;
}

void
sknp_vx (Machine *machine, word nibbles[4])
{
  word vx = nibbles[1], vx_v = machine->registers[vx];  

  if (machine->keypad[vx_v])
    machine->pc += 2;
  else
    machine->pc += 4;
}

void
ld_vx_dt (Machine *machine, word nibbles[4])
{
  word vx = nibbles[1];

  machine->registers[vx] = machine->delay;
  machine->pc += 2;
}

void
ld_vx_k (Machine *machine, word nibbles[4])
{
  word vx = nibbles[1];

  machine->key_register = vx;
  machine->state = MACHINE_WAITING_KB_INTERRUPT;
  machine->pc += 2;
}

void
ld_dt_vx (Machine *machine, word nibbles[4])
{
  word vx = nibbles[1];

  machine->delay = machine->registers[vx];
  machine->pc += 2;
}

void
ld_st_vx (Machine *machine, word nibbles[4])
{
  word vx = nibbles[1];

  machine->sound = machine->registers[vx];
  machine->pc += 2;
}

void
add_i_vx (Machine *machine, word nibbles[4])
{
  word vx = nibbles[1], vx_v = machine->registers[vx];

  machine->i += vx_v;
  machine->pc += 2;
}

void
ld_f_vx (Machine *machine, word nibbles[4])
{
  word vx = nibbles[1], vx_v = machine->registers[vx];
  address font_adr = vx_v * FONT_SPRITE_LENGTH;

  machine->i = font_adr;
  machine->pc += 2;
}

void
ld_b_vx (Machine *machine, word nibbles[4])
{
  word vx = nibbles[1], vx_v = machine->registers[vx], hundreds, tens,
    ones;
  address start = machine->i;

  hundreds = (vx_v / 100) % 10;
  tens = (vx_v / 10) % 10;
  ones = vx_v % 10;
  
  machine->memory[start] = hundreds;
  machine->memory[start + 1] = tens;
  machine->memory[start + 2] = ones;
  machine->pc += 2;
}

void
ld_i_vx (Machine *machine, word nibbles[4])
{
  word x = nibbles[1];
  address start = machine->i;
  int i = 0;

  while (machine->i <= (start + x))
    {
      machine->memory[machine->i] = machine->registers[i];
      i++;
      machine->i++;
    }

  machine->pc += 2;
}

void
ld_vx_i (Machine *machine, word nibbles[4])
{
  word x = nibbles[1];
  address start = machine->i;
  int i = 0;

  while (machine->i <= (start + x))
    {
      machine->registers[i] = machine->memory[machine->i];
      i++;
      machine->i++;
    }

  machine->pc += 2;
}

void
nop (Machine *machine, word nibbles[4])
{
  machine->pc += 2;
}
