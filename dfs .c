#include "graphe1.h"

Graphe* creerGraphe(int sommet) {
    Graphe* graphe = (Graphe*)malloc(sizeof(Graphe));
    if (graphe == NULL) {
        fprintf(stderr, "Erreur : Allocation mémoire pour le graphe échouée.\n");
        return NULL;
    }

    graphe->sommet = sommet;
    graphe->visite = (bool*)calloc(sommet, sizeof(bool)); // Initialisation à false
    graphe->supprime = (bool*)calloc(sommet, sizeof(bool)); // Initialisation à false
    graphe->adjMatrix = (int**)malloc(sommet * sizeof(int*));

    for (int i = 0; i < sommet; i++) {
        graphe->adjMatrix[i] = (int*)calloc(sommet, sizeof(int));
        for (int j = 0; j < sommet; j++) {
            graphe->adjMatrix[i][j] = INT_MAX; // Initialisation à "aucune arête"
        }
    }

    return graphe;
}

void ajouterArete(Graphe* graphe, int src, int dest, int poids) {
    if (src >= 0 && src < graphe->sommet && dest >= 0 && dest < graphe->sommet) {
        graphe->adjMatrix[src][dest] = poids; // Ajouter uniquement l'arc orienté src -> dest
    } else {
        fprintf(stderr, "Erreur : Indices de sommets invalides (%d -> %d).\n", src, dest);
    }
}


void libererGraphe(Graphe* graphe) {
    if (graphe != NULL) {
        for (int i = 0; i < graphe->sommet; i++) {
            free(graphe->adjMatrix[i]);
        }
        free(graphe->adjMatrix);
        free(graphe->visite);
        free(graphe->supprime);
        free(graphe);
    }
}


Graphe* chargerGrapheDepuisFichier(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Erreur : Impossible d'ouvrir le fichier '%s'.\n", filename);
        return NULL;
    }

    int numVertices, numEdges;
    if (fscanf(file, "%d %d", &numVertices, &numEdges) != 2) {
        fprintf(stderr, "Erreur : Format incorrect dans le fichier '%s'.\n", filename);
        fclose(file);
        return NULL;
    }

    Graphe* graphe = creerGraphe(numVertices);
    if (graphe == NULL) {
        fclose(file);
        return NULL;
    }

    for (int i = 0; i < numEdges; i++) {
        int src, dest, poids;
        if (fscanf(file, "%d %d %d", &src, &dest, &poids) != 3) {
            fprintf(stderr, "Erreur : Format incorrect pour les aretes dans le fichier '%s'.\n", filename);
            libererGraphe(graphe);
            fclose(file);
            return NULL;
        }
        ajouterArete(graphe, src, dest, poids);
    }

    fclose(file);
    return graphe;
}
void sauvegarderGrapheDansFichier(Graphe* graphe, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        fprintf(stderr, "Erreur : Impossible d'ouvrir le fichier '%s' en ecriture.\n", filename);
        return;
    }

    int nbAretes = 0;

    // Compter les aretes restantes
    for (int i = 0; i < graphe->sommet; i++) {
        if (graphe->supprime && graphe->supprime[i]) continue; // Ignorer les sommets supprimes
        for (int j = 0; j < graphe->sommet; j++) {
            if (graphe->adjMatrix[i][j] != INT_MAX && (!graphe->supprime || !graphe->supprime[j])) {
                nbAretes++;
            }
        }
    }

    // Ecrire le nombre de sommets et d'aretes
    fprintf(file, "%d %d\n", graphe->sommet, nbAretes);

    // Ecrire les aretes restantes
    for (int i = 0; i < graphe->sommet; i++) {
        if (graphe->supprime && graphe->supprime[i]) continue; // Ignorer les sommets supprimes
        for (int j = 0; j < graphe->sommet; j++) {
            if (graphe->adjMatrix[i][j] != INT_MAX && (!graphe->supprime || !graphe->supprime[j])) {
                fprintf(file, "%d %d %d\n", i, j, graphe->adjMatrix[i][j]);
            }
        }
    }

    fclose(file);
    printf("Graphe sauvegarde dans le fichier '%s' avec %d aretes restantes.\n", filename, nbAretes);
}


void rechercherSommetsParticuliers(Graphe* graphe) {
    printf("\nRecherche de sommets particuliers :\n");

    // Liste des premiers maillons (aucun arc entrant)
    printf("Derniers maillons (especes sans predateurs) : ");
    for (int i = 1; i < graphe->sommet; i++) {
        bool aucunEntrant = true;
        for (int j = 0; j < graphe->sommet; j++) {
            if (graphe->adjMatrix[j][i] != INT_MAX) {
                aucunEntrant = false;
                break;
            }
        }
        if (aucunEntrant) {
            printf("%d ", i); // Décalage pour afficher les sommets à partir de 1
        }
    }

    // Liste des derniers maillons (aucun arc sortant)
    printf("\nPremiers maillons (producteurs primaires) : ");
    for (int i = 1; i < graphe->sommet; i++) {
        bool aucunSortant = true;
        for (int j = 0; j < graphe->sommet; j++) {
            if (graphe->adjMatrix[i][j] != INT_MAX) {
                aucunSortant = false;
                break;
            }
        }
        if (aucunSortant) {
            printf("%d ", i); // Décalage pour afficher les sommets à partir de 1
        }
    }

    // Liste des sommets ayant une seule source d’alimentation
    printf("\nEspeces ayant une seule source alimentation : ");
    for (int i = 1; i < graphe->sommet; i++) {
        int compteurEntrants = 0;
        for (int j = 0; j < graphe->sommet; j++) {
            if (graphe->adjMatrix[j][i] != INT_MAX) {
                compteurEntrants++;
            }
        }
        if (compteurEntrants == 1) {
            printf("%d ", i); // Décalage pour afficher les sommets à partir de 1
        }
    }

    printf("\n");
}

