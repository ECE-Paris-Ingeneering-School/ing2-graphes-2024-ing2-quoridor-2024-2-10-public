#include <stdio.h>
#include "dijkstra.h"

// Algorithme de Dijkstra
void dijkstra(Graphe* graphe, int sommet_source) {
    int nombre_sommets = graphe->nombre_sommets;
    int dist[nombre_sommets];  // distances
    int precedents[nombre_sommets];  // précédents sommets dans le plus court chemin
    int visite[nombre_sommets];  // sommets visités

    // Initialisation
    for (int i = 0; i < nombre_sommets; i++) {
        dist[i] = INF;
        precedents[i] = -1;
        visite[i] = 0;
    }
    dist[sommet_source] = 0;

    // Algorithme de Dijkstra
    for (int i = 0; i < nombre_sommets - 1; i++) {

        int min_dist = INF, min_index = -1;
        for (int v = 0; v < nombre_sommets; v++) {
            if (!visite[v] && dist[v] <= min_dist) {
                min_dist = dist[v];
                min_index = v;
            }
        }

        if (min_index == -1) break;

        // Marquer ce sommet comme visité
        visite[min_index] = 1;

        for (int j = 0; j < graphe->nombre_aretes; j++) {
            int u = graphe->aretes[j].source;
            int v = graphe->aretes[j].destination;
            int poids = graphe->aretes[j].poids;

            if (u == min_index && !visite[v] && dist[u] + poids < dist[v]) {
                dist[v] = dist[u] + poids;
                precedents[v] = u;
            }
            if (v == min_index && !visite[u] && dist[v] + poids < dist[u]) {
                dist[u] = dist[v] + poids;
                precedents[u] = v;
            }
        }
    }

    // Afficher les résultats
    printf("Sommet source : %d\n", sommet_source);
    for (int i = 0; i < nombre_sommets; i++) {
        if (i != sommet_source && dist[i] != INF) {
            printf("Plus court chemin vers le sommet %d : distance = %d\n", i, dist[i]);

            // Afficher le chemin du sommet source vers ce sommet
            printf("Chemin : %d", i);
            int precedent = precedents[i];
            while (precedent != -1) {
                printf(" <- %d", precedent);
                precedent = precedents[precedent];
            }
            printf("\n");
        } else if (dist[i] == INF) {
            printf("Sommet %d inaccessible depuis le sommet source %d\n", i, sommet_source);
        }
    }
}
