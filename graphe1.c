#include <string.h>
#include "graphe1.h"
#include <stdio.h>
#include <stdlib.h>

Sommet* lireSommets(FILE* fichier, int* nb_sommets) {
    char ligne[256];
    *nb_sommets = 0;

    // Première passe pour compter les sommets
    while (fgets(ligne, sizeof(ligne), fichier)) {
        if (strstr(ligne, "# Arcs")) break; // Arrêter à la section des arcs
        if (ligne[0] != '#' && strlen(ligne) > 1) (*nb_sommets)++;
    }

    rewind(fichier);
    Sommet* sommets = (Sommet*)malloc(*nb_sommets * sizeof(Sommet));
    if (!sommets) {
        fprintf(stderr, "Erreur : Allocation memoire pour les sommets.\n");
        return NULL;
    }

    int index = 0;
    while (fgets(ligne, sizeof(ligne), fichier)) {
        if (strstr(ligne, "# Arcs")) break;
        if (ligne[0] != '#' && strlen(ligne) > 1) {
            sscanf(ligne, "%49s %d %f", sommets[index].nom, &sommets[index].population, &sommets[index].taux_croissance);
            index++;
        }
    }
    return sommets;
}

GrapheArc* lireArcs(FILE* fichier, int* nb_arcs) {
    char ligne[256];
    *nb_arcs = 0;

    // Première passe pour compter les arcs
    bool sectionArcs = false;
    while (fgets(ligne, sizeof(ligne), fichier)) {
        if (strstr(ligne, "# Arcs")) { sectionArcs = true; continue; }
        if (sectionArcs && strlen(ligne) > 1) (*nb_arcs)++;
    }

    rewind(fichier);
    GrapheArc* arcs = (GrapheArc*)malloc(*nb_arcs * sizeof(GrapheArc));
    if (!arcs) {
        fprintf(stderr, "Erreur : Allocation mémoire pour les arcs.\n");
        return NULL;
    }

    int index = 0;
    sectionArcs = false;
    while (fgets(ligne, sizeof(ligne), fichier)) {
        if (strstr(ligne, "# Arcs")) { sectionArcs = true; continue; }
        if (sectionArcs && strlen(ligne) > 1) {
            sscanf(ligne, "%49s %49s %d", arcs[index].predateur, arcs[index].proie, &arcs[index].interaction);
            index++;
        }
    }
    return arcs;
}

void ajouterArete(int** adjMatrix, Sommet* sommets, int nb_sommets, const char* src, const char* dest, int poids) {
    int srcIndex = -1, destIndex = -1;
    for (int i = 0; i < nb_sommets; i++) {
        if (strcmp(sommets[i].nom, src) == 0) srcIndex = i;
        if (strcmp(sommets[i].nom, dest) == 0) destIndex = i;
    }
    if (srcIndex != -1 && destIndex != -1) {
        adjMatrix[srcIndex][destIndex] = poids; // Arc orienté
    } else {
        fprintf(stderr, "Erreur : Sommets invalides (%s -> %s).\n", src, dest);
    }
}

// Créer et initialiser une matrice d'adjacence
int** creerMatriceAdjacence(int nb_sommets) {
    int** adjMatrix = (int**)malloc(nb_sommets * sizeof(int*));
    for (int i = 0; i < nb_sommets; i++) {
        adjMatrix[i] = (int*)malloc(nb_sommets * sizeof(int));
        for (int j = 0; j < nb_sommets; j++) {
            adjMatrix[i][j] = INT_MAX; // Initialiser avec une valeur élevée (aucune arête)
        }
    }
    return adjMatrix;
}

