#include <stdio.h>
#include <stdlib.h>
#include "machine.h"
#include "debug.h"
#include "rom.h"

int
main (void)
{
  machine* chip = (machine*) malloc (sizeof (machine));

  if (!chip)
    {
      perror ("could not allocate memory for a chip machine");
      exit (EXIT_FAILURE);
    }

  load_rom (chip, "roms/logo.ch8");
  hex_print (chip->memory, 0x200, 0x305);

  free (chip);
  return EXIT_SUCCESS;
}
