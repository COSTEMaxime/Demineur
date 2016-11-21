#ifndef SDL_H_INCLUDED
#define SDL_H_INCLUDED

void init (SDL_Surface** screen, TTF_Font** police);
void afficher (SDL_Surface** screen, TTF_Font** police, int time, int nbBombes, int **grille, int **mask, int *continuer);
void fin(SDL_Surface** screen, TTF_Font** police, int** grille,  int result);
void afficherMines(SDL_Surface** screen, int** grille);

#include "sdl.c"

#endif // SDL_H_INCLUDED
