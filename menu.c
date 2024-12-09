//
// Created by inesd on 08/12/2024.
//
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h> // Pour sleep()

#include "menu.h"
#include "graphe1.h"
#include "dijkstra.h"
// Pause la console pendant un certain nombre de secondes
void pauseConsole(int seconds) {
    sleep(seconds);
}

// Efface l'écran de la console
void clearConsole() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// Affiche le menu principal
void afficherMenu() {
    clearConsole();
    printf("=== Menu Principal ===\n");
    printf("1. Charger un graphe depuis un fichier\n");
    printf("2.Generer le graphe\n");
    printf("3. Chaine alimentaire (Dijkstra)\n");
    printf("4. Executer DFS\n");
    printf("5. Verifier la connexite du graphe\n");
    printf("6. Rechercher des sommets particuliers\n");
    printf("7. Calculer les niveaux trophiques\n");
    printf("8. Calculer les centralites\n");
    printf("9. Simuler la disparition d'une espece\n");
    printf("10. Simuler la dynamique des populations\n");
    printf("11. Quitter\n");
    printf("Votre choix : ");
}

// Gère les choix du menu
int gererChoix(int choix, Sommet** sommets, int* nb_sommets, int*** adjMatrix) {
    int sommetDepart, sommetArrivee, sommet;
    switch (choix) {
        case 1: { // Charger un graphe depuis un fichier
            char filename[100];
            printf("Entrez le nom du fichier contenant les sommets et arcs : ");
            scanf("%s", filename);

            FILE* fichier = fopen(filename, "r");
            if (!fichier) {
                fprintf(stderr, "Erreur : Impossible d'ouvrir le fichier '%s'.\n", filename);
                break;
            }

            // Lire les sommets
            *sommets = lireSommets(fichier, nb_sommets);
            fclose(fichier);

            // Créer la matrice d'adjacence
            *adjMatrix = creerMatriceAdjacence(*nb_sommets);

            // Lire les arcs
            fichier = fopen(filename, "r");
            if (!fichier) {
                fprintf(stderr, "Erreur : Impossible d'ouvrir le fichier '%s'.\n", filename);
                break;
            }
            int nb_arcs = 0;
            GrapheArc* arcs = lireArcs(fichier, &nb_arcs);
            fclose(fichier);

            // Ajouter les arcs dans la matrice d'adjacence
            for (int i = 0; i < nb_arcs; i++) {
                ajouterArete(*adjMatrix, *sommets, *nb_sommets, arcs[i].predateur, arcs[i].proie, arcs[i].interaction);
            }
            free(arcs);

            printf("Graphe charge avec succes.\n");
            pauseConsole(2);
            break;
        }/*
            case 2: { // Afficher un graphe DOT
                char dotFile[100];
                printf("Entrez le nom du fichier DOT : ");
                scanf("%s", dotFile);

                afficherGraphvizDot(dotFile);
                pauseConsole(2);
                break;
            }
            case 2 : { // Ouvrir le fichier DOT
                char dotFile[100];
                printf("Entrez le nom du fichier DOT : ");
                scanf("%s", dotFile);

                ouvrirFichierDot(dotFile);
                pauseConsole(2);
                break;
            }*/
        case 3: { // Dijkstra
            printf("Entrez le sommet de depart : ");
            scanf("%d", &sommetDepart);
            sommetDepart--;
            printf("Entrez le sommet d'arrivee : ");
            scanf("%d", &sommetArrivee);
            sommetArrivee--;

            Dijkstra* resultat = dijkstra(*sommets, *nb_sommets, *adjMatrix, sommetDepart);
            if (resultat) {
                afficherResultatsDijkstra(resultat, *sommets, *nb_sommets, sommetArrivee);
                libererResultatsDijkstra(resultat);
            }
            pauseConsole(10);
            break;
        }
        case 4: { // DFS
            printf("Entrez le sommet de depart : ");
            scanf("%d", &sommetDepart);
            sommetDepart--;

            bool* visite = (bool*)calloc(*nb_sommets, sizeof(bool));
            int* chemin = (int*)malloc(*nb_sommets * sizeof(int));
            int cheminIndex = 0;

            int* predecesseurs = (int*)malloc(*nb_sommets * sizeof(int));
            int* distances = (int*)malloc(*nb_sommets * sizeof(int));

            dfs(*sommets, *nb_sommets, *adjMatrix, sommetDepart, visite, predecesseurs, distances, chemin, &cheminIndex);

            printf("Chemin DFS : ");
            for (int i = 0; i < cheminIndex; i++) {
                printf("%s%s", (*sommets)[chemin[i]].nom, (i == cheminIndex - 1) ? "\n" : " -> ");
            }

            free(visite);
            free(chemin);
            free(predecesseurs);
            free(distances);
            pauseConsole(10);
            break;
        }
        case 5: { // Vérification de la connexité
            if (*adjMatrix == NULL) {
                printf("Le graphe n'est pas charge.\n");
            } else {
                if (verifierConnexite(*adjMatrix, *nb_sommets)) {
                    printf("Le graphe est connexe.\n");
                } else {
                    printf("Le graphe n'est pas connexe.\n");
                }
            }
            pauseConsole(5);
            break;
        }
        case 6: { // Rechercher des sommets particuliers
            rechercherSommetsParticuliers(*sommets, *nb_sommets, *adjMatrix);
            pauseConsole(10);
            break;
        }
        case 7: { // Calculer les niveaux trophiques
            float moyenne = 0.0;
            int* niveaux = calculerNiveauxTrophiquesEntiersSansPrecision(*sommets, *nb_sommets, *adjMatrix, &moyenne);
            if (niveaux) {
                afficherNiveauxTrophiquesEntiersSansPrecision(*sommets, *nb_sommets, niveaux);
                printf("Niveau trophique moyen : %.2f\n", moyenne);
                free(niveaux);
            }
            pauseConsole(10);
            break;
        }
        case 8: { // Calculer les centralités
            calculerCentralites(*sommets, *nb_sommets, *adjMatrix);
            pauseConsole(10);
            break;
        }
        case 9: { // Simuler la disparition d'une espèce
            printf("Entrez le sommet a supprimer : ");
            scanf("%d", &sommet);
            simulerDisparition(*sommets, *nb_sommets, *adjMatrix, sommet);
            char filename[100];
            printf("Entrez le nom du fichier pour sauvegarder le graphe modifie : ");
            scanf("%s", filename);
            sauvegarderGrapheDansFichier(*sommets, *nb_sommets, *adjMatrix, filename);
            pauseConsole(2);
            break;
        }
        case 10:{ // Simulation avec saisie utilisateur
            simulerDynamiquePopulation(*sommets, *nb_sommets, *adjMatrix);
            pauseConsole(10);
            break;
        }
        case 11: {
            printf("Au revoir !\n");
            return 1; // Quitter le programme
        }
        default: {
            printf("Choix invalide. Veuillez réessayer.\n");
            pauseConsole(2);
            break;
        }
    }
    return 0; // Continuer le programme
}
