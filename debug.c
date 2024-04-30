#include <stdio.h>
#include "machine.h" 
#include "debug.h"

char
printable_char (char c)
{
  if (c >= ASCII_PRINTABLE_START && c <= ASCII_PRINTABLE_END)
    return c;
  else
    return '.';
}

void
hex_print (word *bytes, address start, address end)
{
  unsigned int byte_count = end - start + 1,
    line_count = (byte_count + (HEXP_LINE_LENGTH - 1)) / HEXP_LINE_LENGTH;

  for (unsigned int i = 0; i < line_count; i++)
    {
      unsigned int line_start = (i * HEXP_LINE_LENGTH) + start;
      printf ("0x%03X: ", line_start);

      for (unsigned int j = 0; j < HEXP_LINE_LENGTH; j++)
        {
	  unsigned int addr = line_start + j;

	  if (addr <= end)
	    printf ("%02X ", bytes[addr]);
	  else
	    printf ("   ");
	}

      printf ("| ");

      for (unsigned int j = 0; j < HEXP_LINE_LENGTH; j++)
        {
	  unsigned int addr = line_start + j;

	  if (addr <= end)
	    printf ("%c", printable_char(bytes[addr]));
	  else
	    printf (" ");
	}

      printf ("\n");
    }
}

void
display_print (Machine *machine)
{
  for (int i = 0; i < DISPLAY_LINES; i++)
    {
      for (int j = 0; j < DISPLAY_COLUMNS; j++)
	{	  
	  if (machine->display[i][j])
	    printf ("\033[0;37m");
	  else
	    printf ("\033[0;30m");

	  printf ("█\033[0m");
	}

      printf ("\n");
    }
}
