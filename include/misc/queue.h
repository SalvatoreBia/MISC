#pragma once
#ifndef QUEUE_H
#define QUEUE_H

#include <stddef.h>


/**
 * @brief Opaque handle to a queue instance.
 */
typedef struct misc_generic_queue* misc_queue;

/**
 * @brief Creates a new queue with the specified element size.
 * @param elem_size Size in bytes of each element
 * @return Pointer to the new queue, or NULL on allocation failure
 */
misc_queue misc_queue_create(size_t elem_size);

/**
 * @brief Destroys the queue and frees all associated memory.
 * @param q Queue to destroy
 * @note The queue handle becomes invalid after this call.
 */
void misc_queue_destroy(misc_queue q);

/**
 * @brief Removes all elements from the queue.
 * @param q Queue to clear
 * @note The queue's size is set to zero.
 */
void misc_queue_clear(misc_queue q);

/**
 * @brief Returns the number of elements currently stored in the queue.
 * @param q Queue to query
 * @return Number of elements in the queue
 */
size_t misc_queue_size(const misc_queue q);

/**
 * @brief Checks if the queue is empty.
 * @param q Queue to check
 * @return 1 if the queue contains no elements, 0 otherwise
 */
int misc_queue_isempty(const misc_queue q);

/**
 * @brief Enqueues an element at the back of the queue.
 * @param q Queue to modify
 * @param elem Pointer to the element to enqueue
 * @return 1 on success, 0 on allocation failure
 */
int misc_queue_push(misc_queue q, const void *elem);

/**
 * @brief Dequeues the front element from the queue.
 * @param q Queue to modify
 * @param out Optional pointer where removed element will be copied (can be NULL)
 * @note Does nothing if the queue is empty.
 */
void misc_queue_pop(misc_queue q, void *out);

/**
 * @brief Returns a pointer to the front element without removing it.
 * @param q Queue to query
 * @return Pointer to the front element, or NULL if queue is empty
 * @warning The pointer remains valid until the queue is modified.
 */
void* misc_queue_front(const misc_queue q);

/**
 * @brief Returns a pointer to the back element without removing it.
 * @param q Queue to query
 * @return Pointer to the back element, or NULL if queue is empty
 * @warning The pointer remains valid until the queue is modified.
 */
void* misc_queue_back(const misc_queue q);

#endif /* QUEUE_H */
