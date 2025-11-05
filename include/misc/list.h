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



/* Opaque handle to a singly linked list instance. */
typedef struct misc_generic_single_linked_list* misc_list;

/* Creates a new list with the specified element size. */
misc_list misc_list_create(size_t elem_size);

/* Destroys a list and frees all associated memory. */
void misc_list_destroy(misc_list list);

/* Clears all elements from the list, setting its size to 0. */
void misc_list_clear(misc_list list);

/* Returns the number of elements in the list. */
size_t misc_list_size(const misc_list list);

/* Checks whether the list is empty. */
int misc_list_isempty(const misc_list list);

/* Inserts an element at the front of the list. */
int misc_list_pushfront(misc_list list, const void *elem);

/* Inserts an element at the back of the list. */
int misc_list_pushback(misc_list list, const void *elem);

/* Removes the first element from the list. */
void misc_list_popfront(misc_list list, void *out);

/* Removes the last element from the list. */
void misc_list_popback(misc_list list, void *out);

/* Inserts an element at the specified index. */
int misc_list_insert(misc_list list, size_t idx, const void *elem);

/* Removes an element at the specified index. */
int misc_list_remove(misc_list list, size_t idx, void *out);

/* Returns a pointer to the element at the specified index. */
void* misc_list_get(const misc_list list, size_t idx);

/* Returns a pointer to the first element in the list. */
void* misc_list_front(const misc_list list);

/* Returns a pointer to the last element in the list. */
void* misc_list_back(const misc_list list);

#endif /* LIST_H */