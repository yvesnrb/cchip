#include "machine.h"
#include "decoder.h"
#include "ops.h"

void
machine_step (Machine *machine)
{
  word high = machine->memory[machine->pc],
    low = machine->memory[machine->pc + 1], nibbles[4];

  split_nibbles (nibbles, high, low);

  if (matches_op ("00E0", high, low))
    cls (machine, nibbles);
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
  else if (matches_op ("6xkk", high, low))
    ld_vx_byte (machine, nibbles);
  else if (matches_op ("7xkk", high, low))
    add_vx_byte (machine, nibbles);
  else if (matches_op ("8xy0", high, low))
    ld_vx_vy (machine, nibbles);
  else if (matches_op ("8xy1", high, low))
    or_vx_vy (machine, nibbles);
  else if (matches_op ("8xy2", high, low))
    and_vx_vy (machine, nibbles);
  else if (matches_op ("8xy3", high, low))
    xor_vx_vy (machine, nibbles);
  else if (matches_op ("8xy4", high, low))
    add_vx_vy (machine, nibbles);
  else if (matches_op ("8xy5", high, low))
    sub_vx_vy (machine, nibbles);
  else if (matches_op ("8xy6", high, low))
    shr_vx_vy (machine, nibbles);
  else if (matches_op ("8xy7", high, low))
    subn_vx_vy (machine, nibbles);
  else if (matches_op ("8xyE", high, low))
    shl_vx_vy (machine, nibbles);
  else if (matches_op ("9xy0", high, low))
    sne_vx_vy (machine, nibbles);
  else if (matches_op ("Annn", high, low))
    ld_i_addr (machine, nibbles);
  else if (matches_op ("Bnnn", high, low))
    jp_v0_addr (machine, nibbles);
  else if (matches_op ("Cxkk", high, low))
    rnd_vx_byte (machine, nibbles);
  else if (matches_op ("Dxyn", high, low))
    drw_vx_vy_nibble (machine, nibbles);
  else if (matches_op ("ExA1", high, low))
    sknp_vx (machine, nibbles);
  else if (matches_op ("Fx07", high, low))
    ld_vx_dt (machine, nibbles);
  else if (matches_op ("Fx15", high, low))
    ld_dt_vx (machine, nibbles);
  else if (matches_op ("Fx18", high, low))
    ld_st_vx (machine, nibbles);
  else if (matches_op ("Fx1E", high, low))
    add_i_vx (machine, nibbles);
  else if (matches_op ("Fx33", high, low))
    ld_b_vx (machine, nibbles);
  else if (matches_op ("Fx55", high, low))
    ld_i_vx (machine, nibbles);
  else if (matches_op ("Fx65", high, low))
    ld_vx_i (machine, nibbles);
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
