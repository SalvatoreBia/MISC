#include "misc/graph.h"
#include "misc/vector.h"
#include "misc/list.h"
#include "misc/stack.h"
#include <stdlib.h>
#include <string.h>


typedef struct misc_edge
{

    size_t target;
    double weight;

} misc_edge;


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


size_t misc_graph_nodecount(const misc_graph g)
{
    if (g == NULL) return 0;
    return misc_vector_length(g->nodes);
}


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

    misc_list neighbors = misc_list_create(sizeof(misc_edge));
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


static void del_indegree_links(misc_graph g, size_t idx)
{
    if (g == NULL) return;
    size_t size = misc_graph_nodecount(g);
    for (size_t i = 0; i < size; ++i)
    {
        if (i != idx)
        {
            misc_gnode *node = (misc_gnode*)misc_vector_get(g->nodes, i);
            if (node != NULL && node->is_active && node->neighbors != NULL)
            {
                size_t list_size = misc_list_size(node->neighbors);
                for (size_t j = 0; j < list_size; ++j)
                {
                    misc_edge *edge = (misc_edge*)misc_list_get(node->neighbors, j);
                    if (edge != NULL && edge->target == idx)
                    {
                        misc_list_remove(node->neighbors, j, NULL);
                        break;
                    }
                }
            }
        }
    }
}


void misc_graph_delnode(misc_graph g, size_t elem)
{
    if (g == NULL) return;

    size_t nnodes = misc_vector_length(g->nodes);
    if (nnodes == 0 || elem >= nnodes) return;

    misc_gnode *delnode = (misc_gnode*)misc_vector_get(g->nodes, elem);
    if (delnode == NULL) return;
    if (!delnode->is_active) return;

    delnode->is_active = 0;
    if (delnode->neighbors != NULL) misc_list_destroy(delnode->neighbors);
    if (delnode->data != NULL) free(delnode->data);

    delnode->neighbors = NULL;
    delnode->data = NULL;

    misc_stack_push(g->ids, &elem);

    del_indegree_links(g, elem);
}


int misc_graph_islink(misc_graph g, size_t elem1, size_t elem2)
{
    if (g == NULL) return 0;

    size_t nnodes = misc_vector_length(g->nodes);
    if (elem1 >= nnodes || elem2 >= nnodes) return 0;

    misc_gnode *n1 = (misc_gnode*)misc_vector_get(g->nodes, elem1);
    if (n1 == NULL || !n1->is_active || n1->neighbors == NULL) return 0;

    int found = 0;
    size_t list_size = misc_list_size(n1->neighbors);
    for (size_t i = 0; i < list_size && !found; ++i)
    {
        misc_edge *edge = (misc_edge*)misc_list_get(n1->neighbors, i);
        if (edge != NULL && edge->target == elem2) found = 1;
    }

    if (g->is_undirected && !found)
    {
        misc_gnode *n2 = (misc_gnode*)misc_vector_get(g->nodes, elem2);
        if (n2 != NULL && n2->is_active && n2->neighbors != NULL)
        {
            size_t list_size2 = misc_list_size(n2->neighbors);
            for (size_t i = 0; i < list_size2 && !found; ++i)
            {
                misc_edge *edge = (misc_edge*)misc_list_get(n2->neighbors, i);
                if (edge != NULL && edge->target == elem1) found = 1;
            }
        }
    }

    return found;
}


static int misc_graph_isnode(misc_graph g, size_t idx)
{
    if (g == NULL) return 0;
    size_t nnodes = misc_vector_length(g->nodes);
    if (idx >= nnodes) return 0;
    misc_gnode *node = (misc_gnode*)misc_vector_get(g->nodes, idx);
    if (node == NULL) return 0;
    return node->is_active;
}


int misc_graph_link(misc_graph g, size_t elem1, size_t elem2)
{
    if (g == NULL) return 0;

    size_t nnodes = misc_vector_length(g->nodes);
    if (elem1 >= nnodes || elem2 >= nnodes) return 0;
    if (!misc_graph_isnode(g, elem1) || !misc_graph_isnode(g, elem2)) return 0;
    if (misc_graph_islink(g, elem1, elem2)) return 1;

    misc_gnode *node1 = (misc_gnode*)misc_vector_get(g->nodes, elem1);
    if (node1 == NULL || node1->neighbors == NULL) return 0;

    misc_edge edge = { .target = elem2, .weight = 1.0 };
    if (!misc_list_pushback(node1->neighbors, &edge)) return 0;

    if (g->is_undirected)
    {
        misc_gnode *node2 = (misc_gnode*)misc_vector_get(g->nodes, elem2);
        if (node2 == NULL || node2->neighbors == NULL)
        {
            size_t list_size = misc_list_size(node1->neighbors);
            for (size_t i = 0; i < list_size; ++i)
            {
                misc_edge *e = (misc_edge*)misc_list_get(node1->neighbors, i);
                if (e != NULL && e->target == elem2)
                {
                    misc_list_remove(node1->neighbors, i, NULL);
                    break;
                }
            }
            return 0;
        }
        misc_edge edge2 = { .target = elem1, .weight = 1.0 };
        if (!misc_list_pushback(node2->neighbors, &edge2))
        {
            size_t list_size = misc_list_size(node1->neighbors);
            for (size_t i = 0; i < list_size; ++i)
            {
                misc_edge *e = (misc_edge*)misc_list_get(node1->neighbors, i);
                if (e != NULL && e->target == elem2)
                {
                    misc_list_remove(node1->neighbors, i, NULL);
                    break;
                }
            }
            return 0;
        }
    }

    return 1;
}


