#include "misc/graph.h"
#include "misc/vector.h"
#include "misc/list.h"
#include "misc/stack.h"
#include <stdlib.h>
#include <string.h>


typedef struct misc_generic_graphnode
{

    int is_active;
    void *data;
    misc_list neighbors;

} misc_gnode;


struct misc_generic_graph
{

    misc_vector nodes;
    misc_stack  ids;

    int is_undirected;
    int is_weighted;

    size_t elem_size;

};


misc_graph misc_graph_create(size_t elem_size, int flags)
{
    if (elem_size == 0) return NULL;

    misc_graph g = (misc_graph) malloc(sizeof(struct misc_generic_graph));
    if (g == NULL) return NULL;

    g->is_undirected = flags & MISC_GRAPH_UNDIRECTED;
    g->is_weighted   = flags & MISC_GRAPH_WEIGHTED;
    g->elem_size     = elem_size;

    g->nodes = misc_vector_create(sizeof(misc_gnode));
    if (g->nodes == NULL)
    {
        free((void*)g);
        return NULL;
    }

    g->ids = misc_stack_create(sizeof(size_t));
    if (g->ids == NULL)
    {
        misc_vector_destroy(g->nodes);
        free((void*)g);
        return NULL;
    }

    return g;
}


void misc_graph_destroy(misc_graph g)
{
    if (g != NULL)
    {
        if (g->nodes != NULL)
        {
            size_t node_count = misc_vector_length(g->nodes);
            for (size_t i = 0; i < node_count; ++i)
            {
                misc_gnode *node = (misc_gnode*) misc_vector_get(g->nodes, i);
                
                if (node != NULL && node->is_active)
                {
                    if (node->data != NULL) free(node->data);
                    if (node->neighbors != NULL) misc_list_destroy(node->neighbors);
                }
            }
            misc_vector_destroy(g->nodes);
        }

        if (g->ids != NULL) misc_stack_destroy(g->ids);

        free((void*)g);
    }
}


size_t misc_graph_addnode(misc_graph g, const void *data, int *status)
{
    if (g == NULL || data == NULL) goto status_fail;

    void *val = malloc(g->elem_size);
    if (val == NULL) goto status_fail;

    memcpy(val, data, g->elem_size);

    misc_list neighbors = misc_list_create(sizeof(misc_gnode));
    if (neighbors == NULL)
    {
        free(val);
        goto status_fail;
    }

    misc_gnode new_node = { .is_active = 1, .data = val, .neighbors = neighbors };

    size_t stack_size = misc_stack_size(g->ids);
    if (stack_size == 0)
    {
        size_t new_id = misc_vector_length(g->nodes);
        if (!misc_vector_pushback(g->nodes, &new_node))
        {
            misc_list_destroy(neighbors);
            free(val);
            goto status_fail;
        }
        
        if (status != NULL) *status = 0;
        return new_id;
    }
    else
    {
        size_t id;
        if (!misc_stack_pop(g->ids, &id))
        {
            misc_list_destroy(neighbors);
            free(val);
            goto status_fail;
        }
        
        misc_vector_set(g->nodes, id, &new_node);

        if (status != NULL) *status = 0;
        return id;
    }


status_fail:
    if (status != NULL) *status = -1;
    return (size_t)-1;
}