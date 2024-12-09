//
// Created by inesd on 08/12/2024.
//

#ifndef INGE2_PROJET_MENU_H
#define INGE2_PROJET_MENU_H
#include "graphe1.h"
#include "dijkstra.h"

void pauseConsole(int seconds);
void clearConsole();
void afficherMenu();
int gererChoix(int choix, Sommet** sommets, int* nb_sommets, int*** adjMatrix);

#endif //INGE2_PROJET_MENU_H
