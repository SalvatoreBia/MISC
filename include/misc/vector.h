#pragma once
#ifndef VECTOR_H
#define VECTOR_H

#include <stddef.h>

#define DEFAULT_CAPACITY 32

/*
 * Opaque handle to a vector instance.
 */
typedef struct misc_generic_vector* misc_vector;

/*
 * Creates a new vector with the specified element size.
 * Returns NULL on allocation failure.
 */
misc_vector misc_vector_create(size_t elem_size);

/*
 * Destroys the vector and frees all associated memory.
 * The vector handle becomes invalid after this call.
 */
void misc_vector_destroy(misc_vector vec);

/*
 * Removes all elements from the vector.
 * The vector's length is set to zero but capacity remains unchanged.
 */
void misc_vector_clear(misc_vector vec);

/*
 * Returns the number of elements currently stored in the vector.
 */
size_t misc_vector_length(const misc_vector vec);

/*
 * Returns the current capacity of the vector.
 * Capacity represents the number of elements that can be stored without reallocation.
 */
size_t misc_vector_capacity(const misc_vector vec);

/*
 * Checks if the vector is empty.
 * Returns 1 if the vector contains no elements, 0 otherwise.
 */
int misc_vector_isempty(const misc_vector vec);

/*
 * Appends an element to the end of the vector.
 * Automatically grows the vector if necessary.
 * Returns 1 on success, 0 on allocation failure.
 */
int misc_vector_pushback(misc_vector vec, const void *elem);

/*
 * Removes and optionally retrieves the last element from the vector.
 * If out is not NULL, the removed element is copied to the provided address.
 * Does nothing if the vector is empty.
 */
void misc_vector_popback(misc_vector vec, void *out);

/*
 * Inserts an element at the specified index.
 * Elements at and after the index are shifted right.
 * Returns 1 on success, 0 on allocation failure or invalid index.
 */
int misc_vector_insert(misc_vector vec, size_t idx, const void *elem);

/*
 * Removes the element at the specified index.
 * Elements after the index are shifted left.
 * If out is not NULL, the removed element is copied to the provided address.
 * Returns 1 on success, 0 if the index is out of bounds.
 */
int misc_vector_remove(misc_vector vec, size_t idx, void *out);

/*
 * Sets the value of the element at the specified index.
 * Does nothing if the index is out of bounds.
 */
void misc_vector_set(misc_vector vec, size_t idx, const void *elem);

/*
 * Returns a pointer to the element at the specified index.
 * Returns NULL if the index is out of bounds.
 * The pointer remains valid until the vector is modified.
 */
void* misc_vector_get(const misc_vector vec, size_t idx);

/*
 * Returns a pointer to the first element in the vector.
 * Returns NULL if the vector is empty.
 * The pointer remains valid until the vector is modified.
 */
void* misc_vector_front(const misc_vector vec);

/*
 * Returns a pointer to the last element in the vector.
 * Returns NULL if the vector is empty.
 * The pointer remains valid until the vector is modified.
 */
void* misc_vector_back(const misc_vector vec);


#endif /* VECTOR_H */
