#ifndef PROJET_INGE2_DIJKSTRA_H
#define PROJET_INGE2_DIJKSTRA_H

#include "graphe1.h"

typedef struct {
    int* distances;
    int* predecesseurs;
} DijkstraResultat;

DijkstraResultat* dijkstra(Graphe* graphe, int sommetDepart);
void afficherResultatsDijkstra(DijkstraResultat* resultat, int sommetArrivee, Graphe* graphe);
void libererResultatsDijkstra(DijkstraResultat* resultat, Graphe* graphe);

#endif //PROJET_INGE2_DIJKSTRA_H
