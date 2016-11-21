#ifndef FONCTIONS_H_INCLUDED
#define FONCTIONS_H_INCLUDED

/*
    grille : -1 = bombe
              0 = pas de bombe � proximit�
              1 = une bombe � proximit�
              2 = deux bombes � proximit�
              ...


    mask : 0 = ne pas afficher cette case
           1 = afficher cette case
           2 = flag
*/

int** genererGrille (int xPos, int yPos);
int** genererMask ();
void discover (int** grille, int** mask, int xPos, int yPos, int *continuer);
void discover_ (int** grille, int** mask, int** checkCase, int xPos, int yPos);
void testBombe (int** mask, int nbBombes, int xPos, int yPos);
int test (int x, int y);
int placeFlag (int** mask, int xPos, int yPos);
int testWin (int** grille, int** mask);

#include "time.h"
#include "fonctions.c"


#endif // FONCTIONS_H_INCLUDED
