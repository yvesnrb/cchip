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

  chip->display[0][0] = true;
  chip->display[31][63] = true;

  display_print (chip);

  free (chip);
  return EXIT_SUCCESS;
}
