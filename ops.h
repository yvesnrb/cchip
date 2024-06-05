/* This module contains functions for executing each operation on the
   CHIP-8 instruction set.
 */
#ifndef OPS_H
#define OPS_H

#include <stdbool.h>
#include "machine.h"

/* 00E0 - Clear the display. */
void cls (Machine *machine);

/* 00EE - Return from subrotine.
   The interpreter sets the program counter to the address at the top
   of the stack, then subtracts 1 from the stack pointer.
 */
void ret (Machine *machine, word nibbles[4]);

/* 1nnn - Jump to location nnn.
   The interpreter sets the program counter to nnn.
 */
void jp_addr (Machine *machine, word nibbles[4]);

/* 2nnn - Call subrotine at nnn.
   The interpreter increments the stack pointer, then puts the current
   PC on the top of the stack. The PC is then set to nnn.
 */
void call_addr (Machine *machine, word nibbles[4]);

/* 3xkk - Skip next instruction if vx = kk.
   The interpreter compares register Vx to kk, and if they are equal,
   increments the program counter by 2.
 */
void se_vx_byte (Machine *machine, word nibbles[4]);

/* 4xkk - Skip next instruction if vx != kk.
   The interpreter compares register Vx to kk, and if they are not
   equal, increments the program counter by 2.
 */
void sne_vx_byte (Machine *machine, word nibbles[4]);

/* 5xy0 - Skip next instruction if vx = vy.
   The interpreter compares register Vx to register Vy, and if they
   are equal, increments the program counter by 2.
 */
void se_vx_vy (Machine *machine, word nibbles[4]);

/* 6xkk - Set vx = kk.
   The interpreter puts the value kk into register Vx.
 */
void ld_vx_byte (Machine *machine, word nibbles[4]);

/* 7xkk - Set vx = vx + kk.
   Adds the value kk to the value of register Vx, then stores the
   result in Vx.
 */
void add_vx_byte (Machine *machine, word nibbles[4]);

/* 8xy0 - Set vx = vy.
   Stores the value of register Vy in register Vx.
 */
void ld_vx_vy (Machine *machine, word nibbles[4]);

/* 8xy1 - Set vx = vx OR vy.
   
   Performs a bitwise OR on the values of Vx and Vy, then stores the
   result in Vx. A bitwise OR compares the corrseponding bits from two
   values, and if either bit is 1, then the same bit in the result is
   also 1. If `vf_reset_on` is set to non-zero, this operation always
   resets the flag register.
 */
void or_vx_vy (Machine *machine, word nibbles[4], bool vf_reset_on);

/* 8xy2 - Set Vx = Vx AND Vy.
   
   Performs a bitwise AND on the values of Vx and Vy, then stores the
   result in Vx. A bitwise AND compares the corrseponding bits from
   two values, and if both bits are 1, then the same bit in the result
   is also 1. Otherwise, it is 0. If `vf_reset_on` is set to non-zero,
   this operation always resets the flag register.
 */
void and_vx_vy (Machine *machine, word nibbles[4], bool vf_reset_on);

/* 8xy3 - Set Vx = Vx XOR Vy.

   Performs a bitwise exclusive OR on the values of Vx and Vy, then
   stores the result in Vx. An exclusive OR compares the corrseponding
   bits from two values, and if the bits are not both the same, then
   the corresponding bit in the result is set to 1. Otherwise, it is
   0. If `vf_reset_on` is set to non-zero, this operation always
   resets the flag register.
 */
void xor_vx_vy (Machine *machine, word nibbles[4], bool vf_reset_on);

/* 8xy4 - Set Vx = Vx + Vy, set VF = carry.
   The values of Vx and Vy are added together. If the result is
   greater than 8 bits (i.e., > 255,) VF is set to 1, otherwise
   0. Only the lowest 8 bits of the result are kept, and stored in
   Vx.
 */
void add_vx_vy (Machine *machine, word nibbles[4]);

/* 8xy5 - Set Vx = Vx - Vy, set VF = NOT borrow.
   If Vx > Vy, then VF is set to 1, otherwise 0. Then Vy is subtracted
   from Vx, and the results stored in Vx.
 */
void sub_vx_vy (Machine *machine, word nibbles[4]);

/* 8xy6 - Set Vx = Vy SHR 1.
   The value of register Vx is set to Vy right shifted once. If the
   least-significant bit of Vy is 1, then VF is set to 1, otherwise 0.
   If `vy_shifting_on` is set to non-zero, this operation works only
   with the value of Vx - Vy is ignored.
 */
void shr_vx_vy (Machine *machine, word nibbles[4], bool vy_shifting_on);

/* 8xy7 - Set Vx = Vy - Vx, set VF = NOT borrow.
   If Vy > Vx, then VF is set to 1, otherwise 0. Then Vx is subtracted
   from Vy, and the results stored in Vx.
 */
void subn_vx_vy (Machine *machine, word nibbles[4]);

