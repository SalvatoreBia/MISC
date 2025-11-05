#include "misc/queue.h"
#include "misc/list.h"
#include <stdlib.h>


struct misc_generic_queue
{
    misc_list list;
};


misc_queue misc_queue_create(size_t elem_size)
{
    if (elem_size == 0) return NULL;

    misc_queue queue = (misc_queue) malloc(sizeof(struct misc_generic_queue));
    if (queue == NULL) return NULL;

    queue->list = misc_list_create(elem_size);
    if (queue->list == NULL)
    {
        free(queue);
        return NULL;
    }
    
    return queue;
}


void misc_queue_destroy(misc_queue q)
{
    if (q != NULL)
    {
        misc_list_destroy(q->list);
        free((void*)q);
    }
}


void misc_queue_clear(misc_queue q)
{
    if (q == NULL) return;
    misc_list_clear(q->list);
}


size_t misc_queue_size(const misc_queue q)
{
    return misc_list_size(q->list);
}


int misc_queue_isempty(const misc_queue q)
{
    if (q == NULL) return 1;
    return misc_list_isempty(q->list);
}


int misc_queue_push(misc_queue q, const void *elem)
{
    if (q == NULL) return 0;
    return misc_list_pushback(q->list, elem);
}


void misc_queue_pop(misc_queue q, void *out)
{
    if (q == NULL) return;
    misc_list_popfront(q->list, out);
}


void* misc_queue_front(const misc_queue q)
{
    if (q == NULL) return NULL;
    return misc_list_front(q->list);
}


void* misc_queue_back(const misc_queue q)
{
    if (q == NULL) return NULL;
    return misc_list_back(q->list);
}
