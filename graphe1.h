
#ifndef INGE2_PROJET_GRAPHE1_H
#define INGE2_PROJET_GRAPHE1_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
typedef struct {
    char nom[50];
    int population;
    float taux_croissance;
} Sommet;

// Définition de la structure pour les arcs
typedef struct {
    char predateur[50];
    char proie[50];
    int interaction;
} GrapheArc;

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

Sommet* lireSommets(FILE* fichier, int* nb_sommets);
GrapheArc* lireArcs(FILE* fichier, int* nb_arcs);
int** creerMatriceAdjacence(int nb_sommets);
void ajouterArete(int** adjMatrix, Sommet* sommets, int nb_sommets, const char* src, const char* dest, int poids);
void sauvegarderGrapheDansFichier(Sommet* sommets, int nb_sommets, int** adjMatrix, const char* filename);
void rechercherSommetsParticuliers(Sommet* sommets, int nb_sommets, int** adjMatrix);
int* calculerNiveauxTrophiquesEntiersSansPrecision(Sommet* sommets, int nb_sommets, int** adjMatrix, float* moyenne);
void afficherNiveauxTrophiquesEntiersSansPrecision(Sommet* sommets, int nb_sommets, int* niveaux);
void calculerCentralites(Sommet* sommets, int nb_sommets, int** adjMatrix);
void simulerDisparition(Sommet* sommets, int nb_sommets, int** adjMatrix, int sommet);
void dfs(Sommet* sommets, int nb_sommets, int** adjMatrix, int sommetDepart,bool* visite, int* predecesseurs, int* distances, int* chemin, int* cheminIndex);
void simulerDynamiquePopulation(Sommet* sommets, int nb_sommets, int** adjMatrix);
void dfsConnexite(int** adjMatrix, int nb_sommets, bool* visite, int sommet);
bool verifierConnexite(int** adjMatrix, int nb_sommets);

#endif //INGE2_PROJET_GRAPHE1_H
