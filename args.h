/* This module contains functions for parsing and handling command
   line arguments.
 */
#ifndef ARGS_H
#define ARGS_H

#include <stdbool.h>

#define ROM_PATH_MAX_SIZE 100

typedef struct Options Options;

struct Options
{
  bool vf_reset_on;
  bool index_increment_on;
  bool display_wait_on;
  bool display_clipping_on;
  bool vy_shifting_on;
  bool vx_jump_on;
  int scaling;
  char rom_path[ROM_PATH_MAX_SIZE];
};

/* Parse the argument count `argc` and argument values `argv` into
   an `Options` struct.
 */
Options arg_parse (int argc, char **argv);

#endif
