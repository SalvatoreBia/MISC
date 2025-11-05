#pragma once
#ifndef LIST_H
#define LIST_H

#include <stddef.h>
#include <sys/types.h>


typedef struct misc_generic_node
{
    void *val;

    struct misc_generic_node *next;
} misc_node;



/*
 * Opaque handle to a singly linked list instance.
 */
typedef struct misc_generic_single_linked_list* misc_list;

/*
 * Creates a new singly linked list with the specified element size.
 * Returns NULL on allocation failure.
 */
misc_list misc_list_create(size_t elem_size);

/*
 * Destroys the list and frees all associated memory.
 * The list handle becomes invalid after this call.
 */
void misc_list_destroy(misc_list list);

/*
 * Removes all elements from the list.
 * The list's size is set to zero.
 */
void misc_list_clear(misc_list list);

/*
 * Returns the number of elements currently stored in the list.
 */
size_t misc_list_size(const misc_list list);

/*
 * Checks if the list is empty.
 * Returns 1 if the list contains no elements, 0 otherwise.
 */
int misc_list_isempty(const misc_list list);

/*
 * Inserts an element at the front of the list.
 * Returns 1 on success, 0 on allocation failure.
 */
int misc_list_pushfront(misc_list list, const void *elem);

/*
 * Inserts an element at the back of the list.
 * Returns 1 on success, 0 on allocation failure.
 */
int misc_list_pushback(misc_list list, const void *elem);

/*
 * Removes the first element from the list.
 * If out is not NULL, the removed element is copied to the provided address.
 * Does nothing if the list is empty.
 */
void misc_list_popfront(misc_list list, void *out);

/*
 * Removes the last element from the list.
 * If out is not NULL, the removed element is copied to the provided address.
 * Does nothing if the list is empty.
 */
void misc_list_popback(misc_list list, void *out);

/*
 * Inserts an element at the specified index.
 * Elements at and after the index are shifted right.
 * Returns 1 on success, 0 on allocation failure or invalid index.
 */
int misc_list_insert(misc_list list, size_t idx, const void *elem);

/*
 * Removes the element at the specified index.
 * If out is not NULL, the removed element is copied to the provided address.
 * Returns 1 on success, 0 if the index is out of bounds.
 */
int misc_list_remove(misc_list list, size_t idx, void *out);

/*
 * Returns a pointer to the element at the specified index.
 * Returns NULL if the index is out of bounds.
 * The pointer remains valid until the list is modified.
 */
void* misc_list_get(const misc_list list, size_t idx);

/*
 * Returns a pointer to the first element in the list.
 * Returns NULL if the list is empty.
 * The pointer remains valid until the list is modified.
 */
void* misc_list_front(const misc_list list);

/*
 * Returns a pointer to the last element in the list.
 * Returns NULL if the list is empty.
 * The pointer remains valid until the list is modified.
 */
void* misc_list_back(const misc_list list);

#endif /* LIST_H */