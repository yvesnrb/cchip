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
or_vx_vy (Machine *machine, word nibbles[4], bool vf_reset_on)
{
  word vx = nibbles[1], vy = nibbles[2], vx_v = machine->registers[vx],
    vy_v = machine->registers[vy], or;

  or = vx_v | vy_v;
  machine->registers[vx] = or;
  if (vf_reset_on) machine->registers[0xF] = 0;
  machine->pc += 2;
}

void
and_vx_vy (Machine *machine, word nibbles[4], bool vf_reset_on)
{
  word vx = nibbles[1], vy = nibbles[2], vx_v = machine->registers[vx],
    vy_v = machine->registers[vy], and;

  and = vx_v & vy_v;
  machine->registers[vx] = and;
  if (vf_reset_on) machine->registers[0xF] = 0;
  machine->pc += 2;
}

void
xor_vx_vy (Machine *machine, word nibbles[4], bool vf_reset_on)
{
  word vx = nibbles[1], vy = nibbles[2], vx_v = machine->registers[vx],
    vy_v = machine->registers[vy], xor;

  xor = vx_v ^ vy_v;
  machine->registers[vx] = xor;
  if (vf_reset_on) machine->registers[0xF] = 0;
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
shr_vx_vy (Machine *machine, word nibbles[4], bool vy_shifting_on)
{
  word vx = nibbles[1], vx_v = machine->registers[vx], vy = nibbles[2],
    vy_v = machine->registers[vy], operand;
    
  if (vy_shifting_on)
    operand = vy_v;
  else
    operand = vx_v;

  machine->registers[vx] = operand >> 1;

  if (operand & 0b00000001)
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
shl_vx_vy (Machine *machine, word nibbles[4], bool vy_shifting_on)
{
  word vx = nibbles[1], vx_v = machine->registers[vx], vy = nibbles[2],
    vy_v = machine->registers[vy], operand;

  if (vy_shifting_on)
    operand = vy_v;
  else
    operand = vx_v;
    
  machine->registers[vx] = operand << 1;

  if (operand & 0b10000000)
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
jp_v0_addr (Machine *machine, word nibbles[4], bool vx_jump_on)
{
  word v0_v = machine->registers[0x0], vx = nibbles[1],
    vx_v = machine->registers[vx], operand;
  address nnn = (nibbles[1] << 8) | (nibbles[2] << 4) | nibbles[3];

  if (vx_jump_on)
    operand = vx_v;
  else
    operand = v0_v;

  machine->pc = nnn + operand;
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
drw_vx_vy_nibble (Machine *machine,
		  word nibbles[4],
		  bool display_wait_on,
		  bool display_clipping_on)
{
  word vx = nibbles[1], vy = nibbles[2], n = nibbles[3],
    x = machine->registers[vx] % DISPLAY_COLUMNS,
    y = machine->registers[vy] % DISPLAY_LINES;
  address sprite_start = machine->i, sprite_end = machine->i + n;
  bool pixel, collision = false;

  for (address i = sprite_start; i < sprite_end; i++, y++)
    {
      if (y >= DISPLAY_LINES && display_clipping_on)
	break;
      else
	y %= DISPLAY_LINES;

      for (int j = 0, cur_x; j < SPRITE_WIDTH; j++)
	{
	  cur_x = x + j;

	  if (cur_x >= DISPLAY_COLUMNS && display_clipping_on)
	    break;
	  else
	    cur_x %= DISPLAY_COLUMNS;

	  pixel = (machine->memory[i] & (0b10000000 >> j)) != 0;

	  if (machine->display[y][cur_x] && pixel) collision = true;

	  machine->display[y][cur_x] ^= pixel;
	}
    }

  machine->registers[0xF] = collision;
  machine->pc += 2;
  if (display_wait_on)
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
ld_i_vx (Machine *machine, word nibbles[4], bool index_increment_on)
{
  word x = nibbles[1];
  address start = machine->i, end = start + x;

  for (int i = 0; machine->i <= end; i++, machine->i++)
    machine->memory[machine->i] = machine->registers[i];

  if (!index_increment_on) machine->i = start;

  machine->pc += 2;
}

void
ld_vx_i (Machine *machine, word nibbles[4], bool index_increment_on)
{
  word x = nibbles[1];
  address start = machine->i, end = start + x;

  for (int i = 0; machine->i <= end; i++, machine->i++)
    machine->registers[i] = machine->memory[machine->i];

  if (!index_increment_on) machine->i = start;
  
  machine->pc += 2;
}

void
nop (Machine *machine, word nibbles[4])
{
  machine->pc += 2;
}
