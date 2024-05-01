/* This module contains functions for a stack implementation fit to be
   the CHIP-8's stack. It is necessary for the initial value of the
   stack pointer to be -1.
 */
#ifndef STACK_H
#define STACK_H

#include "machine.h"

/* Add an element `value` to the stack `stack`, using `sp` as a stack
   pointer.
 */
void stack_push (address stack[], signed char *sp, address value);

/* Remove an element from the top of the stack and return it. */
address stack_pop (address stack[], signed char *sp);

/* Return the element at the top of the stack. */
address stack_top (address stack[], signed char *sp);

#endif
