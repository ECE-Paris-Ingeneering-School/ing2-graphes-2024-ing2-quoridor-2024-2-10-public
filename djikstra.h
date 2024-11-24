#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#define INF 1000000  // Représente une valeur infinie pour les distances non atteignables

// Structure pour représenter une arête dans le graphe
typedef struct {
    int source;
    int destination;
    int poids;
} Arete;

// Structure pour représenter un graphe
typedef struct {
    int nombre_sommets;
    int nombre_aretes;
    Arete* aretes;
} Graphe;

// Fonction pour trouver le plus court chemin à partir du sommet source avec l'algorithme de Dijkstra
void dijkstra(Graphe* graphe, int sommet_source);

#endif
