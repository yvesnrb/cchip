#include <stdbool.h>
#include <SDL2/SDL.h>
#include "machine.h"
#include "sdl.h"

SDL_Renderer*
sdl_setup (int scaling_factor)
{
  SDL_Window *window = NULL;
  SDL_Renderer *renderer = NULL;

  if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
      perror ("could not initialize SDL");
      exit (EXIT_FAILURE);
    }

  SDL_CreateWindowAndRenderer (DISPLAY_COLUMNS * scaling_factor,
			       DISPLAY_LINES * scaling_factor,
			       0,
			       &window,
			       &renderer);

  if (!window)
    {
      perror ("could not create SDL window");
      exit (EXIT_FAILURE);
    }

  if (!renderer)
    {
      perror ("could not create SDL renderer");
      exit (EXIT_FAILURE);
    }

  SDL_RenderSetScale (renderer, scaling_factor, scaling_factor);
  return renderer;
}

void
sdl_render (SDL_Renderer *renderer, Machine *machine)
{
  SDL_SetRenderDrawColor (renderer, AMBER_OFF);
  SDL_RenderClear (renderer);

  for (int i = 0; i < DISPLAY_LINES; i++)
    for (int j = 0; j < DISPLAY_COLUMNS; j++)
      {
	if (machine->display[i][j])
          SDL_SetRenderDrawColor (renderer, AMBER_ON);
	else
          SDL_SetRenderDrawColor (renderer, AMBER_OFF);

	SDL_RenderDrawPoint (renderer, j, i);
      }

  SDL_RenderPresent (renderer);
}

void
sdl_loop (Machine *machine, SDL_Renderer *renderer)
{
  SDL_Event e;
  bool quit = false;

  while (!quit)
    {
      step (machine);
      sdl_render (renderer, machine);
      /* SDL_Delay (250); */

      while (SDL_PollEvent(&e))
	{
	  if (e.type == SDL_QUIT)
	    {
	      quit = true;
	    }
	}
    }

  SDL_Quit ();
}
