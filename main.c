#include <SDL2/SDL.h>
#include "args.h"
#include "machine.h"
#include "rom.h"
#include "decoder.h"
#include "sdl.h"

int
main (int argc, char **argv)
{
  Machine *machine = NULL;
  SDL_Renderer *renderer = NULL;
  int scaling = 10;
  char rom_path[100];

  arg_parse (argc, argv, &scaling, rom_path);

  machine = (Machine*) malloc (sizeof (Machine));
  machine->sp = -1;

  if (!machine)
    {
      perror ("could not allocate memory for a chip machine");
      exit (EXIT_FAILURE);
    }

  load_rom (machine, rom_path);
  renderer = sdl_setup (scaling);
  sdl_loop (machine, renderer);

  free (machine);
  return EXIT_SUCCESS;
}
