#include <stdio.h>
#include <stdlib.h>
#include "machine.h"
#include "debug.h"
#include "rom.h"
#include "decoder.h"

int
main (void)
{
  Machine *machine = (Machine*) malloc (sizeof (Machine));

  if (!machine)
    {
      perror ("could not allocate memory for a chip machine");
      exit (EXIT_FAILURE);
    }

  load_rom (machine, "/Users/yves/Desktop/CChip/roms/logo.ch8");

  for (int i = 0; i < 40; i++)
    step (machine);

  display_print (machine);
  printf ("program counter: 0x%X\n", machine->pc);

  free (machine);
  return EXIT_SUCCESS;
}
