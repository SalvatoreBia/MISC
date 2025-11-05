#pragma once
#ifndef STACK_H
#define STACK_H

#include <stddef.h>

/* Opaque handle to a stack instance. */
typedef struct misc_generic_stack* misc_stack;

/* Creates a new stack with the specified element size. */
misc_stack misc_stack_create(size_t elem_size);

/* Destroys a stack and frees all associated memory. */
void misc_stack_destroy(misc_stack st);

/* Clears all elements from the stack, setting its size to 0. */
void misc_stack_clear(misc_stack st);

/* Returns the number of elements in the stack. */
size_t misc_stack_size(const misc_stack st);

/* Checks whether the stack is empty. */
int misc_stack_isempty(const misc_stack st);

/* Pushes an element onto the top of the stack. */
int misc_stack_push(misc_stack st, const void *elem);

/* Pops the top element from the stack. 
 *
 * returns 1 if the stack was not empty before the call, 0 otherwise.
 */
int misc_stack_pop(misc_stack st, void *out);

/* Returns a pointer to the top element without removing it. */
void* misc_stack_top(const misc_stack st);

#endif /* STACK_H */