#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include "args.h"

void
print_usage ()
{
  printf ("Usage: chip [-s number] rompath\n");
}

void
arg_parse (int argc,
	   char **argv,
	   int *scaling,
	   char rom_path[ROM_PATH_MAX_SIZE])
{
  char c;
  opterr = 0;

  while ((c = getopt (argc, argv, "s:")) != -1)
    {
      switch (c)
	{
	case 's':
	  *scaling = strtol (optarg, NULL, 10);
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
      strncpy (rom_path, argv[optind], ROM_PATH_MAX_SIZE);
    }
}
