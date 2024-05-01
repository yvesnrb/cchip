#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "stack.h"
#include "machine.h"

bool
is_empty (signed char sp)
{
  if (sp == -1)
    return true;
  else
    return false;
}

bool
is_full (signed char sp)
{
  if (sp >= STACK_SIZE)
    return true;
  else
    return false;
}

void
stack_push (address stack[], signed char *sp, address value)
{
  if (is_full (*sp))
    {
      printf ("CHIP-8 stack overflow.\n");
      exit (EXIT_FAILURE);
    }

  stack[*sp + 1] = value;
  (*sp)++;
}

address
stack_pop (address stack[], signed char *sp)
{
  if (is_empty (*sp))
    return (address) 0;

  (*sp)--;
  return stack[*sp + 1];
}

address
stack_top (address stack[], signed char *sp)
{
  if (is_empty (*sp))
    return (address) 0;
  else
    return stack[*sp];
}
