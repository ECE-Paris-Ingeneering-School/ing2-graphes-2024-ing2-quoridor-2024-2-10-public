#ifndef GRAPHE1_H
#define GRAPHE1_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

// Structure du graphe
typedef struct {
    int numVertices;      // Le nombre de sommets
    int** adjMatrix;      // La matrice d'adjacence (poids des aretes)
    bool* visited;        // Les marqueurs pour DFS
} Graphe;

// Fonctions de gestion du graphe
Graphe* creerGraphe(int numVertices);
void ajouterArete(Graphe* graphe, int src, int dest, int poids);
void libererGraphe(Graphe* graphe);

// DFS
void dfs(Graphe* graphe, int startVertex, int* predecesseurs, int* distances, int* chemin, int* cheminIndex);

#endif // GRAPHE1_H
