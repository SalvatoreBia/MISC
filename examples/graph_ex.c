#include <stdio.h>
#include <string.h>
#include "misc/graph.h"

typedef struct Person
{
    int id;
    char name[20];
} Person;

int main()
{
    // +--------------------------------+
    // | Create a social network graph  |
    // +--------------------------------+

    misc_graph social = misc_graph_create(sizeof(Person), MISC_GRAPH_UNDIRECTED);
    if (social == NULL)
    {
        printf("misc_graph handle allocation failed. Exiting...\n");
        return 1;
    }

    printf("=== Social Network Graph ===\n\n");

    Person people[] = {
        {1, "Alice"},
        {2, "Bob"},
        {3, "Charlie"},
        {4, "Diana"},
        {5, "Eve"}
    };

    size_t nodes[5];
    int status;

    printf("Adding people to the network...\n");
    for (size_t i = 0; i < 5; i++)
    {
        nodes[i] = misc_graph_addnode(social, &people[i], &status);
        if (status != 0)
        {
            printf("Failed to add person %s\n", people[i].name);
            misc_graph_destroy(social);
            return 1;
        }
        printf("  Added: %s (ID: %d)\n", people[i].name, people[i].id);
    }

    printf("\nCreating friendships...\n");
    
    // Alice <-> Bob
    if (misc_graph_link(social, nodes[0], nodes[1]))
        printf("  %s and %s are now friends\n", people[0].name, people[1].name);

    // Alice <-> Charlie
    if (misc_graph_link(social, nodes[0], nodes[2]))
        printf("  %s and %s are now friends\n", people[0].name, people[2].name);

    // Bob <-> Diana
    if (misc_graph_link(social, nodes[1], nodes[3]))
        printf("  %s and %s are now friends\n", people[1].name, people[3].name);

    // Charlie <-> Diana
    if (misc_graph_link(social, nodes[2], nodes[3]))
        printf("  %s and %s are now friends\n", people[2].name, people[3].name);

    // Diana <-> Eve
    if (misc_graph_link(social, nodes[3], nodes[4]))
        printf("  %s and %s are now friends\n", people[3].name, people[4].name);

    printf("\nNetwork statistics:\n");
    printf("  Total people: %zu\n", misc_graph_nodecount(social));
    printf("  Total friendships: %zu\n", misc_graph_linkcount(social));

    printf("\nChecking friendships...\n");
    if (misc_graph_islink(social, nodes[0], nodes[1]))
        printf("  %s and %s are friends\n", people[0].name, people[1].name);

    if (!misc_graph_islink(social, nodes[0], nodes[4]))
        printf("  %s and %s are not friends\n", people[0].name, people[4].name);

    printf("\n%s leaves the network...\n", people[2].name);
    misc_graph_delnode(social, nodes[2]);

    printf("\nUpdated statistics:\n");
    printf("  Total people: %zu\n", misc_graph_nodecount(social));
    printf("  Total friendships: %zu\n", misc_graph_linkcount(social));

    misc_graph_destroy(social);
    return 0;
}