void sauvegarderGrapheDansFichier(Sommet* sommets, int nb_sommets, int** adjMatrix, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        fprintf(stderr, "Erreur : Impossible d'ouvrir le fichier '%s' en ecriture.\n", filename);
        return;
    }

    int nbAretes = 0;

    // Compter les arêtes existantes
    for (int i = 0; i < nb_sommets; i++) {
        for (int j = 0; j < nb_sommets; j++) {
            if (adjMatrix[i][j] != INT_MAX) {
                nbAretes++;
            }
        }
    }

    // Écrire les sommets
    fprintf(file, "# Sommets (nom, population, taux de croissance)\n");
    for (int i = 0; i < nb_sommets; i++) {
        fprintf(file, "%s %d %.2f\n", sommets[i].nom, sommets[i].population, sommets[i].taux_croissance);
    }

    // Écrire les arcs
    fprintf(file, "\n# Arcs (source, destination, coefficient d'interaction)\n");
    for (int i = 0; i < nb_sommets; i++) {
        for (int j = 0; j < nb_sommets; j++) {
            if (adjMatrix[i][j] != INT_MAX) {
                fprintf(file, "%s %s %d\n", sommets[i].nom, sommets[j].nom, adjMatrix[i][j]);
            }
        }
    }

    fclose(file);
    printf("Graphe sauvegarde dans le fichier '%s' avec %d aretes.\n", filename, nbAretes);
}

void rechercherSommetsParticuliers(Sommet* sommets, int nb_sommets, int** adjMatrix) {
    printf("\nRecherche de sommets particuliers :\n");

    // Derniers maillons (espèces sans prédateurs : aucun arc entrant)
    printf("Derniers maillons (especes sans predateurs) : ");
    for (int i = 0; i < nb_sommets; i++) {
        bool aucunEntrant = true;
        for (int j = 0; j < nb_sommets; j++) {
            if (adjMatrix[j][i] != INT_MAX) { // S'il y a un arc vers i
                aucunEntrant = false;
                break;
            }
        }
        if (aucunEntrant) {
            printf("%s ", sommets[i].nom);
        }
    }

    // Premiers maillons (producteurs primaires : aucun arc sortant)
    printf("\nPremiers maillons (producteurs primaires) : ");
    for (int i = 0; i < nb_sommets; i++) {
        bool aucunSortant = true;
        for (int j = 0; j < nb_sommets; j++) {
            if (adjMatrix[i][j] != INT_MAX) { // S'il y a un arc sortant de i
                aucunSortant = false;
                break;
            }
        }
        if (aucunSortant) {
            printf("%s ", sommets[i].nom);
        }
    }

    // Sommets ayant une seule source d'alimentation
    printf("\nEspeces ayant une seule source d'alimentation : ");
    for (int i = 0; i < nb_sommets; i++) {
        int compteurEntrants = 0;
        for (int j = 0; j < nb_sommets; j++) {
            if (adjMatrix[j][i] != INT_MAX) { // Compter les arcs entrants vers i
                compteurEntrants++;
            }
        }
        if (compteurEntrants == 1) {
            printf("%s ", sommets[i].nom);
        }
    }

    printf("\n");
}

int* calculerNiveauxTrophiquesEntiersSansPrecision(Sommet* sommets, int nb_sommets, int** adjMatrix, float* moyenne) {
    int* niveaux = (int*)calloc(nb_sommets, sizeof(int));
    int* tempNiveaux = (int*)calloc(nb_sommets, sizeof(int));
    if (niveaux == NULL || tempNiveaux == NULL) {
        fprintf(stderr, "Erreur d'allocation memoire.\n");
        free(niveaux);
        free(tempNiveaux);
        return NULL;
    }

    // Initialisation des niveaux à 1 pour tous les sommets
    for (int i = 0; i < nb_sommets; i++) {
        niveaux[i] = 1;
    }

    bool changement = true;
    while (changement) {
        changement = false;

        // Copie des niveaux actuels
        for (int i = 0; i < nb_sommets; i++) {
            tempNiveaux[i] = niveaux[i];
        }

        // Mise à jour des niveaux trophiques
        for (int i = 0; i < nb_sommets; i++) {
            int sommeNiveaux = 0, sommePoids = 0;
            for (int j = 0; j < nb_sommets; j++) {
                if (adjMatrix[j][i] != INT_MAX) { // Arc entrant
                    sommeNiveaux += tempNiveaux[j] * adjMatrix[j][i];
                    sommePoids += adjMatrix[j][i];
                }
            }
            if (sommePoids > 0) {
                int nouveauNiveau = 1 + (sommeNiveaux / sommePoids);
                if (nouveauNiveau != niveaux[i]) {
                    niveaux[i] = nouveauNiveau;
                    changement = true;
                }
            }
        }
    }

    // Calcul de la moyenne des niveaux trophiques
    int somme = 0;
    for (int i = 0; i < nb_sommets; i++) {
        somme += niveaux[i];
    }
    *moyenne = (float)somme / nb_sommets;

    free(tempNiveaux);
    return niveaux;
}
void afficherNiveauxTrophiquesEntiersSansPrecision(Sommet* sommets, int nb_sommets, int* niveaux) {
    printf("Niveaux trophiques des sommets :\n");
    for (int i = 0; i < nb_sommets; i++) {
        printf("%s : %d\n", sommets[i].nom, niveaux[i]);
    }
}

