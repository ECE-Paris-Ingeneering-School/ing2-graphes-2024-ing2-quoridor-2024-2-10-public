#include "dijkstra.h"
#include "graphe1.h"
#include <stdlib.h>
#include <limits.h>

DijkstraResultat* dijkstra(Graphe* graphe, int sommetDepart) {
    printf("Debut de Dijkstra avec sommet de depart : %d\n", sommetDepart + 1);

    DijkstraResultat* resultat = (DijkstraResultat*)malloc(sizeof(DijkstraResultat));
    if (resultat == NULL) {
        fprintf(stderr, "Erreur d allocation memoire pour DijkstraResultat.\n");
        return NULL;
    }

    resultat->distances = (int*)malloc(graphe->numVertices * sizeof(int));
    resultat->predecesseurs = (int*)malloc(graphe->numVertices * sizeof(int));
    if (resultat->distances == NULL || resultat->predecesseurs == NULL) {
        fprintf(stderr, "Erreur d allocation memoire pour les tableaux.\n");
        free(resultat->distances);
        free(resultat->predecesseurs);
        free(resultat);
        return NULL;
    }

    for (int i = 0; i < graphe->numVertices; i++) {
        resultat->distances[i] = INT_MAX;
        resultat->predecesseurs[i] = -1;
        graphe->visited[i] = false;
    }
    resultat->distances[sommetDepart] = 0;

    for (int i = 0; i < graphe->numVertices; i++) {
        int sommetActuel = -1;
        int minDistance = INT_MAX;

        for (int j = 0; j < graphe->numVertices; j++) {
            if (!graphe->visited[j] && resultat->distances[j] < minDistance) {
                minDistance = resultat->distances[j];
                sommetActuel = j;
            }
        }

        if (sommetActuel == -1) {
            printf("Tous les sommets accessibles ont ete traites.\n");
            break;
        }

        graphe->visited[sommetActuel] = true;
        printf("Sommet traite : %d avec distance minimale : %d\n", sommetActuel + 1, minDistance);

        for (int voisin = 0; voisin < graphe->numVertices; voisin++) {
            int poidsArete = graphe->adjMatrix[sommetActuel][voisin];
            if (poidsArete != INT_MAX && !graphe->visited[voisin]) {
                int nouvelleDistance = resultat->distances[sommetActuel] + poidsArete;
                if (nouvelleDistance < resultat->distances[voisin]) {
                    resultat->distances[voisin] = nouvelleDistance;
                    resultat->predecesseurs[voisin] = sommetActuel;
                    printf("Mise a jour : sommet %d via %d avec nouvelle distance : %d\n", voisin + 1, sommetActuel + 1, nouvelleDistance);
                }
            }
        }
    }

    return resultat;
}

void afficherResultatsDijkstra(DijkstraResultat* resultat, int sommetArrivee, Graphe* graphe) {
    if (resultat->distances[sommetArrivee] == INT_MAX) {
        printf("Pas de chemin entre le sommet de depart et le sommet d'arrivee %d.\n", sommetArrivee);
    } else {
        printf("Distance minimale jusqu'au sommet %d : %d\n", sommetArrivee, resultat->distances[sommetArrivee]);

        int chemin[graphe->numVertices];
        int index = 0;
        int sommetActuel = sommetArrivee;

        while (sommetActuel != -1) {
            chemin[index++] = sommetActuel;
            sommetActuel = resultat->predecesseurs[sommetActuel];
        }

        printf("Chemin : ");
        for (int i = index - 1; i >= 0; i--) {
            printf("%d%s", chemin[i], (i == 0) ? "\n" : " -> ");
        }
    }
}

void libererResultatsDijkstra(DijkstraResultat* resultat, Graphe* graphe) {
    free(resultat->distances);
    free(resultat->predecesseurs);
    free(resultat);
}
