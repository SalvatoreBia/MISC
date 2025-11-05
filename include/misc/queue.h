#pragma once
#ifndef QUEUE_H
#define QUEUE_H

#include <stddef.h>


/*
 * Opaque handle to a queue instance.
 */
typedef struct misc_generic_queue* misc_queue;

/*
 * Creates a new queue with the specified element size.
 * Returns NULL on allocation failure.
 */
misc_queue misc_queue_create(size_t elem_size);

/*
 * Destroys the queue and frees all associated memory.
 * The queue handle becomes invalid after this call.
 */
void misc_queue_destroy(misc_queue q);

/*
 * Removes all elements from the queue.
 * The queue's size is set to zero.
 */
void misc_queue_clear(misc_queue q);

/*
 * Returns the number of elements currently stored in the queue.
 */
size_t misc_queue_size(const misc_queue q);

/*
 * Checks if the queue is empty.
 * Returns 1 if the queue contains no elements, 0 otherwise.
 */
int misc_queue_isempty(const misc_queue q);

/*
 * Enqueues an element at the back of the queue.
 * Returns 1 on success, 0 on allocation failure.
 */
int misc_queue_push(misc_queue q, const void *elem);

/*
 * Dequeues the front element from the queue.
 * If out is not NULL, the removed element is copied to the provided address.
 * Does nothing if the queue is empty.
 */
void misc_queue_pop(misc_queue q, void *out);

/*
 * Returns a pointer to the front element without removing it.
 * Returns NULL if the queue is empty.
 * The pointer remains valid until the queue is modified.
 */
void* misc_queue_front(const misc_queue q);

/*
 * Returns a pointer to the back element without removing it.
 * Returns NULL if the queue is empty.
 * The pointer remains valid until the queue is modified.
 */
void* misc_queue_back(const misc_queue q);

#endif /* QUEUE_H */
