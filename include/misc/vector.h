#pragma once
#ifndef VECTOR_H
#define VECTOR_H

#include <stddef.h>

#define DEFAULT_CAPACITY 32

/* Opaque handle to a vector instance. */
typedef struct misc_generic_vector* misc_vector;

/* Creates a new vector with the specified element size. */
misc_vector misc_vector_create(size_t elem_size);

/* Destroys a vector and frees all associated memory. */
void misc_vector_destroy(misc_vector vec);

/* Clears all elements from the vector, setting its length to 0. */
void misc_vector_clear(misc_vector vec);

/* Returns the number of elements in the vector. */
size_t misc_vector_length(const misc_vector vec);

/* Returns the current capacity of the vector. */
size_t misc_vector_capacity(const misc_vector vec);

/* Checks whether the vector is empty. */
int misc_vector_isempty(const misc_vector vec);

/* Appends an element to the end of the vector. */
int misc_vector_pushback(misc_vector vec, const void *elem);

/* Removes the last element from the vector. */
void misc_vector_popback(misc_vector vec, void *out);

/* Inserts an element at the specified index, shifting subsequent elements right. */
int misc_vector_insert(misc_vector vec, size_t idx, const void *elem);

/* Removes an element at the specified index, shifting subsequent elements left. */
int misc_vector_remove(misc_vector vec, size_t idx, void *out);

/* Sets the value of an element at the specified index. */
void misc_vector_set(misc_vector vec, size_t idx, const void *elem);

/* Returns a pointer to the element at the specified index. */
void* misc_vector_get(const misc_vector vec, size_t idx);

/* Returns a pointer to the first element in the vector. */
void* misc_vector_front(const misc_vector vec);

/* Returns a pointer to the last element in the vector. */
void* misc_vector_back(const misc_vector vec);


#endif /* VECTOR_H */
