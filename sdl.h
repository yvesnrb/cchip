/* This module contains functions for sound, video and keyboard input
   using SDL2.
 */

#pragma once

#include <SDL2/SDL.h>
#include "machine.h"

#define AMBER_ON 247, 206, 70, 255
#define AMBER_OFF 146, 105, 33, 255

/* Setup an SDL renderer using a scaling factor `scalling_factor`. The
   factor multiplies the native CHIP-8 resolution of 64x32
 */
SDL_Renderer* sdl_setup (int scaling_factor);

/* Render the display of a machine `machine` using an SDL renderer
   `renderer`.
 */
void sdl_render (SDL_Renderer *renderer, Machine *machine);
