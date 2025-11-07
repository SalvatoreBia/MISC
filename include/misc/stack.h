#pragma once
#ifndef STACK_H
#define STACK_H

#include <stddef.h>

/**
 * @brief Opaque handle to a stack instance.
 */
typedef struct misc_generic_stack* misc_stack;

/**
 * @brief Creates a new stack with the specified element size.
 * @param elem_size Size in bytes of each element
 * @return Pointer to the new stack, or NULL on allocation failure
 */
misc_stack misc_stack_create(size_t elem_size);

/**
 * @brief Destroys the stack and frees all associated memory.
 * @param st Stack to destroy
 * @note The stack handle becomes invalid after this call.
 */
void misc_stack_destroy(misc_stack st);

/**
 * @brief Removes all elements from the stack.
 * @param st Stack to clear
 * @note The stack's size is set to zero.
 */
void misc_stack_clear(misc_stack st);

/**
 * @brief Returns the number of elements currently stored in the stack.
 * @param st Stack to query
 * @return Number of elements in the stack
 */
size_t misc_stack_size(const misc_stack st);

/**
 * @brief Checks if the stack is empty.
 * @param st Stack to check
 * @return 1 if the stack contains no elements, 0 otherwise
 */
int misc_stack_isempty(const misc_stack st);

/**
 * @brief Pushes an element onto the top of the stack.
 * @param st Stack to modify
 * @param elem Pointer to the element to push
 * @return 1 on success, 0 on allocation failure
 */
int misc_stack_push(misc_stack st, const void *elem);

/**
 * @brief Removes the top element from the stack.
 * @param st Stack to modify
 * @param out Optional pointer where removed element will be copied (can be NULL)
 * @return 1 if an element was removed, 0 if the stack was empty
 */
int misc_stack_pop(misc_stack st, void *out);

/**
 * @brief Returns a pointer to the top element without removing it.
 * @param st Stack to query
 * @return Pointer to the top element, or NULL if stack is empty
 * @warning The pointer remains valid until the stack is modified.
 */
void* misc_stack_top(const misc_stack st);

#endif /* STACK_H */