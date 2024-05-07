#include <stdbool.h>
#include <stdlib.h>
#include "SDL_audio.h"
#include "buzzer.h"

oscillator
oscillator_create (float freq, float volume)
{
  oscillator buzzer =
    {
      .current_step = 0,
      .volume = volume,
      .step_size = (2 * M_PI) / freq
    };

  return buzzer;
}

float
oscillator_step (oscillator *osc)
{
  osc->current_step += osc->step_size;
  return sinf (osc->current_step) * osc->volume;
}

void
oscillator_callback (void *params, Uint8 *stream, int length)
{
  float *fstream = (float*) stream;
  oscillator *osc = (oscillator*) params;

  for (int i = 0; i < 2; i++)
    {
      float volume = oscillator_step (osc);
      fstream[i] = volume;
    }
}

SDL_AudioSpec*
buzzer_setup ()
{
  float volume = 0.8, freq = 44100 / 440.00;
  oscillator *buzzer_oscillator =
    (oscillator*) malloc (sizeof (oscillator));
  SDL_AudioSpec *audio_spec =
    (SDL_AudioSpec*) malloc (sizeof (SDL_AudioSpec));

  *buzzer_oscillator = oscillator_create (freq, volume);
  audio_spec->format = AUDIO_F32SYS;
  audio_spec->channels = 1;
  audio_spec->freq = 44100;
  audio_spec->samples = 2;
  audio_spec->callback = oscillator_callback;
  audio_spec->userdata = buzzer_oscillator;

  if (SDL_OpenAudio(audio_spec, NULL) < 0)
    {
      perror ("could not initialize SDL audio");
      exit (EXIT_FAILURE);
    }

  return audio_spec;
}

void
buzzer_teardown (SDL_AudioSpec *audio_spec)
{
  free (audio_spec->userdata);
  free (audio_spec);
}

void
buzzer_toggle (bool is_on)
{
  if (is_on) SDL_PauseAudio (false);
  else SDL_PauseAudio (true);
}
