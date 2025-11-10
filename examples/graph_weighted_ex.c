#include <stdio.h>
#include <stdlib.h>
#include "misc/graph.h"

int main(void)
{
    printf("=== Esempio di Grafo Pesato ===\n\n");

    // Crea un grafo orientato e pesato
    misc_graph g = misc_graph_create(sizeof(int), MISC_GRAPH_DIRECTED | MISC_GRAPH_WEIGHTED);
    if (g == NULL)
    {
        fprintf(stderr, "Errore nella creazione del grafo\n");
        return 1;
    }

    // Aggiungi alcuni nodi (città)
    int cities[] = {1, 2, 3, 4}; // Roma, Milano, Napoli, Torino
    const char *city_names[] = {"Roma", "Milano", "Napoli", "Torino"};
    size_t nodes[4];
    
    for (int i = 0; i < 4; i++)
    {
        int status;
        nodes[i] = misc_graph_addnode(g, &cities[i], &status);
        if (status != 0)
        {
            fprintf(stderr, "Errore nell'aggiunta del nodo %d\n", i);
            misc_graph_destroy(g);
            return 1;
        }
        printf("Aggiunto nodo %zu: %s\n", nodes[i], city_names[i]);
    }

    printf("\n=== Aggiunta Collegamenti con Pesi (distanze in km) ===\n");

    // Aggiungi collegamenti pesati (distanze tra città)
    // Roma -> Milano: 570 km
    if (misc_graph_addw(g, nodes[0], nodes[1], 570.0))
        printf("Roma -> Milano: 570 km\n");

    // Roma -> Napoli: 225 km
    if (misc_graph_addw(g, nodes[0], nodes[2], 225.0))
        printf("Roma -> Napoli: 225 km\n");

    // Milano -> Torino: 140 km
    if (misc_graph_addw(g, nodes[1], nodes[3], 140.0))
        printf("Milano -> Torino: 140 km\n");

    // Napoli -> Roma: 225 km (direzione opposta)
    if (misc_graph_addw(g, nodes[2], nodes[0], 225.0))
        printf("Napoli -> Roma: 225 km\n");

    printf("\n=== Verifica Collegamenti e Pesi ===\n");

    // Verifica se esistono i collegamenti e stampa i pesi
    int status;
    if (misc_graph_islink(g, nodes[0], nodes[1]))
    {
        double weight = misc_graph_getw(g, nodes[0], nodes[1], &status);
        if (status == 0)
            printf("Distanza Roma -> Milano: %.1f km\n", weight);
    }

    if (misc_graph_islink(g, nodes[0], nodes[2]))
    {
        double weight = misc_graph_getw(g, nodes[0], nodes[2], &status);
        if (status == 0)
            printf("Distanza Roma -> Napoli: %.1f km\n", weight);
    }

    if (misc_graph_islink(g, nodes[1], nodes[3]))
    {
        double weight = misc_graph_getw(g, nodes[1], nodes[3], &status);
        if (status == 0)
            printf("Distanza Milano -> Torino: %.1f km\n", weight);
    }

    // Verifica un collegamento che non esiste
    if (!misc_graph_islink(g, nodes[3], nodes[0]))
    {
        printf("Torino -> Roma: collegamento non esistente\n");
    }

    printf("\n=== Aggiornamento Peso ===\n");

    // Aggiorna il peso di un collegamento esistente
    if (misc_graph_addw(g, nodes[0], nodes[1], 580.0))
    {
        double weight = misc_graph_getw(g, nodes[0], nodes[1], &status);
        if (status == 0)
            printf("Distanza aggiornata Roma -> Milano: %.1f km\n", weight);
    }

    printf("\n=== Grafo Non Orientato con Pesi ===\n");

    // Crea un grafo non orientato e pesato
    misc_graph g2 = misc_graph_create(sizeof(int), MISC_GRAPH_UNDIRECTED | MISC_GRAPH_WEIGHTED);
    if (g2 == NULL)
    {
        fprintf(stderr, "Errore nella creazione del grafo non orientato\n");
        misc_graph_destroy(g);
        return 1;
    }

    // Aggiungi nodi
    size_t nodes2[3];
    for (int i = 0; i < 3; i++)
    {
        nodes2[i] = misc_graph_addnode(g2, &cities[i], &status);
    }

    // Aggiungi collegamento con peso (automaticamente bidirezionale)
    if (misc_graph_addw(g2, nodes2[0], nodes2[1], 100.0))
    {
        printf("Aggiunto collegamento bidirezionale con peso 100.0\n");
        
        double w1 = misc_graph_getw(g2, nodes2[0], nodes2[1], &status);
        double w2 = misc_graph_getw(g2, nodes2[1], nodes2[0], &status);
        
        printf("Peso 0->1: %.1f\n", w1);
        printf("Peso 1->0: %.1f\n", w2);
    }

    // Cleanup
    misc_graph_destroy(g);
    misc_graph_destroy(g2);

    printf("\n=== Fine Esempio ===\n");
    return 0;
}
