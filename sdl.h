/* This module contains functions for sound, video and keyboard input
   using SDL2. It also defines the main execution loop, since that is
   complexed with SDL.
 */
#ifndef MYSDL_H
#define MYSDL_H

#include <SDL2/SDL.h>
#include "machine.h"

#define AMBER_ON 247, 206, 70, 255
#define AMBER_OFF 146, 105, 33, 255

static const SDL_KeyCode keymap[16] = {
  SDLK_x, SDLK_1, SDLK_2, SDLK_3,
  SDLK_q, SDLK_w, SDLK_e, SDLK_a,
  SDLK_s, SDLK_d, SDLK_z, SDLK_c,
  SDLK_4, SDLK_r, SDLK_f, SDLK_v
};

/* Setup an SDL renderer using a scaling factor `scalling_factor`. The
   factor multiplies the native CHIP-8 resolution of 64x32
 */
SDL_Renderer* sdl_setup (int scaling_factor);

/* Render the display of a machine `machine` using an SDL renderer
   `renderer`.
 */
void sdl_render (SDL_Renderer *renderer, Machine *machine);

/* Loop the simulation on `machine`, using `renderer` until a
   quittable event occurs.
 */
void sdl_loop (Machine *machine, SDL_Renderer *renderer);

#endif
