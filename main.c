#include <stdio.h>
#include <stdlib.h>
#include "machine.h"
#include "debug.h"
#include "rom.h"
#include "decoder.h"

int
main (void)
{
  machine* chip = (machine*) malloc (sizeof (machine));

  if (!chip)
    {
      perror ("could not allocate memory for a chip machine");
      exit (EXIT_FAILURE);
    }

  load_rom (chip, "roms/logo.ch8");

  printf ("decoding 0xF110 with pattern F**0 %s\n", matches_op ("F**0", 0xF1, 0x10) ? "match!" : "no match!");
  printf ("decoding 0xF000 with pattern F**0 %s\n", matches_op ("F**0", 0xF0, 0x00) ? "match!" : "no match!");
  printf ("decoding 0xF111 with pattern F**0 %s\n", matches_op ("F**0", 0xF1, 0x11) ? "match!" : "no match!");
  printf ("decoding 0xA110 with pattern F**0 %s\n", matches_op ("F**0", 0xA1, 0x10) ? "match!" : "no match!");
  printf ("decoding 0xF11A with pattern ***A %s\n", matches_op ("***A", 0xF1, 0x1A) ? "match!" : "no match!");
  printf ("decoding 0xF11B with pattern ***A %s\n", matches_op ("***A", 0xF1, 0x1B) ? "match!" : "no match!");
  printf ("decoding 0xF11B with pattern **** %s\n", matches_op ("****", 0xF1, 0x1B) ? "match!" : "no match!");
  printf ("decoding 0xFCCB with pattern *CC* %s\n", matches_op ("*CC*", 0xFC, 0xCB) ? "match!" : "no match!");
  printf ("decoding 0xF1CB with pattern *CC* %s\n", matches_op ("*CC*", 0xF1, 0xCB) ? "match!" : "no match!");

  free (chip);
  return EXIT_SUCCESS;
}
