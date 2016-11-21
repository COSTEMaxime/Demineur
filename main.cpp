#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <time.h>

#include "constantes.h"
#include "fonctions.h"
#include "sdl.h"
#include "jeu.h"

int main ( int argc, char** argv )
{

    //variable qui va stocker les informations concernant la fenêtre
    SDL_Surface* screen = NULL;

    //variable qui va contenir les informations concernant la police
    TTF_Font* police = NULL;

    //fonction qui initialise les diffrentes bibliothèques/variables qui servent à l'affichage...
    init(&screen, &police);

    //fonction principale du jeu
    jeu(&screen, &police);

    //fermeture de la SDL
    TTF_CloseFont(police);
    TTF_Quit();
    SDL_Quit();

    return 0;
}
