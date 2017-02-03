void jeu (SDL_Surface** screen, TTF_Font** police)
{
    int debutJeu, continuer, clic, result;
    debutJeu = 1;
    continuer = 1;
    clic = 1,
    result = 0;
    //compteur du nombre de drapeaux restants � placer
    int flagCounter = NB_MINES;

    SDL_Event event;
    //tableau qui va d�finir sui le contenu d'une case est affich� ou pas et si cette comporte un drapeau
    int **mask = genererMask();

    while (debutJeu)
    {

        SDL_WaitEvent(&event);
        afficher(screen, police, 0, flagCounter, NULL, mask, &continuer);


        switch (event.type)
        {
        case SDL_QUIT:
            debutJeu = 0;
            continuer = 0;
            clic = 0;
            break;

        case SDL_MOUSEBUTTONDOWN:
            if (event.button.button == SDL_BUTTON_LEFT && event.motion.y < HAUTEUR_FENETRE - HAUTEUR_BARRE)
            {
                debutJeu = 0;
            }
            break;
        }
    }

    //g�n�rer la grille
    int tempsBase = SDL_GetTicks();
    int **grille = genererGrille((int)event.motion.x / TAILLE_BLOC, (int)event.motion.y / TAILLE_BLOC);
    //on d�couvre la grille & partir de l'endroit o� l'on a cliqu�
    discover(grille, mask, (int)event.motion.x / TAILLE_BLOC, (int)event.motion.y / TAILLE_BLOC, &continuer);

    while (continuer)
    {
        //gestion des �v�nements
        SDL_WaitEvent(&event);
        switch (event.type)
        {
        //si on quitte la fen�tre
        case SDL_QUIT:
            continuer = 0;
            break;

        //si clic souris
        case SDL_MOUSEBUTTONDOWN:

            if (event.motion.y < HAUTEUR_FENETRE - HAUTEUR_BARRE)
            {
                int posX = (int)event.motion.x / TAILLE_BLOC;
                int posY = (int)event.motion.y / TAILLE_BLOC;

                if (event.button.button == SDL_BUTTON_LEFT)
                {
                    discover(grille, mask, posX, posY, &continuer);
                } else if (event.button.button == SDL_BUTTON_RIGHT)
                {
                    flagCounter+= placeFlag(mask, posX, posY);
                }
            }
            break;
        }

        //test si gagner
        if (flagCounter == 0)
            if (testWin(grille, mask))
            {
                continuer = 0;
                result = 1;
            }

        //affichage
        afficher(screen, police, (SDL_GetTicks() - tempsBase) / 1000, flagCounter, grille, mask, &continuer);
    }

    //message de fin
    fin(screen, police, grille, result);

    //attente d'un clic dans la fen�tre
    while (clic)
    {
        //si clic on sort de la boucle
        SDL_WaitEvent(&event);
        if (event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_QUIT)
            clic = 0;
    }
}
