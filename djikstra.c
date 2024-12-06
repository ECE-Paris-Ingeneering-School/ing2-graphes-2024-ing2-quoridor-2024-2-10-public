#include "dijkstra.h"
#include "graphe1.h"
#include <stdlib.h>
#include <limits.h>

Dijkstra* dijkstra(Graphe* graphe, int sommetDepart) {
    printf("\nChaine alimentaire pour le sommet : %d\n", sommetDepart);

    Dijkstra* resultat = (Dijkstra*)malloc(sizeof(Dijkstra));
    if (resultat == NULL) {
        fprintf(stderr, "Erreur d'allocation mémoire pour Dijkstra.\n");
        return NULL;
    }

    resultat->distances = (int*)malloc(graphe->sommet * sizeof(int));
    resultat->predecesseurs = (int*)malloc(graphe->sommet * sizeof(int));

    for (int i = 0; i < graphe->sommet; i++) {
        resultat->distances[i] = INT_MAX;
        resultat->predecesseurs[i] = -1;
        graphe->visite[i] = false;
    }
    resultat->distances[sommetDepart] = 0;

    for (int i = 0; i < graphe->sommet; i++) {
        int sommetActuel = -1;
        int minDistance = INT_MAX;

        for (int j = 0; j < graphe->sommet; j++) {
            if (!graphe->visite[j] && !graphe->supprime[j] && resultat->distances[j] < minDistance) {
                minDistance = resultat->distances[j];
                sommetActuel = j;
            }
        }

        if (sommetActuel == -1) {
            break; // Aucun sommet atteignable restant
        }

        graphe->visite[sommetActuel] = true;

        for (int voisin = 0; voisin < graphe->sommet; voisin++) {
            int poidsArete = graphe->adjMatrix[sommetActuel][voisin];
            if (poidsArete != INT_MAX && !graphe->visite[voisin] && !graphe->supprime[voisin]) {
                int nouvelleDistance = resultat->distances[sommetActuel] + poidsArete;
                if (nouvelleDistance < resultat->distances[voisin]) {
                    resultat->distances[voisin] = nouvelleDistance;
                    resultat->predecesseurs[voisin] = sommetActuel;
                }
            }
        }
    }

    return resultat;
}


void afficherResultatsDijkstra(Dijkstra* resultat, int sommetArrivee, Graphe* graphe) {
    if (resultat->distances[sommetArrivee] == INT_MAX) {
        printf("Pas de chemin entre le sommet de depart et le sommet d'arrivee %d.\n", sommetArrivee);
    } else {
        printf("\nDistance minimale jusqu'au sommet %d : %d\n", sommetArrivee, resultat->distances[sommetArrivee]);

        int chemin[graphe->sommet];
        int index = 0;
        int sommetActuel = sommetArrivee;

        while (sommetActuel != -1) {
            chemin[index++] = sommetActuel;
            sommetActuel = resultat->predecesseurs[sommetActuel];
        }

        printf("Chaine alimentaire : ");
        for (int i = index - 1; i >= 0; i--) {
            printf("%d%s", chemin[i], (i == 0) ? "\n" : " -> ");
        }
    }
}

void libererResultatsDijkstra(Dijkstra* resultat, Graphe* graphe) {
    free(resultat->distances);
    free(resultat->predecesseurs);
    free(resultat);
}
