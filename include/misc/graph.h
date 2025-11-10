#pragma once
#ifndef GRAPH_H
#define GRAPH_H

#include <stddef.h>

/**
 * @brief Opaque handle to a graph instance.
 */
typedef struct misc_generic_graph* misc_graph;

/**
 * @brief Flag for creating a directed graph (default behavior).
 */
#define MISC_GRAPH_DIRECTED   0

/**
 * @brief Flag for creating an undirected graph.
 */
#define MISC_GRAPH_UNDIRECTED 1

/**
 * @brief Flag for creating a weighted graph.
 */
#define MISC_GRAPH_WEIGHTED   2

/**
 * @brief Creates a new graph with the specified element size and flags.
 * @param elem_size Size in bytes of each node's data
 * @param flags Graph configuration flags (can be OR'd together):
 *              - MISC_GRAPH_DIRECTED: creates a directed graph
 *              - MISC_GRAPH_UNDIRECTED: creates an undirected graph
 *              - MISC_GRAPH_WEIGHTED: enables edge weights
 * @return Pointer to the new graph, or NULL on allocation failure
 */
misc_graph misc_graph_create(size_t elem_size, int flags);

/**
 * @brief Destroys the graph and frees all associated memory.
 * @param g Graph to destroy
 * @note The graph handle becomes invalid after this call.
 */
void misc_graph_destroy(misc_graph g);

/**
 * @brief Adds a new node to the graph with the specified data.
 * @param g Graph to modify
 * @param data Pointer to the data to store in the node
 * @param status Optional pointer to status code (0 on success, non-zero on failure)
 * @return Unique identifier for the newly created node
 */
size_t misc_graph_addnode(misc_graph g, const void* data, int *status);

/**
 * @brief Removes a node from the graph.
 * @param g Graph to modify
 * @param elem Identifier of the node to remove
 * @note All edges connected to this node are also removed.
 */
void misc_graph_delnode(misc_graph g, size_t elem);

/**
 * @brief Creates an edge between two nodes.
 * @param g Graph to modify
 * @param elem1 Identifier of the first node
 * @param elem2 Identifier of the second node
 * @return 1 on success, 0 on failure
 * @note For undirected graphs, the edge is bidirectional.
 */
int misc_graph_link(misc_graph g, size_t elem1, size_t elem2);

/**
 * @brief Removes an edge between two nodes.
 * @param g Graph to modify
 * @param elem1 Identifier of the first node
 * @param elem2 Identifier of the second node
 * @note For undirected graphs, removes the edge in both directions.
 */
void misc_graph_unlink(misc_graph g, size_t elem1, size_t elem2);

/**
 * @brief Adds or updates a weighted edge between two nodes.
 * @param g Graph to modify
 * @param elem1 Identifier of the first node
 * @param elem2 Identifier of the second node
 * @param weight Weight value for the edge
 * @return 1 on success, 0 on failure
 * @note The graph must have been created with MISC_GRAPH_WEIGHTED flag.
 *       For undirected graphs, sets the weight for both directions.
 */
int misc_graph_addw(misc_graph g, size_t elem1, size_t elem2, double weight);

/**
 * @brief Retrieves the weight of an edge between two nodes.
 * @param g Graph to query
 * @param elem1 Identifier of the first node
 * @param elem2 Identifier of the second node
 * @param status Optional pointer to status code (0 on success, non-zero on failure)
 * @return Weight of the edge, or 0.0 if the edge doesn't exist
 * @note The graph must have been created with MISC_GRAPH_WEIGHTED flag.
 */
double misc_graph_getw(misc_graph g, size_t elem1, size_t elem2, int *status);

/**
 * @brief Checks if an edge exists between two nodes.
 * @param g Graph to query
 * @param elem1 Identifier of the first node
 * @param elem2 Identifier of the second node
 * @return 1 if the edge exists, 0 otherwise
 */
int misc_graph_islink(misc_graph g, size_t elem1, size_t elem2);

/**
 * @brief Returns the number of nodes in the graph.
 * @param g Graph to query
 * @return Number of nodes in the graph
 */
size_t misc_graph_nodecount(const misc_graph g);

/**
 * @brief Returns the number of edges in the graph.
 * @param g Graph to query
 * @return Number of edges in the graph
 */
size_t misc_graph_linkcount(const misc_graph g);


#endif /* GRAPH_H */
