/* This module contains helper functions for decoding instructions. */
#ifndef DECODER_H
#define DECODER_H

#include <stdbool.h>
#include "machine.h"

/* Split a `high` and a `low` bytes into an array of four nibbles. */
void split_nibbles (byte nibbles[4], byte high, byte low);

/* Return true if a high and a low bytes match a pattern string.
   The pattern string must contain four characters, the characters 0-F
   will match that literal value on high/low. Any other characters will
   match with any value at all.
 */
bool matches_op (char pattern[5], byte high, byte low);

#endif