void calculerCentralites(Sommet* sommets, int nb_sommets, int** adjMatrix) {
    printf("Degres pour chaque sommet :\n");
    for (int i = 0; i < nb_sommets; i++) {
        int degreEntrant = 0, degreSortant = 0;

        // Calcul du degré entrant
        for (int j = 0; j < nb_sommets; j++) {
            if (adjMatrix[j][i] != INT_MAX) {
                degreEntrant++;
            }
        }

        // Calcul du degré sortant
        for (int j = 0; j < nb_sommets; j++) {
            if (adjMatrix[i][j] != INT_MAX) {
                degreSortant++;
            }
        }

        int degreTotal = degreEntrant + degreSortant;

        // Affichage des résultats avec le nom du sommet
        printf("%s : Entrée = %d, Sortie = %d, Total = %d\n",
               sommets[i].nom, degreEntrant, degreSortant, degreTotal);
    }
}

void simulerDisparition(Sommet* sommets, int nb_sommets, int** adjMatrix, int sommet) {
    if (sommet < 1 || sommet > nb_sommets) {
        fprintf(stderr, "Erreur : Sommet invalide (%d).\n", sommet);
        return;
    }

    int indexSommet = sommet - 1; // Conversion pour un index base 0

    // Supprimer toutes les arêtes impliquant le sommet
    for (int i = 0; i < nb_sommets; i++) {
        adjMatrix[indexSommet][i] = INT_MAX; // Supprimer les arcs sortants
        adjMatrix[i][indexSommet] = INT_MAX; // Supprimer les arcs entrants
    }

    printf("Le sommet '%s' et toutes ses aretes ont ete supprimes.\n", sommets[indexSommet].nom);
}

void dfs(Sommet* sommets, int nb_sommets, int** adjMatrix, int sommetDepart,bool* visite, int* predecesseurs, int* distances, int* chemin, int* cheminIndex) {
    visite[sommetDepart] = true;
    chemin[(*cheminIndex)++] = sommetDepart;

    for (int i = 0; i < nb_sommets; i++) {
        if (adjMatrix[sommetDepart][i] != INT_MAX && !visite[i]) { // Arc existant et sommet non visité
            int nouveauPoids = distances[sommetDepart] + adjMatrix[sommetDepart][i];
            if (nouveauPoids < distances[i]) {
                distances[i] = nouveauPoids;
                predecesseurs[i] = sommetDepart;
            }
            dfs(sommets, nb_sommets, adjMatrix, i, visite, predecesseurs, distances, chemin, cheminIndex);
        }
    }
}

