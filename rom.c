#include <stdlib.h>
#include <stdio.h>
#include "machine.h"

void
load_rom (machine* m, char filename[])
{
  FILE* file = fopen (filename, "rb");
  unsigned char* buffer;
  unsigned long length;

  if (!file)
    {
      perror ("could not open rom file");
      exit (EXIT_FAILURE);
    }

  fseek (file, 0, SEEK_END);
  length = ftell (file);
  rewind (file);

  if (length > MAX_PROGRAM_LENGTH)
    {
      perror ("rom file is too large for this machine");
      exit (EXIT_FAILURE);
    }

  buffer = (unsigned char*) malloc (length * sizeof (unsigned char));

  if (!buffer)
    {
      perror ("could not allocate memory for a rom buffer");
      exit (EXIT_FAILURE);
    }

  fread (buffer, length, 1, file);

  for (unsigned short i = PROGRAM_START, j = 0; j < length; i++, j++)
    m->memory[i] = buffer[j];

  fclose (file);
  free (buffer);
}