/* 8xyE - Set Vx = Vy SHL 1.
   The value of register Vx is set to Vy left shifted once. If the
   most-significant bit of Vy is 1, then VF is set to 1, otherwise to
   0. If `vy_shifting_on` is set to non-zero, this operation works
   only with the value of Vx - Vy is ignored.
 */
void shl_vx_vy (Machine *machine, word nibbles[4], bool vy_shifting_on);

/* 9xy0 - Skip next instruction if Vx != Vy.
   The values of Vx and Vy are compared, and if they are not equal,
   the program counter is increased by 2.
 */
void sne_vx_vy (Machine *machine, word nibbles[4]);

/* Annn - Set I = nnn.
   The value of register I is set to nnn.
 */
void ld_i_addr (Machine *machine, word nibbles[4]);

/* Bnnn - Jump to location nnn + V0.
   The program counter is set to nnn plus the value of V0. If
   `vx_jump_on` is set to non-zero, use the highest nibble of nnn as
   Vx instead of V0.
 */
void jp_v0_addr (Machine *machine, word nibbles[4], bool vx_jump_on);

/* Cxkk - Set Vx = random byte AND kk.
   The interpreter generates a random number from 0 to 255, which is
   then ANDed with the value kk. The results are stored in Vx. See
   instruction 8xy2 for more information on AND.
 */
void rnd_vx_byte (Machine *machine, word nibbles[4]);

/* Dxyn - Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision.

   The interpreter reads n bytes from memory, starting at the address
   stored in I. These bytes are then displayed as sprites on screen at
   coordinates (Vx, Vy). Sprites are XORed onto the existing
   screen. If this causes any pixels to be erased, VF is set to 1,
   otherwise it is set to 0. See instruction 8xy3 for more information
   on XOR. If `display_wait_on` is set to non-zero, the interpreter
   must wait for the next display interrupt before continuing. If
   `display_clipping_on` is set to non-zero, sprites draw near the
   edges of the screen get clipped instead of wrapping to the opposite
   side.
 */
void drw_vx_vy_nibble (Machine *machine,
		       word nibbles[4],
		       bool display_wait_on,
		       bool display_clipping_on);

/* Ex9E - Skip next instruction if key with the value of Vx is pressed.
   Checks the keyboard, and if the key corresponding to the value of Vx
   is currently in the down position, PC is increased by 2.
 */
void skp_vx (Machine *machine, word nibbles[4]);

/* ExA1 - Skip next instruction if key with the value of Vx is not pressed.
   Checks the keyboard, and if the key corresponding to the value of
   Vx is currently in the up position, PC is increased by 2.
 */
void sknp_vx (Machine *machine, word nibbles[4]);

/* Fx07 - Set Vx = delay timer value.
   The value of DT is placed into Vx.
 */
void ld_vx_dt (Machine *machine, word nibbles[4]);

/* Fx0A - Wait for a key press, store the value of the key in Vx.
   All execution stops until a key is pressed, then the value of that
   key is stored in Vx.
 */
void ld_vx_k (Machine *machine, word nibbles[4]);

/* Fx15 - Set delay timer = Vx.
   DT is set equal to the value of Vx.
 */
void ld_dt_vx (Machine *machine, word nibbles[4]);

/* Fx18 - Set sound timer = Vx.
   ST is set equal to the value of Vx.
 */
void ld_st_vx (Machine *machine, word nibbles[4]);

/* Fx1E - Set I = I + Vx.
   The values of I and Vx are added, and the results are stored in I.
 */
void add_i_vx (Machine *machine, word nibbles[4]);

/* Fx29 - Set I = location of sprite for digit Vx.
   The value of I is set to the location for the hexadecimal sprite
   corresponding to the value of Vx.
*/
void ld_f_vx (Machine *machine, word nibbles[4]);

/* Fx33 - Store BCD representation of Vx in memory locations I, I+1, and I+2.
   The interpreter takes the decimal value of Vx, and places the
   hundreds digit in memory at location in I, the tens digit at
   location I+1, and the ones digit at location I+2.
 */
void ld_b_vx (Machine *machine, word nibbles[4]);

/* Fx55 - Store registers V0 through Vx in memory starting at location I.

   The interpreter copies the values of registers V0 through Vx into
   memory, starting at the address in I. Setting `index_increment_on`
   to non-zero will increment I by the length of memory that was read.
 */
void ld_i_vx (Machine *machine, word nibbles[4], bool index_increment_on);

/* Fx65 - Read registers V0 through Vx from memory starting at location I.

   The interpreter reads values from memory starting at location I
   into registers V0 through Vx. Setting `index_increment_on` to
   non-zero will increment I by the length of memory that was read.
 */
void ld_vx_i (Machine *machine, word nibbles[4], bool index_increment_on);

/* NOP - Simply increments the program counter and moves on. */
void nop (Machine *machine, word nibbles[4]);

#endif
