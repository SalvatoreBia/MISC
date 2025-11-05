#pragma once
#ifndef QUEUE_H
#define QUEUE_H

#include <stddef.h>


/* Opaque handle to a queue instance. */
typedef struct misc_generic_queue* misc_queue;


/* Creates a new queue with the specified element size. */
misc_queue misc_queue_create(size_t elem_size);

/* Destroys a queue and frees all associated memory. */
void misc_queue_destroy(misc_queue q);

/* Clears all elements from the queue, setting its size to 0. */
void misc_queue_clear(misc_queue q);

/* Returns the number of elements in the queue. */
size_t misc_queue_size(const misc_queue q);

/* Checks whether the queue is empty. */
int misc_queue_isempty(const misc_queue q);

/* Enqueues an element at the back of the queue. */
int misc_queue_push(misc_queue q, const void *elem);

/* Dequeues the front element from the queue. */
void misc_queue_pop(misc_queue q, void *out);

/* Returns a pointer to the front element without removing it. */
void* misc_queue_front(const misc_queue q);

/* Returns a pointer to the back element without removing it. */
void* misc_queue_back(const misc_queue q);

#endif /* QUEUE_H */
