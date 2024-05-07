#include <stdbool.h>
#include <time.h>
#include <SDL2/SDL.h>
#include "machine.h"
#include "sdl.h"
#include "buzzer.h"

SDL_Renderer*
sdl_setup (int scaling_factor)
{
  SDL_Window *window = NULL;
  SDL_Renderer *renderer = NULL;

  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
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

uint64_t
current_ns_time ()
{
  uint64_t time = 0;
  struct timespec tp;

  clock_gettime (CLOCK_MONOTONIC, &tp);
  time += tp.tv_sec * 1000000000;
  time += tp.tv_nsec;

  return time;
}

void
poll_event (bool *quit, Machine *machine)
{
  SDL_Event e;
  const Uint8 *keystates;

  SDL_PumpEvents ();
  keystates = SDL_GetKeyboardState (NULL);

  for (int i = 0; i < 16; i++)
    if (keystates[SDL_GetScancodeFromKey (keymap[i])])
      machine->keypad[i] = true;
    else
      machine->keypad[i] = false;
  
  while (SDL_PollEvent(&e))
    {
      if (e.type == SDL_QUIT) *quit = true;
      if (e.type == SDL_KEYUP)
	for (int i = 0; i < 16; i++)
	  if (keymap[i] == e.key.keysym.sym)
            machine_keyboard_interrupt (machine, i);
    }
    
}

void
sdl_loop (Machine *machine, SDL_Renderer *renderer)
{
  bool quit = false;
  uint64_t last_machine_step = 0, last_timer_decrement = 0,
    last_display_interrupt = 0, current_time;

  while (!quit)
    {
      current_time = current_ns_time ();
      
      if (machine->sound > 0)
        buzzer_toggle (true);
      else
        buzzer_toggle (false);

      if (current_time >= (last_machine_step + 1000000))
	{
	  machine_step (machine);
	  last_machine_step = current_time;
	}

      if (current_time >= (last_timer_decrement + 16666666))
	{
	  machine_step_timers (machine);
	  last_timer_decrement = current_time;
	}

      if (current_time >= (last_display_interrupt + 16666666))
	{
	  machine_display_interrupt (machine);
	  last_display_interrupt = current_time;
	}

      poll_event (&quit, machine);
      sdl_render (renderer, machine);
    }

  SDL_Quit ();
}
