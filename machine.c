#include "machine.h"
#include "decoder.h"
#include "ops.h"

void
step (Machine *machine)
{
  byte high = machine->memory[machine->pc],
    low = machine->memory[machine->pc + 1], nibbles[4];

  split_nibbles (nibbles, high, low);

  if (matches_op ("00E0", high, low))
    cls (machine, nibbles);
  else if (matches_op ("6xkk", high, low))
    ld_vx_byte (machine, nibbles);
  else if (matches_op ("Annn", high, low))
    ld_i_addr (machine, nibbles);
  else if (matches_op ("Dxyn", high, low))
    drw_vx_vy_nibble (machine, nibbles);
  else if (matches_op ("1nnn", high, low))
    jp_addr (machine, nibbles);
  else if (matches_op ("7xkk", high, low))
    add_vx_byte (machine, nibbles);
  else
    nop (machine, nibbles);

  /* TODO protect pc out of bounds */
}
