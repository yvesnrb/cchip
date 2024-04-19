#include <stdbool.h>

unsigned char
char_to_hex (char c)
{
  switch (c)
    {
    case ('0'):
      return 0x0;
      break;
    case ('1'):
      return 0x1;
      break;
    case ('2'):
      return 0x2;
      break;
    case ('3'):
      return 0x3;
      break;
    case ('4'):
      return 0x4;
      break;
    case ('5'):
      return 0x5;
      break;
    case ('6'):
      return 0x6;
      break;
    case ('7'):
      return 0x7;
      break;
    case ('8'):
      return 0x8;
      break;
    case ('9'):
      return 0x9;
      break;
    case ('A'):
      return 0xA;
      break;
    case ('B'):
      return 0xB;
      break;
    case ('C'):
      return 0xC;
      break;
    case ('D'):
      return 0xD;
      break;
    case ('E'):
      return 0xE;
      break;
    case ('F'):
      return 0xF;
      break;
    default:
      return 0x10;
    }
}

void
split_nibbles (unsigned char nibbles[4], unsigned char high, unsigned char low)
{
  nibbles[0] = (high & 0b0000000011110000) >> 4;
  nibbles[1] = (high & 0b0000000000001111);
  nibbles[2] = (low & 0b0000000011110000) >> 4;
  nibbles[3] = (low & 0b0000000000001111);
}

bool
matches_op (char pattern[5], unsigned char high, unsigned char low)
{
  unsigned char pattern_nibbles[4], nibbles[4];
  split_nibbles (nibbles, high, low);

  for (int i = 0; pattern[i] != '\0'; i++)
    pattern_nibbles[i] = char_to_hex (pattern[i]);

  for (int i = 0; i < 4; i++)
    if (pattern_nibbles[i] != nibbles[i] && pattern_nibbles[i] != 0x10)
      return false;

  return true;
}
