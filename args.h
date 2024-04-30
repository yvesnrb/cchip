/* This module contains functions for parsing and handling command
   line arguments.
 */
#ifndef ARGS_H
#define ARGS_H

#define ROM_PATH_MAX_SIZE 100

/* Parse the argument count `argc` and argument values `argv` into
   `scaling` and `rom_path`
 */
void arg_parse (int argc,
		char **argv,
		int* scaling,
		char rom_path[ROM_PATH_MAX_SIZE]);

#endif