void simulerDynamiquePopulation(Sommet* sommets, int nb_sommets, int** adjMatrix) {
    int nb_etapes, nb_sommets_simules;

    // Demander le nombre d'étapes
    printf("Entrez le nombre d'etapes de la simulation : ");
    scanf("%d", &nb_etapes);

    // Demander le nombre de sommets à simuler
    printf("Entrez le nombre de sommets a inclure dans la simulation : ");
    scanf("%d", &nb_sommets_simules);

    int indices_sommets_simules[nb_sommets_simules];
    float populations[nb_sommets_simules];

    // Saisie des sommets et des populations initiales
    for (int i = 0; i < nb_sommets_simules; i++) {
        printf("Entrez le numero du sommet %d (0-%d) : ", i + 1, nb_sommets - 1);
        scanf("%d", &indices_sommets_simules[i]);

        if (indices_sommets_simules[i] < 0 || indices_sommets_simules[i] >= nb_sommets) {
            printf("Erreur : Sommet invalide. Reessayez.\n");
            i--; // Re-demander ce sommet
            continue;
        }

        printf("Entrez la population initiale pour %s : ", sommets[indices_sommets_simules[i]].nom);
        scanf("%f", &populations[i]);
    }

    printf("\nSimulation de la dynamique des populations sur %d etapes :\n", nb_etapes);

    // Simulation
    for (int t = 0; t < nb_etapes; t++) {
        printf("\nEtape %d:\n", t + 1);
        float nouvelle_population[nb_sommets_simules];

        for (int k = 0; k < nb_sommets_simules; k++) {
            int i = indices_sommets_simules[k];
            float croissance = sommets[i].taux_croissance * populations[k] * (1.0 - (populations[k] / 1000.0)); // K = 1000
            float consommation = 0.0;

            // Consommation par les successeurs (prédateurs)
            for (int j = 0; j < nb_sommets; j++) {
                if (adjMatrix[i][j] != INT_MAX) {
                    consommation += (adjMatrix[i][j] / 100.0) * populations[k];
                }
            }

            nouvelle_population[k] = populations[k] + croissance - consommation;

            // Assurer que la population ne devient pas négative
            if (nouvelle_population[k] < 0) nouvelle_population[k] = 0;

            printf("%s: %.2f\n", sommets[i].nom, nouvelle_population[k]);
        }

        // Mise à jour des populations
        for (int k = 0; k < nb_sommets_simules; k++) {
            populations[k] = nouvelle_population[k];
        }
    }
}




void afficherSommets(Sommet* sommets, int nb_sommets) {
    printf("Sommets :\n");
    for (int i = 0; i < nb_sommets; i++) {
        printf("%s %d %.2f\n", sommets[i].nom, sommets[i].population, sommets[i].taux_croissance);
    }
}

void afficherArcs(GrapheArc* arcs, int nb_arcs) {
    printf("\nArcs :\n");
    for (int i = 0; i < nb_arcs; i++) {
        printf("%s -> %s (interaction : %d)\n", arcs[i].predateur, arcs[i].proie, arcs[i].interaction);
    }
}

void dfsConnexite(int** adjMatrix, int nb_sommets, bool* visite, int sommet) {
    visite[sommet] = true; // Marquer le sommet comme visité
    printf("Sommet visite : %d\n", sommet); // Affichage pour débogage

    // Parcourir les voisins
    for (int i = 0; i < nb_sommets; i++) {
        if (adjMatrix[sommet][i] != INT_MAX && !visite[i]) {
            dfsConnexite(adjMatrix, nb_sommets, visite, i); // Appel récursif
        }
    }
}

// Fonction principale pour vérifier la connexité du graphe
bool verifierConnexite(int** adjMatrix, int nb_sommets) {
    // **Initialisation** du tableau visite à false
    bool* visite = (bool*)calloc(nb_sommets, sizeof(bool));
    if (visite == NULL) { // Vérification d'allocation mémoire
        fprintf(stderr, "Erreur d'allocation memoire pour le tableau visite.\n");
        return false;
    }

    // Lancer le DFS à partir du sommet 0
    dfsConnexite(adjMatrix, nb_sommets, visite, 0);

    // Vérifier si tous les sommets ont été visités
    for (int i = 0; i < nb_sommets; i++) {
        if (!visite[i]) { // Si un sommet n'a pas été visité, le graphe n'est pas connexe
            free(visite); // Libérer la mémoire
            return false;
        }
    }

    free(visite); // Libérer la mémoire après utilisation
    return true;  // Le graphe est connexe
}
