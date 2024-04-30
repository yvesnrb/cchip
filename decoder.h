/* This module contains helper functions for decoding instructions. */
#ifndef DECODER_H
#define DECODER_H

#include <stdbool.h>
#include "machine.h"

/* Split a `high` and a `low` words into an array of four nibbles. */
void split_nibbles (word nibbles[4], word high, word low);

/* Return true if a high and a low words match a pattern string.
   The pattern string must contain four characters, the characters 0-F
   will match that literal value on high/low. Any other characters will
   match with any value at all.
 */
bool matches_op (char pattern[5], word high, word low);

#endif
