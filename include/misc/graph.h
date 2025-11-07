#pragma once
#ifndef GRAPH_H
#define GRAPH_H

#include <stddef.h>


typedef struct misc_generic_graph* misc_graph;


#define MISC_GRAPH_DIRECTED   0
#define MISC_GRAPH_UNDIRECTED 1
#define MISC_GRAPH_WEIGHTED   2

misc_graph misc_graph_create(size_t elem_size, int flags);

void misc_graph_destroy(misc_graph g);

size_t misc_graph_addnode(misc_graph g, const void* data, int *status);

void misc_graph_delnode(misc_graph g, size_t elem);

int misc_graph_link(misc_graph g, size_t elem1, size_t elem2);

void misc_graph_unlink(misc_graph g, size_t elem1, size_t elem2);

int misc_graph_addw(misc_graph g, size_t elem1, size_t elem2, double weight);

int misc_graph_islink(misc_graph g, size_t elem1, size_t elem2);

size_t misc_graph_nodecount(const misc_graph g);

size_t misc_graph_linkcount(const misc_graph g);


#endif /* GRAPH_H */