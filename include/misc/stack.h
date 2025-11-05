#pragma once
#ifndef STACK_H
#define STACK_H

#include <stddef.h>

/*
 * Opaque handle to a stack instance.
 */
typedef struct misc_generic_stack* misc_stack;

/*
 * Creates a new stack with the specified element size.
 * Returns NULL on allocation failure.
 */
misc_stack misc_stack_create(size_t elem_size);

/*
 * Destroys the stack and frees all associated memory.
 * The stack handle becomes invalid after this call.
 */
void misc_stack_destroy(misc_stack st);

/*
 * Removes all elements from the stack.
 * The stack's size is set to zero.
 */
void misc_stack_clear(misc_stack st);

/*
 * Returns the number of elements currently stored in the stack.
 */
size_t misc_stack_size(const misc_stack st);

/*
 * Checks if the stack is empty.
 * Returns 1 if the stack contains no elements, 0 otherwise.
 */
int misc_stack_isempty(const misc_stack st);

/*
 * Pushes an element onto the top of the stack.
 * Returns 1 on success, 0 on allocation failure.
 */
int misc_stack_push(misc_stack st, const void *elem);

/*
 * Removes the top element from the stack.
 * If out is not NULL, the removed element is copied to the provided address.
 * Returns 1 if an element was removed, 0 if the stack was empty.
 */
int misc_stack_pop(misc_stack st, void *out);

/*
 * Returns a pointer to the top element without removing it.
 * Returns NULL if the stack is empty.
 * The pointer remains valid until the stack is modified.
 */
void* misc_stack_top(const misc_stack st);

#endif /* STACK_H */