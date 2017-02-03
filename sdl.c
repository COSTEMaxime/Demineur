#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include "constantes.h"

//fonction qui affiche la fenetre et initialise le pointeur screen
void init (SDL_Surface** screen, TTF_Font** police)
{

    if (SDL_Init(SDL_INIT_VIDEO) == -1) //test pour savoir si SDl a bien démarée (c'est qu'elle est capricieuse cette bibliothèque...)
    {
        fprintf(stderr, "Erreur d'initialisation de la SDL : %s\n", SDL_GetError());   //si fail on affiche le message d'erreur
        exit(1);    //et on quitte le programme avec le code d'erreur 1
    }

    *screen = SDL_SetVideoMode(LARGEUR_FENETRE, HAUTEUR_FENETRE, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);

    if (*screen == NULL)
    {
        fprintf(stderr, "Erreur lors du chargement de la fenêtre : %s\n", SDL_GetError());
        exit(1);
    }

        //bibliothèque pour le texte
    if (TTF_Init() == -1)
    {
        fprintf(stderr, "Erreur d'initialisation de TTF_Init : %s\n", TTF_GetError());
        exit(1);
    }


    *police = TTF_OpenFont("polices/arial.ttf", 22);

    if (*police == NULL)
    {
        fprintf(stderr, "Erreur lors du chargement de la police : %s", TTF_GetError());
        exit(1);

    }

    SDL_WM_SetCaption("Demineur", NULL);
    SDL_WM_SetIcon(IMG_Load("images/icone.png"), NULL);

}

void afficher (SDL_Surface** screen, TTF_Font** police, int time, int nbBombes, int **grille, int **mask, int *continuer)
{

    //effacement de l'écran
    SDL_FillRect(*screen, NULL, SDL_MapRGB((*screen)->format, 255,255,255));
    //affichage de la barre en bas
    SDL_Surface* barre = NULL;
    SDL_Rect posBarre;
    posBarre.x = 0;
    posBarre.y = HAUTEUR_FENETRE - HAUTEUR_BARRE;
    barre = IMG_Load("images/barre.png");
    SDL_BlitSurface(barre, NULL, *screen, &posBarre);


    //surfaces qui vont servir à l'affichage du texte
    SDL_Surface* texteTemps = NULL;
    SDL_Rect posTexteTemps;
    posTexteTemps.x = 10;
    posTexteTemps.y = posBarre.y + 10;

    SDL_Surface* texteBombes = NULL;
    SDL_Rect posTexteBombes;
    posTexteBombes.x = LARGEUR_FENETRE / 2 + 70;
    posTexteBombes.y = posBarre.y + 10;

    SDL_Color couleurNoire = {0,0,0};   //couleur du texte

    //affichage du temps
    char temps[20] = "";
    sprintf(temps, "Temps : %d", time);
    texteTemps = TTF_RenderText_Blended(*police, temps, couleurNoire);
    SDL_BlitSurface(texteTemps, NULL, *screen, &posTexteTemps);

    //affichages des bombes
    char bombes[20] = "";
    sprintf(bombes, "Flags restants : %d", nbBombes);
    texteBombes = TTF_RenderText_Blended(*police, bombes, couleurNoire);
    SDL_BlitSurface(texteBombes, NULL, *screen, &posTexteBombes);

    //creation des différentes textures
    SDL_Surface *unknown = NULL,
                *flag = NULL,
                *mine = NULL;

    SDL_Surface *nbMines[9] = {NULL};

    unknown = IMG_Load("images/unknown.png");
    flag = IMG_Load("images/flag.png");
    mine = IMG_Load("images/mine.png");

    char nbMines_[20] = "";

    for (int i = 0; i < 9; i++)
    {
        sprintf(nbMines_, "images/%d.png", i);
        nbMines[i] = IMG_Load(nbMines_);
    }

    SDL_Rect pos;

    //affichage de la grille
    for (int i = 0; i < NB_BLOCS_LARGEUR; i++)
    {
        for (int j = 0; j < NB_BLOCS_HAUTEUR; j++)
        {

            pos.x = i * TAILLE_BLOC;
            pos.y = j * TAILLE_BLOC;
            //si on doit afficher la case
            if (mask[i][j] == 1)
            {
                //test si la case est une bombe
                if (grille[i][j] == -1)
                {
                    SDL_BlitSurface(mine, NULL, *screen, &pos);
                    *continuer = 0;
                }
                else
                {
                    SDL_BlitSurface(nbMines[grille[i][j]], NULL, *screen, &pos);
                }

            } else if (mask[i][j] == 2) //test si l'on doit afficher un drapeau
                SDL_BlitSurface(flag, NULL, *screen, &pos);
              else  //sinon on affiche l'image d'une case non découverte
                SDL_BlitSurface(unknown, NULL, *screen, &pos);
        }
    }

    SDL_Flip(*screen);

    //libération de la mémoire
    for (int i = 0; i < 9; i++)
    {
        SDL_FreeSurface(nbMines[i]);
    }
    SDL_FreeSurface(flag);
    SDL_FreeSurface(unknown);
    SDL_FreeSurface(mine);
    SDL_FreeSurface(texteTemps);
    SDL_FreeSurface(texteBombes);
    SDL_FreeSurface(barre);
}


void fin (SDL_Surface** screen, TTF_Font** police, int** grille, int result)
{

    SDL_Surface *message = NULL;

    SDL_Rect posMessage;

    posMessage.x = LARGEUR_FENETRE / 2 - 30;
    posMessage.y = HAUTEUR_FENETRE - HAUTEUR_BARRE + 10;

    SDL_Color couleurNoire = {0,0,0};

    char texte[30] = "";

    //affichage du message de fin + des mines si perdu
    if (result)
    {
        sprintf(texte, "Gagné !, cliquez pour rejouer");

    } else  {
       sprintf(texte, "Perdu !, cliquez pour rejouer");
       afficherMines(screen, grille);
    }

    message = TTF_RenderText_Blended(*police, texte, couleurNoire);
    SDL_BlitSurface(message, NULL, *screen, &posMessage);

    SDL_Flip(*screen);
    SDL_FreeSurface(message);
}

void afficherMines (SDL_Surface** screen, int** grille)
{
    //affichage de toutes les mines
    SDL_Surface* mine;
    mine = IMG_Load("images/mine.png");
    SDL_Rect pos;

    for (int i = 0; i < NB_BLOCS_LARGEUR; i++)
    {
        for (int j = 0; j < NB_BLOCS_HAUTEUR; j++)
        {
            if (grille[i][j] == -1)
            {
                pos.x = i * TAILLE_BLOC;
                pos.y = j * TAILLE_BLOC;
                SDL_BlitSurface(mine, NULL, *screen, &pos);
            }
        }
    }

    SDL_Flip(*screen);
    SDL_FreeSurface(mine);
}