int* calculerNiveauxTrophiquesEntiersSansPrecision(Graphe* graphe, float* moyenne) {
    int* niveaux = (int*)calloc(graphe->sommet, sizeof(int));
    int* tempNiveaux = (int*)calloc(graphe->sommet, sizeof(int));
    if (niveaux == NULL || tempNiveaux == NULL) {
        fprintf(stderr, "Erreur d'allocation mémoire.\n");
        free(niveaux);
        free(tempNiveaux);
        return NULL;
    }

    // Initialisation des niveaux à 1 pour tous les sommets
    for (int i = 0; i < graphe->sommet; i++) {
        niveaux[i] = 1;
    }

    bool changement = true;
    while (changement) {
        changement = false;

        // Copie des niveaux actuels
        for (int i = 0; i < graphe->sommet; i++) {
            tempNiveaux[i] = niveaux[i];
        }

        for (int i = 0; i < graphe->sommet; i++) {
            int sommeNiveaux = 0, sommePoids = 0;
            for (int j = 0; j < graphe->sommet; j++) {
                if (graphe->adjMatrix[j][i] != INT_MAX) {
                    sommeNiveaux += tempNiveaux[j] * graphe->adjMatrix[j][i];
                    sommePoids += graphe->adjMatrix[j][i];
                }
            }
            if (sommePoids > 0) {
                int nouveauNiveau = 1 + (sommeNiveaux / sommePoids); // Calcul basé sur les prédécesseurs
                if (nouveauNiveau != niveaux[i]) {
                    niveaux[i] = nouveauNiveau;
                    changement = true;
                }
            }
        }
    }

    // Calcul de la moyenne des niveaux trophiques
    int somme = 0;
    for (int i = 0; i < graphe->sommet; i++) {
        somme += niveaux[i];
    }
    *moyenne = (float)somme / graphe->sommet;

    free(tempNiveaux);
    return niveaux;
}

void afficherNiveauxTrophiquesEntiersSansPrecision(Graphe* graphe, int* niveaux) {
    printf("Niveaux trophiques des sommets :\n");
    for (int i = 0; i < graphe->sommet; i++) {
        printf("Sommet %d : %d\n", i + 1, niveaux[i]);
    }
}



void calculerCentralites(Graphe* graphe) {
    printf("Degres pour chaque sommet :\n");
    for (int i = 1; i < graphe->sommet; i++) {
        int degreEntrant = 0, degreSortant = 0;
        for (int j = 0; j < graphe->sommet; j++) {
            if (graphe->adjMatrix[j][i] != INT_MAX) degreEntrant++;
            if (graphe->adjMatrix[i][j] != INT_MAX) degreSortant++;
        }
        int degreTotal = degreEntrant + degreSortant;
        printf("Sommet %d : Entree = %d, Sortie = %d, Total = %d\n",
               i, degreEntrant, degreSortant, degreTotal);
    }
}


void simulerDisparition(Graphe* graphe, int sommet) {
    if (sommet < 1 || sommet > graphe->sommet) {
        fprintf(stderr, "Erreur : Sommet invalide (%d).\n", sommet);
        return;
    }

    int indexSommet = sommet - 1; // Conversion pour un index base 0

    // Parcourir toute la matrice d'adjacence
    for (int i = 0; i < graphe->sommet; i++) {
        for (int j = 0; j < graphe->sommet; j++) {
            // Supprimer toutes les aretes où le sommet est implique
            if (i == indexSommet || j == indexSommet) {
                graphe->adjMatrix[i][j] = INT_MAX; // Supprime l'arete
            }
        }
    }

    // Marquer le sommet comme supprime (facultatif, utile pour d'autres fonctions)
    if (graphe->supprime != NULL) {
        graphe->supprime[indexSommet] = true;
    }

    printf("Le sommet %d et toutes ses aretes ont ete supprimes.\n", sommet);
}

void dfs(Graphe* graphe, int sommetDepart, int* predecesseurs, int* distances, int* chemin, int* cheminIndex) {
    graphe->visite[sommetDepart] = true;
    chemin[(*cheminIndex)++] = sommetDepart;

    for (int i = 0; i < graphe->sommet; i++) {
        if (graphe->adjMatrix[sommetDepart][i] != INT_MAX && !graphe->supprime[i] && !graphe->visite[i]) {
            int nouveauPoids = distances[sommetDepart] + graphe->adjMatrix[sommetDepart][i];
            if (nouveauPoids < distances[i]) {
                distances[i] = nouveauPoids;
                predecesseurs[i] = sommetDepart;
            }
            dfs(graphe, i, predecesseurs, distances, chemin, cheminIndex);
        }
    }
}
