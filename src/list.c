#include "misc/list.h"
#include <stdlib.h>
#include <string.h>


typedef struct misc_generic_node
{

    void *val;
    struct misc_generic_node *next;
    
} misc_node;


struct misc_generic_single_linked_list
{

    misc_node *head;
    misc_node *tail;
    
    size_t size;
    size_t elem_size;

};


misc_list misc_list_create(size_t elem_size)
{
    if (elem_size == 0) return NULL;

    misc_list list = (misc_list) malloc(sizeof(struct misc_generic_single_linked_list));
    if (list == NULL) return NULL;

    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    list->elem_size = elem_size;
    
    return list;
}


void misc_list_destroy(misc_list list)
{
    misc_list_clear(list);
    if (list != NULL) free((void*)list);
}


void misc_list_clear(misc_list list)
{
    if (list == NULL) return;
    
    misc_node *current = list->head;
    while (current != NULL)
    {
        misc_node *next = current->next;
        if (current->val != NULL) free(current->val);
        free(current);
        current = next;
    }
    
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
}


size_t misc_list_size(const misc_list list)
{
    if (list == NULL) return 0;

    return list->size;
}


int misc_list_isempty(const misc_list list)
{
    if (list == NULL) return -1;

    return list->size == 0;
}


int misc_list_pushfront(misc_list list, const void *elem)
{
    if (list == NULL || elem == NULL) return 0;

    misc_node *new = (misc_node*) malloc(sizeof(misc_node));
    if (new == NULL) return 0;

    new->val = malloc(list->elem_size);
    if (new->val == NULL)
    {
        free(new);
        return 0;
    }

    memcpy(new->val, elem, list->elem_size);
    
    new->next = list->head;
    list->head = new;
    
    if (list->tail == NULL)
    {
        list->tail = new;
    }
    
    list->size++;
    
    return 1;
}


int misc_list_pushback(misc_list list, const void *elem)
{
    if (list == NULL || elem == NULL) return 0;

    misc_node *new = (misc_node*) malloc(sizeof(misc_node));
    if (new == NULL) return 0;

    new->val = malloc(list->elem_size);
    if (new->val == NULL)
    {
        free(new);
        return 0;
    }

    memcpy(new->val, elem, list->elem_size);
    new->next = NULL;

    if (list->tail == NULL)
    {
        list->head = new;
        list->tail = new;
    }
    else
    {
        list->tail->next = new;
        list->tail = new;
    }

    list->size++;

    return 1;
}


void misc_list_popfront(misc_list list, void *out)
{
    if (list == NULL) return;
    if (list->head == NULL) return;

    misc_node *first = list->head;
    
    if (out != NULL)
    {
        memcpy(out, first->val, list->elem_size);
    }
    
    list->head = first->next;
    
    if (list->head == NULL)
    {
        list->tail = NULL;
    }
    
    free(first->val);
    free(first);
    list->size--;
}


void misc_list_popback(misc_list list, void *out)
{
    if (list == NULL) return;
    if (list->head == NULL) return;

    if (list->head->next == NULL)
    {
        if (out != NULL)
        {
            memcpy(out, list->head->val, list->elem_size);
        }
        free(list->head->val);
        free((void*)list->head);
        list->head = NULL;
        list->tail = NULL;
    }
    else
    {
        misc_node *prev = list->head;
        while (prev->next->next != NULL)
        {
            prev = prev->next;
        }
        
        misc_node *last = prev->next;
        if (out != NULL)
        {
            memcpy(out, last->val, list->elem_size);
        }
        
        free(last->val);
        free(last);
        prev->next = NULL;
        list->tail = prev;
    }
    
    list->size--;
}


int misc_list_insert(misc_list list, size_t idx, const void *elem)
{
    if (list == NULL || elem == NULL) return 0;
    if (idx > list->size) return 0;

    if (idx == 0)
    {
        return misc_list_pushfront(list, elem);
    }
    else if (idx == list->size)
    {
        return misc_list_pushback(list, elem);
    }
    else
    {
        misc_node *new = (misc_node*) malloc(sizeof(misc_node));
        if (new == NULL) return 0;

        new->val = malloc(list->elem_size);
        if (new->val == NULL)
        {
            free(new);
            return 0;
        }

        memcpy(new->val, elem, list->elem_size);

        misc_node *prev = list->head;
        for (size_t i = 0; i < idx - 1; ++i)
        {
            prev = prev->next;
        }

        new->next = prev->next;
        prev->next = new;
    }

    list->size++;

    return 1;
}


int misc_list_remove(misc_list list, size_t idx, void *out)
{
    if (list == NULL) return 0;
    if (list->head == NULL) return 0;
    if (idx >= list->size) return 0;

    if (idx == 0)
    {
        misc_list_popfront(list, out);
        return 1;
    }
    else
    {
        misc_node *prev = list->head;
        for (size_t i = 0; i < idx - 1; ++i)
        {
            prev = prev->next;
        }

        misc_node *to_remove = prev->next;
        
        if (out != NULL)
        {
            memcpy(out, to_remove->val, list->elem_size);
        }

        prev->next = to_remove->next;
        
        if (to_remove == list->tail)
        {
            list->tail = prev;
        }
        
        free(to_remove->val);
        free(to_remove);
        list->size--;

        return 1;
    }
}


void* misc_list_get(const misc_list list, size_t idx)
{
    if (list == NULL) return NULL;
    if (idx >= list->size) return NULL;

    misc_node *tmp = list->head;
    for (size_t i = 0; i < idx; ++i)
    {
        tmp = tmp->next;
    }

    return tmp->val;
}

void* misc_list_front(const misc_list list)
{
    if (list == NULL || list->head == NULL) return NULL;
    return list->head->val;
}

void *misc_list_back(const misc_list list)
{
    if (list == NULL || list->tail == NULL) return NULL;
    return list->tail->val;
}
