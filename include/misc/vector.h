#pragma once
#ifndef VECTOR_H
#define VECTOR_H

#include <stddef.h>

#define DEFAULT_CAPACITY 32

/*
 * Opaque handle to a vector instance.
 */
typedef struct misc_generic_vector* misc_vector;

/**
 * @brief Creates a new vector with the specified element size.
 * @param elem_size Size in bytes of each element
 * @return Pointer to the new vector, or NULL on allocation failure
 */
misc_vector misc_vector_create(size_t elem_size);

/**
 * @brief Destroys the vector and frees all associated memory.
 * @param vec Vector to destroy
 * @note The vector handle becomes invalid after this call.
 */
void misc_vector_destroy(misc_vector vec);

/**
 * @brief Removes all elements from the vector.
 * @param vec Vector to clear
 * @note The vector's length is set to zero but capacity remains unchanged.
 */
void misc_vector_clear(misc_vector vec);

/**
 * @brief Returns the number of elements currently stored in the vector.
 * @param vec Vector to query
 * @return Number of elements in the vector
 */
size_t misc_vector_length(const misc_vector vec);

/**
 * @brief Returns the current capacity of the vector.
 * @param vec Vector to query
 * @return Current capacity (number of elements that can be stored without reallocation)
 */
size_t misc_vector_capacity(const misc_vector vec);

/**
 * @brief Checks if the vector is empty.
 * @param vec Vector to check
 * @return 1 if the vector contains no elements, 0 otherwise
 */
int misc_vector_isempty(const misc_vector vec);

/**
 * @brief Appends an element to the end of the vector.
 * @param vec Vector to modify
 * @param elem Pointer to the element to append
 * @return 1 on success, 0 on allocation failure
 * @note Automatically grows the vector if necessary.
 */
int misc_vector_pushback(misc_vector vec, const void *elem);

/**
 * @brief Removes and optionally retrieves the last element from the vector.
 * @param vec Vector to modify
 * @param out Optional pointer where removed element will be copied (can be NULL)
 * @note Does nothing if the vector is empty.
 */
void misc_vector_popback(misc_vector vec, void *out);

/**
 * @brief Inserts an element at the specified index.
 * @param vec Vector to modify
 * @param idx Index where to insert the element
 * @param elem Pointer to the element to insert
 * @return 1 on success, 0 on allocation failure or invalid index
 * @note Elements at and after the index are shifted right.
 */
int misc_vector_insert(misc_vector vec, size_t idx, const void *elem);

/**
 * @brief Removes the element at the specified index.
 * @param vec Vector to modify
 * @param idx Index of element to remove
 * @param out Optional pointer where removed element will be copied (can be NULL)
 * @return 1 on success, 0 if the index is out of bounds
 * @note Elements after the index are shifted left.
 */
int misc_vector_remove(misc_vector vec, size_t idx, void *out);

/**
 * @brief Sets the value of the element at the specified index.
 * @param vec Vector to modify
 * @param idx Index of element to set
 * @param elem Pointer to the new element value
 * @note Does nothing if the index is out of bounds.
 */
void misc_vector_set(misc_vector vec, size_t idx, const void *elem);

/**
 * @brief Returns a pointer to the element at the specified index.
 * @param vec Vector to query
 * @param idx Index of element to retrieve
 * @return Pointer to the element, or NULL if index is out of bounds
 * @warning The pointer remains valid until the vector is modified.
 */
void* misc_vector_get(const misc_vector vec, size_t idx);

/**
 * @brief Returns a pointer to the first element in the vector.
 * @param vec Vector to query
 * @return Pointer to the first element, or NULL if vector is empty
 * @warning The pointer remains valid until the vector is modified.
 */
void* misc_vector_front(const misc_vector vec);

/**
 * @brief Returns a pointer to the last element in the vector.
 * @param vec Vector to query
 * @return Pointer to the last element, or NULL if vector is empty
 * @warning The pointer remains valid until the vector is modified.
 */
void* misc_vector_back(const misc_vector vec);


#endif /* VECTOR_H */
