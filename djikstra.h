#ifndef INGE2_PROJET_DIJKSTRA_H
#define INGE2_PROJET_DIJKSTRA_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include "graphe1.h"

typedef struct {
    int* distances;
    int* predecesseurs;
} Dijkstra;

Dijkstra* dijkstra(Sommet* sommets, int nb_sommets, int** adjMatrix, int sommetDepart);
void afficherResultatsDijkstra(Dijkstra* resultat, Sommet* sommets, int nb_sommets, int sommetArrivee);
void libererResultatsDijkstra(Dijkstra* resultat);

#endif
