#include "misc/stack.h"
#include "misc/vector.h"
#include <stdlib.h>

struct misc_generic_stack
{
    misc_vector vec;
};


misc_stack misc_stack_create(size_t elem_size)
{
    if (elem_size == 0) return NULL;

    misc_stack st = (misc_stack) malloc(sizeof(struct misc_generic_stack));
    if (st == NULL) return NULL;

    st->vec = misc_vector_create(elem_size);
    if (st->vec == NULL)
    {
        free((void*)st);
        return NULL;
    }

    return st;
}


void misc_stack_destroy(misc_stack st)
{
    if (st != NULL)
    {
        misc_vector_destroy(st->vec);
        free((void*)st);
    }
}


void misc_stack_clear(misc_stack st)
{
    misc_vector_clear(st->vec);
}


size_t misc_stack_size(const misc_stack st)
{
    return misc_vector_length(st->vec);
}


int misc_stack_isempty(const misc_stack st)
{
    return misc_vector_isempty(st->vec);
}


int misc_stack_push(misc_stack st, const void *elem)
{
    return misc_vector_pushback(st->vec, elem);
}


int misc_stack_pop(misc_stack st, void *out)
{
    if (misc_stack_size(st) == 0) return 0;

    misc_vector_popback(st->vec, out);
    return 1;
}


void* misc_stack_top(const misc_stack st)
{
    return misc_vector_back(st->vec);
}