#ifndef GRAPHE1_H
#define GRAPHE1_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

typedef struct {
    int sommet;
    int** adjMatrix;
    bool* visite;
    bool* supprime;
} Graphe;

Graphe* creerGraphe(int sommet);
void ajouterArete(Graphe* graphe, int src, int dest, int poids);
void libererGraphe(Graphe* graphe);
Graphe* chargerGrapheDepuisFichier(const char* filename);
void sauvegarderGrapheDansFichier(Graphe* graphe, const char* filename);
void rechercherSommetsParticuliers(Graphe* graphe);
int* calculerNiveauxTrophiquesEntiersSansPrecision(Graphe* graphe, float* moyenne);
void afficherNiveauxTrophiquesEntiersSansPrecision(Graphe* graphe, int* niveaux);
void calculerCentralites(Graphe* graphe);
void simulerDisparition(Graphe* graphe, int sommet);
void dfs(Graphe* graphe, int sommetDepart, int* predecesseurs, int* distances, int* chemin, int* cheminIndex);


#endif // GRAPHE1_H
