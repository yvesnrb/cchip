#pragma once

#include <stdbool.h>

void split_nibbles (unsigned char nibbles[4], unsigned char high, unsigned char low);
bool matches_op (char pattern[5], unsigned char high, unsigned char low);