void misc_graph_unlink(misc_graph g, size_t elem1, size_t elem2)
{
    if (g == NULL) return;

    size_t nnodes = misc_graph_nodecount(g);
    if (elem1 >= nnodes || elem2 >= nnodes) return;

    misc_gnode *node1 = (misc_gnode*)misc_vector_get(g->nodes, elem1);
    misc_gnode *node2 = (misc_gnode*)misc_vector_get(g->nodes, elem2);
    if (node1 == NULL || node2 == NULL) return;
    if (!node1->is_active || !node2->is_active) return;

    for (size_t i = 0; i < misc_list_size(node1->neighbors); ++i)
    {
        misc_edge *edge = (misc_edge*)misc_list_get(node1->neighbors, i);
        if (edge != NULL && edge->target == elem2)
        {
            misc_list_remove(node1->neighbors, i, NULL);
            break;
        }
    }

    if (g->is_undirected)
    {
        for (size_t i = 0; i < misc_list_size(node2->neighbors); ++i)
        {
            misc_edge *edge = (misc_edge*)misc_list_get(node2->neighbors, i);
            if (edge != NULL && edge->target == elem1)
            {
                misc_list_remove(node2->neighbors, i, NULL);
                break;
            }
        }
    }
}


int misc_graph_addw(misc_graph g, size_t elem1, size_t elem2, double weight)
{
    if (g == NULL) return 0;

    size_t nnodes = misc_vector_length(g->nodes);
    if (elem1 >= nnodes || elem2 >= nnodes) return 0;
    if (!misc_graph_isnode(g, elem1) || !misc_graph_isnode(g, elem2)) return 0;

    misc_gnode *node1 = (misc_gnode*)misc_vector_get(g->nodes, elem1);
    if (node1 == NULL || node1->neighbors == NULL) return 0;

    size_t list_size = misc_list_size(node1->neighbors);
    for (size_t i = 0; i < list_size; ++i)
    {
        misc_edge *edge = (misc_edge*)misc_list_get(node1->neighbors, i);
        if (edge != NULL && edge->target == elem2)
        {
            edge->weight = weight;
            
            if (g->is_undirected)
            {
                misc_gnode *node2 = (misc_gnode*)misc_vector_get(g->nodes, elem2);
                if (node2 != NULL && node2->neighbors != NULL)
                {
                    size_t list_size2 = misc_list_size(node2->neighbors);
                    for (size_t j = 0; j < list_size2; ++j)
                    {
                        misc_edge *edge2 = (misc_edge*)misc_list_get(node2->neighbors, j);
                        if (edge2 != NULL && edge2->target == elem1)
                        {
                            edge2->weight = weight;
                            break;
                        }
                    }
                }
            }
            return 1;
        }
    }

    misc_edge edge = { .target = elem2, .weight = weight };
    if (!misc_list_pushback(node1->neighbors, &edge)) return 0;

    if (g->is_undirected)
    {
        misc_gnode *node2 = (misc_gnode*)misc_vector_get(g->nodes, elem2);
        if (node2 == NULL || node2->neighbors == NULL)
        {
            list_size = misc_list_size(node1->neighbors);
            for (size_t i = 0; i < list_size; ++i)
            {
                misc_edge *e = (misc_edge*)misc_list_get(node1->neighbors, i);
                if (e != NULL && e->target == elem2)
                {
                    misc_list_remove(node1->neighbors, i, NULL);
                    break;
                }
            }
            return 0;
        }
        misc_edge edge2 = { .target = elem1, .weight = weight };
        if (!misc_list_pushback(node2->neighbors, &edge2))
        {
            list_size = misc_list_size(node1->neighbors);
            for (size_t i = 0; i < list_size; ++i)
            {
                misc_edge *e = (misc_edge*)misc_list_get(node1->neighbors, i);
                if (e != NULL && e->target == elem2)
                {
                    misc_list_remove(node1->neighbors, i, NULL);
                    break;
                }
            }
            return 0;
        }
    }

    return 1;
}


double misc_graph_getw(misc_graph g, size_t elem1, size_t elem2, int *status)
{
    if (g == NULL || !misc_graph_islink(g, elem1, elem2))
    {
        if (status != NULL) *status = -1;
        return 0.0;
    }

    misc_gnode *node1 = (misc_gnode*)misc_vector_get(g->nodes, elem1);
    if (node1 == NULL || node1->neighbors == NULL)
    {
        if (status != NULL) *status = -1;
        return 0.0;
    }

    size_t list_size = misc_list_size(node1->neighbors);
    for (size_t i = 0; i < list_size; ++i)
    {
        misc_edge *edge = (misc_edge*)misc_list_get(node1->neighbors, i);
        if (edge != NULL && edge->target == elem2)
        {
            if (status != NULL) *status = 0;
            return edge->weight;
        }
    }

    if (status != NULL) *status = -1;
    return 0.0;
}


size_t misc_graph_linkcount(const misc_graph g)
{
    if (g == NULL) return 0;

    size_t tot = 0;
    for (size_t i = 0; i < misc_vector_length(g->nodes); ++i)
    {
        misc_gnode *node = (misc_gnode*)misc_vector_get(g->nodes, i);
        if (node == NULL) return 0;

        tot += misc_list_size(node->neighbors);
    }

    return tot;
}
