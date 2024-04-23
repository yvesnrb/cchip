#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "machine.h"
#include "debug.h"
#include "rom.h"
#include "decoder.h"
#include "sdl.h"

int
main (void)
{
  Machine *machine = (Machine*) malloc (sizeof (Machine));
  SDL_Renderer *renderer = sdl_setup (10);
  SDL_Event e;
  bool quit = false;

  if (!machine)
    {
      perror ("could not allocate memory for a chip machine");
      exit (EXIT_FAILURE);
    }

  load_rom (machine, "/Users/yves/Desktop/CChip/roms/logo.ch8");

  while (!quit)
    {
      step (machine);
      sdl_render (renderer, machine);
      SDL_Delay (250);
      
      while (SDL_PollEvent(&e))
	{
	  if (e.type == SDL_QUIT)
	    {
	      quit = true;
	    }
	}
    }

  SDL_Quit ();
  free (machine);
  return EXIT_SUCCESS;
}
