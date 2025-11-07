#pragma once
#ifndef LIST_H
#define LIST_H

#include <stddef.h>
#include <sys/types.h>

/**
 * @brief Opaque handle to a singly linked list instance.
 */
typedef struct misc_generic_single_linked_list* misc_list;

/**
 * @brief Creates a new singly linked list with the specified element size.
 * @param elem_size Size in bytes of each element
 * @return Pointer to the new list, or NULL on allocation failure
 */
misc_list misc_list_create(size_t elem_size);

/**
 * @brief Destroys the list and frees all associated memory.
 * @param list List to destroy
 * @note The list handle becomes invalid after this call.
 */
void misc_list_destroy(misc_list list);

/**
 * @brief Removes all elements from the list.
 * @param list List to clear
 * @note The list's size is set to zero.
 */
void misc_list_clear(misc_list list);

/**
 * @brief Returns the number of elements currently stored in the list.
 * @param list List to query
 * @return Number of elements in the list
 */
size_t misc_list_size(const misc_list list);

/**
 * @brief Checks if the list is empty.
 * @param list List to check
 * @return 1 if the list contains no elements, 0 otherwise
 */
int misc_list_isempty(const misc_list list);

/**
 * @brief Inserts an element at the front of the list.
 * @param list List to modify
 * @param elem Pointer to the element to insert
 * @return 1 on success, 0 on allocation failure
 */
int misc_list_pushfront(misc_list list, const void *elem);

/**
 * @brief Inserts an element at the back of the list.
 * @param list List to modify
 * @param elem Pointer to the element to insert
 * @return 1 on success, 0 on allocation failure
 */
int misc_list_pushback(misc_list list, const void *elem);

/**
 * @brief Removes the first element from the list.
 * @param list List to modify
 * @param out Optional pointer where removed element will be copied (can be NULL)
 * @note Does nothing if the list is empty.
 */
void misc_list_popfront(misc_list list, void *out);

/**
 * @brief Removes the last element from the list.
 * @param list List to modify
 * @param out Optional pointer where removed element will be copied (can be NULL)
 * @note Does nothing if the list is empty.
 */
void misc_list_popback(misc_list list, void *out);

/**
 * @brief Inserts an element at the specified index.
 * @param list List to modify
 * @param idx Index where to insert the element
 * @param elem Pointer to the element to insert
 * @return 1 on success, 0 on allocation failure or invalid index
 * @note Elements at and after the index are shifted right.
 */
int misc_list_insert(misc_list list, size_t idx, const void *elem);

/**
 * @brief Removes the element at the specified index.
 * @param list List to modify
 * @param idx Index of element to remove
 * @param out Optional pointer where removed element will be copied (can be NULL)
 * @return 1 on success, 0 if the index is out of bounds
 */
int misc_list_remove(misc_list list, size_t idx, void *out);

/**
 * @brief Returns a pointer to the element at the specified index.
 * @param list List to query
 * @param idx Index of element to retrieve
 * @return Pointer to the element, or NULL if index is out of bounds
 * @warning The pointer remains valid until the list is modified.
 */
void* misc_list_get(const misc_list list, size_t idx);

/**
 * @brief Returns a pointer to the first element in the list.
 * @param list List to query
 * @return Pointer to the first element, or NULL if list is empty
 * @warning The pointer remains valid until the list is modified.
 */
void* misc_list_front(const misc_list list);

/**
 * @brief Returns a pointer to the last element in the list.
 * @param list List to query
 * @return Pointer to the last element, or NULL if list is empty
 * @warning The pointer remains valid until the list is modified.
 */
void* misc_list_back(const misc_list list);

#endif /* LIST_H */