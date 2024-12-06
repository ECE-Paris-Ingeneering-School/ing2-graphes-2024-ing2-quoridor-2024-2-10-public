#ifndef PROJET_INGE2_DIJKSTRA_H
#define PROJET_INGE2_DIJKSTRA_H

#include "graphe1.h"

typedef struct {
    int* distances;
    int* predecesseurs;
} Dijkstra;

Dijkstra* dijkstra(Graphe* graphe, int sommetDepart);
void afficherResultatsDijkstra(Dijkstra* resultat, int sommetArrivee, Graphe* graphe);
void libererResultatsDijkstra(Dijkstra* resultat, Graphe* graphe);

#endif //PROJET_INGE2_DIJKSTRA_H
