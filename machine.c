#include "machine.h"
#include "decoder.h"
#include "args.h"
#include "ops.h"

void
machine_reset (Machine *machine)
{
  word font[] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, /* 0 */
    0x20, 0x60, 0x20, 0x20, 0x70, /* 1 */
    0xF0, 0x10, 0xF0, 0x80, 0xF0, /* 2 */
    0xF0, 0x10, 0xF0, 0x10, 0xF0, /* 3 */
    0x90, 0x90, 0xF0, 0x10, 0x10, /* 4 */
    0xF0, 0x80, 0xF0, 0x10, 0xF0, /* 5 */
    0xF0, 0x80, 0xF0, 0x90, 0xF0, /* 6 */
    0xF0, 0x10, 0x20, 0x40, 0x40, /* 7 */
    0xF0, 0x90, 0xF0, 0x90, 0xF0, /* 8 */
    0xF0, 0x90, 0xF0, 0x10, 0xF0, /* 9 */
    0xF0, 0x90, 0xF0, 0x90, 0x90, /* A */
    0xE0, 0x90, 0xE0, 0x90, 0xE0, /* B */
    0xF0, 0x80, 0x80, 0x80, 0xF0, /* C */
    0xE0, 0x90, 0x90, 0x90, 0xE0, /* D */
    0xF0, 0x80, 0xF0, 0x80, 0xF0, /* E */
    0xF0, 0x80, 0xF0, 0x80, 0x80  /* F */
  };

  machine->sp = SP_START;
  machine->pc = PROGRAM_START;
  machine->state = MACHINE_RUNNING;
  machine->i = 0;

  for (int i = 0; i < sizeof (font); i++)
    machine->memory[i] = font[i];

  for (int i = 0; i < REGISTERS_LENGTH; i++)
    machine->registers[i] = 0;

  cls (machine);
}

void
machine_step (Machine *machine, Options options)
{
  word high = machine->memory[machine->pc],
    low = machine->memory[machine->pc + 1], nibbles[4];

  if (machine->state != MACHINE_RUNNING)
    return;

  split_nibbles (nibbles, high, low);

  if (matches_op ("00E0", high, low))
    cls (machine);
  else if (matches_op ("00EE", high, low))
    ret (machine, nibbles);
  else if (matches_op ("1nnn", high, low))
    jp_addr (machine, nibbles);
  else if (matches_op ("2nnn", high, low))
    call_addr (machine, nibbles);
  else if (matches_op ("3xkk", high, low))
    se_vx_byte (machine, nibbles);
  else if (matches_op ("4xkk", high, low))
    sne_vx_byte (machine, nibbles);
  else if (matches_op ("5xy0", high, low))
    se_vx_vy (machine, nibbles);
  else if (matches_op ("6xkk", high, low))
    ld_vx_byte (machine, nibbles);
  else if (matches_op ("7xkk", high, low))
    add_vx_byte (machine, nibbles);
  else if (matches_op ("8xy0", high, low))
    ld_vx_vy (machine, nibbles);
  else if (matches_op ("8xy1", high, low))
    or_vx_vy (machine, nibbles, options.vf_reset_on);
  else if (matches_op ("8xy2", high, low))
    and_vx_vy (machine, nibbles, options.vf_reset_on);
  else if (matches_op ("8xy3", high, low))
    xor_vx_vy (machine, nibbles, options.vf_reset_on);
  else if (matches_op ("8xy4", high, low))
    add_vx_vy (machine, nibbles);
  else if (matches_op ("8xy5", high, low))
    sub_vx_vy (machine, nibbles);
  else if (matches_op ("8xy6", high, low))
    shr_vx_vy (machine, nibbles, options.vy_shifting_on);
  else if (matches_op ("8xy7", high, low))
    subn_vx_vy (machine, nibbles);
  else if (matches_op ("8xyE", high, low))
    shl_vx_vy (machine, nibbles, options.vy_shifting_on);
  else if (matches_op ("9xy0", high, low))
    sne_vx_vy (machine, nibbles);
  else if (matches_op ("Annn", high, low))
    ld_i_addr (machine, nibbles);
  else if (matches_op ("Bnnn", high, low))
    jp_v0_addr (machine, nibbles, options.vx_jump_on);
  else if (matches_op ("Cxkk", high, low))
    rnd_vx_byte (machine, nibbles);
  else if (matches_op ("Dxyn", high, low))
    drw_vx_vy_nibble (machine,
		      nibbles,
		      options.display_wait_on,
		      options.display_clipping_on);
  else if (matches_op ("Ex9E", high, low))
    skp_vx (machine, nibbles);
  else if (matches_op ("ExA1", high, low))
    sknp_vx (machine, nibbles);
  else if (matches_op ("Fx07", high, low))
    ld_vx_dt (machine, nibbles);
  else if (matches_op ("Fx0A", high, low))
    ld_vx_k (machine, nibbles);
  else if (matches_op ("Fx15", high, low))
    ld_dt_vx (machine, nibbles);
  else if (matches_op ("Fx18", high, low))
    ld_st_vx (machine, nibbles);
  else if (matches_op ("Fx1E", high, low))
    add_i_vx (machine, nibbles);
  else if (matches_op ("Fx29", high, low))
    ld_f_vx (machine, nibbles);
  else if (matches_op ("Fx33", high, low))
    ld_b_vx (machine, nibbles);
  else if (matches_op ("Fx55", high, low))
    ld_i_vx (machine, nibbles, options.index_increment_on);
  else if (matches_op ("Fx65", high, low))
    ld_vx_i (machine, nibbles, options.index_increment_on);
  else
    nop (machine, nibbles);

  /* TODO protect pc out of bounds */
}

void
machine_step_timers (Machine* machine)
{
  if (machine->delay > 0)
    machine->delay--;
    
  if (machine->sound > 0)
    machine->sound--;
}

void
machine_display_interrupt (Machine *machine)
{
  if (machine->state == MACHINE_WAITING_DSP_INTERRUPT)
    machine->state = MACHINE_RUNNING;
}

void
machine_keyboard_interrupt (Machine *machine, word key_up)
{
  if (machine->state != MACHINE_WAITING_KB_INTERRUPT) return;

  machine->registers[machine->key_register] = key_up;
  machine->state = MACHINE_RUNNING;
}
