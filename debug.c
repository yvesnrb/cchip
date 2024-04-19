#include <stdio.h>

/* TODO fix initial linebreak */
void
hex_print (unsigned char* m, unsigned int start, unsigned int end)
{
  for (unsigned short i = start; i <= end; i++)
    {
      if (i % 0x10 == 0)
	printf ("\n0x%03X: ", i);
      
      printf ("%02X ", m[i]);
    }

  printf ("\n");
}
