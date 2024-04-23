#include <stdlib.h>
#include <stdio.h>
#include "machine.h"
#include "rom.h"

void
load_rom (Machine *machine, char filename[])
{
  FILE* file = fopen (filename, "rb");
  byte *buffer;
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

  buffer = (byte*) malloc (length * sizeof (byte));

  if (!buffer)
    {
      perror ("could not allocate memory for a rom buffer");
      exit (EXIT_FAILURE);
    }

  fread (buffer, length, 1, file);

  for (address i = PROGRAM_START, j = 0; j < length; i++, j++)
    machine->memory[i] = buffer[j];

  machine->pc = PROGRAM_START;

  fclose (file);
  free (buffer);
}
