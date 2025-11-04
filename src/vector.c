#include "misc/vector.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>


struct misc_generic_vector
{
    
    void *data;

    size_t length;
    size_t capacity;
    size_t elem_size;

};


misc_vector misc_vector_create(size_t elem_size)
{
    if (elem_size == 0) return NULL;

    misc_vector vec = (misc_vector) malloc(sizeof(struct misc_generic_vector));
    if (vec == NULL) return NULL;

    void *data = malloc(DEFAULT_CAPACITY * elem_size);
    if (data == NULL)
    {
        free((void*)vec);
        return NULL;
    }

    vec->data = data;
    vec->length = 0;
    vec->capacity = DEFAULT_CAPACITY;
    vec->elem_size = elem_size;

    return vec;
}


void misc_vector_destroy(misc_vector vec)
{
    if (vec != NULL)
    {
        free(vec->data);
        free((void*)vec);
    }
}


void misc_vector_clear(misc_vector vec)
{
    if (vec != NULL) vec->length = 0;
}


size_t misc_vector_length(const misc_vector vec)
{
    return vec->length;
}


size_t misc_vector_capacity(const misc_vector vec)
{
    return vec->capacity;
}


int misc_vector_isempty(const misc_vector vec)
{
    return vec->length == 0;
}


int misc_vector_pushback(misc_vector vec, const void *elem)
{
    if (vec == NULL || elem == NULL) return 0;

    if (vec->length == vec->capacity)
    {
        size_t new_cap = vec->capacity * 2;
        void *new_data = malloc(new_cap * vec->elem_size);
        if (new_data == NULL) return 0;

        memcpy(new_data, vec->data, vec->length * vec->elem_size);
        free(vec->data);
        vec->data = new_data;
        vec->capacity = new_cap;
    }

    memcpy((uint8_t*)vec->data + (vec->length*vec->elem_size), elem, vec->elem_size);
    vec->length++;
    return 1;
}


void misc_vector_popback(misc_vector vec, void *out)
{
    if (vec == NULL || vec->length == 0) return;

    vec->length--;
    if (out != NULL)
    {
        memcpy(out, (uint8_t*)vec->data + (vec->elem_size*vec->length), vec->elem_size);
    }
}


int misc_vector_insert(misc_vector vec, size_t idx, const void *elem)
{
    if (vec == NULL || elem == NULL) return 0;
    if (idx > vec->length) return 0;

    if (vec->length == vec->capacity)
    {
        size_t new_cap = vec->capacity * 2;
        void *new_data = malloc(new_cap * vec->elem_size);
        if (new_data == NULL) return 0;

        memcpy(new_data, vec->data, vec->length * vec->elem_size);
        free(vec->data);
        vec->data = new_data;
        vec->capacity = new_cap;
    }

    if (idx < vec->length)
    {
        void *dest = (uint8_t*)vec->data +((idx + 1)* vec->elem_size);
        void *src  = (uint8_t*)vec->data + (idx * vec->elem_size);
        size_t bytes = (vec->length - idx) * vec->elem_size;

        memmove(dest, src, bytes);
    }

    void *insert_pos = (uint8_t*)vec->data + (idx * vec->elem_size);
    memcpy(insert_pos, elem, vec->elem_size);
    vec->length++;

    return 1;
}


int misc_vector_remove(misc_vector vec, size_t idx, void *out)
{
    if (vec == NULL) return 0;
    if (vec->length == 0) return 0;
    if (idx >= vec->length) return 0;

    if (out != NULL)
    {
        void *elem_pos = (uint8_t*)vec->data + (idx * vec->elem_size);
        memcpy(out, elem_pos, vec->elem_size);
    }

    if (idx < vec->length - 1)
    {
        void *dest = (uint8_t*)vec->data + (idx * vec->elem_size);
        void *src = (uint8_t*)vec->data + ((idx + 1) * vec->elem_size);
        size_t num_bytes = (vec->length - idx - 1) * vec->elem_size;
        
        memmove(dest, src, num_bytes);
    }

    vec->length--;
    
    return 1;
}


void misc_vector_set(misc_vector vec, size_t idx, const void *elem)
{
    if (vec == NULL || elem == NULL) return;
    if (idx >= vec->length) return;

    memcpy((uint8_t*)vec->data + (idx * vec->elem_size), elem, vec->elem_size);
}


void* misc_vector_get(const misc_vector vec, size_t idx)
{
    if (vec == NULL) return NULL;
    if (vec->length == 0) return NULL;
    if (idx >= vec->length) return NULL;

    return ((uint8_t*)vec->data + (idx * vec->elem_size));
}


void* misc_vector_front(const misc_vector vec)
{
    return misc_vector_get(vec, 0);
}


void* misc_vector_back(const misc_vector vec)
{
    if (vec->length == 0) return NULL;
    return misc_vector_get(vec, vec->length-1);
}