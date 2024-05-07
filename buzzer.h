/* This module contains functions for implementing a simple buzzer. */
#ifndef BUZZER_H
#define BUZZER_H

#include "SDL_audio.h"
#include <stdbool.h>

typedef struct oscillator oscillator;

struct oscillator {
  float current_step;
  float step_size;
  float volume;
};

/* Initialize sound system. Must be called before buzzer can be
   toggled.
 */
SDL_AudioSpec* buzzer_setup ();

/* Free allocated memory associated with the sound system. */
void buzzer_teardown (SDL_AudioSpec *audio_spec);

/* Toggle the buzzer on or off. */
void buzzer_toggle (bool is_on);

#endif
