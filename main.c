#include <SDL2/SDL.h>
#include <time.h>
#include <stdlib.h>
#include "args.h"
#include "machine.h"
#include "rom.h"
#include "decoder.h"
#include "sdl.h"
#include "buzzer.h"

int
main (int argc, char **argv)
{
  Machine *machine = NULL;
  SDL_Renderer *renderer = NULL;
  SDL_AudioSpec *audio_spec = NULL;
  Options options;

  options = arg_parse (argc, argv);

  machine = (Machine*) malloc (sizeof (Machine));

  if (!machine)
    {
      perror ("could not allocate memory for a chip machine");
      exit (EXIT_FAILURE);
    }

  srand (time (NULL));
  load_rom (machine, options.rom_path);
  machine_reset (machine);
  renderer = sdl_setup (options.scaling);
  audio_spec = buzzer_setup ();
  sdl_loop (machine, renderer, options);

  buzzer_teardown (audio_spec);
  free (machine);
  return EXIT_SUCCESS;
}
