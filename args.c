#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "args.h"

void
print_usage ()
{
  printf ("Usage: chip [-fidcyj] [-s number] rompath\n");
}

void
set_default_options (Options *options)
{
  options->vf_reset_on = true;
  options->index_increment_on = true;
  options->display_wait_on = true;
  options->display_clipping_on = true;
  options->vy_shifting_on = true;
  options->vx_jump_on = false;
  
  options->scaling = 10;
}

Options
arg_parse (int argc, char **argv)
{
  Options options;
  char c;

  set_default_options (&options);
  
  opterr = 0;
  while ((c = getopt (argc, argv, "s:fidcyj")) != -1)
    {
      switch (c)
	{
	case 's':
	  options.scaling = strtol (optarg, NULL, 10);
	  break;
	case 'f':
	  options.vf_reset_on = false;
	  break;
	case 'i':
	  options.index_increment_on = false;
	  break;
	case 'd':
	  options.display_wait_on = false;
	  break;
	case 'c':
	  options.display_clipping_on = false;
	  break;
	case 'y':
	  options.vy_shifting_on = false;
	  break;
	case 'j':
	  options.vx_jump_on = true;
	  break;
	case '?': default:
	  print_usage ();
	  exit (EXIT_FAILURE);
	  break;
	}
    }

  if (argv[optind] == NULL)
    {
      print_usage();
      exit (EXIT_FAILURE);
    }
  else
    {
      strncpy (options.rom_path, argv[optind], ROM_PATH_MAX_SIZE);
    }

  return options;
}
