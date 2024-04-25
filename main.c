#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <string.h>
#include <unistd.h>
#include "machine.h"
#include "debug.h"
#include "rom.h"
#include "decoder.h"
#include "sdl.h"

void
usage ()
{
  printf ("Usage: chip [-s number] rompath\n");
}

int
main (int argc, char **argv)
{
  Machine *machine = NULL;
  SDL_Renderer *renderer = NULL;
  SDL_Event e;
  bool quit = false;
  int scaling = 10;
  char rom_path[100];
  char c;
  opterr = 0;

  while ((c = getopt (argc, argv, "s:")) != -1)
    {
      switch (c)
	{
	case 's':
	  scaling = strtol (optarg, NULL, 10);
	  break;
	case '?': default:
	  usage ();
	  exit (EXIT_FAILURE);
	  break;
	}
    }

  if (argv[optind] == NULL)
    {
      usage ();
      exit (EXIT_FAILURE);
    }
  else
    {
      strncpy (rom_path, argv[optind], sizeof (rom_path));
    }

  machine = (Machine*) malloc (sizeof (Machine));

  if (!machine)
    {
      perror ("could not allocate memory for a chip machine");
      exit (EXIT_FAILURE);
    }

  load_rom (machine, rom_path);
  renderer = sdl_setup (scaling);

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
