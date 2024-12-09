#include "graphe1.h"
#include "dijkstra.h"
#include <stdio.h>
#include <stdlib.h>

Dijkstra* dijkstra(Sommet* sommets, int nb_sommets, int** adjMatrix, int sommetDepart) {
    printf("\nChaine alimentaire pour le sommet : %s\n", sommets[sommetDepart].nom);

    Dijkstra* resultat = (Dijkstra*)malloc(sizeof(Dijkstra));
    if (resultat == NULL) {
        fprintf(stderr, "Erreur d'allocation memoire pour Dijkstra.\n");
        return NULL;
    }

    resultat->distances = (int*)malloc(nb_sommets * sizeof(int));
    resultat->predecesseurs = (int*)malloc(nb_sommets * sizeof(int));
    bool* visite = (bool*)calloc(nb_sommets, sizeof(bool));

    for (int i = 0; i < nb_sommets; i++) {
        resultat->distances[i] = INT_MAX;
        resultat->predecesseurs[i] = -1;
    }
    resultat->distances[sommetDepart] = 0;

    for (int i = 0; i < nb_sommets; i++) {
        int sommetActuel = -1;
        int minDistance = INT_MAX;

        // Trouver le sommet avec la plus petite distance non encore visite
        for (int j = 0; j < nb_sommets; j++) {
            if (!visite[j] && resultat->distances[j] < minDistance) {
                minDistance = resultat->distances[j];
                sommetActuel = j;
            }
        }

        if (sommetActuel == -1) break; // Plus de sommets atteignables

        visite[sommetActuel] = true;

        // Mettre a jour les distances des voisins
        for (int voisin = 0; voisin < nb_sommets; voisin++) {
            int poidsArete = adjMatrix[sommetActuel][voisin];
            if (poidsArete != INT_MAX && !visite[voisin]) {
                int nouvelleDistance = resultat->distances[sommetActuel] + poidsArete;
                if (nouvelleDistance < resultat->distances[voisin]) {
                    resultat->distances[voisin] = nouvelleDistance;
                    resultat->predecesseurs[voisin] = sommetActuel;
                }
            }
        }
    }

    free(visite);
    return resultat;
}

void afficherResultatsDijkstra(Dijkstra* resultat, Sommet* sommets, int nb_sommets, int sommetArrivee) {
    if (resultat->distances[sommetArrivee] == INT_MAX) {
        printf("Pas de chemin entre le sommet de depart et le sommet d'arrivee %s.\n", sommets[sommetArrivee].nom);
    } else {
        printf("\nDistance minimale jusqu'au sommet %s : %d\n", sommets[sommetArrivee].nom, resultat->distances[sommetArrivee]);

        int chemin[nb_sommets];
        int index = 0;
        int sommetActuel = sommetArrivee;

        while (sommetActuel != -1) {
            chemin[index++] = sommetActuel;
            sommetActuel = resultat->predecesseurs[sommetActuel];
        }

        printf("Chaine alimentaire : ");
        for (int i = index - 1; i >= 0; i--) {
            printf("%s%s", sommets[chemin[i]].nom, (i == 0) ? "\n" : " -> ");
        }
    }
}

void libererResultatsDijkstra(Dijkstra* resultat) {
    free(resultat->distances);
    free(resultat->predecesseurs);
    free(resultat);
}
