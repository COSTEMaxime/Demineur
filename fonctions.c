int** genererGrille (int xPos, int yPos)
{

    //création du tableau
    int **grille = NULL;

    //allocation dynamique du tableau
    grille = (int**) malloc(NB_BLOCS_LARGEUR * sizeof(int*));

    if (grille == NULL)
        exit(-1);

    for (int i = 0; i < NB_BLOCS_LARGEUR; i++)
    {
        grille[i] = (int*) malloc(NB_BLOCS_HAUTEUR * sizeof(int));
        if (grille[i] == NULL)
            exit(-1);
    }

    //initialisation du tableau
    for (int i = 0; i < NB_BLOCS_LARGEUR; i++)
    {
        for (int j = 0; j < NB_BLOCS_HAUTEUR; j++)
        {
            grille[i][j] = 0;
        }
    }

    //initilisation des mines
    int nbMines = 0;
    srand(time(NULL));

    while (nbMines != NB_MINES)
    {
        int x = rand()%NB_BLOCS_LARGEUR;
        int y = rand()%NB_BLOCS_HAUTEUR;

        if (grille[x][y] != -1 && (abs(x-xPos) > 1 && abs(y-yPos) > 1))
        {
            //placement de la mine
            grille[x][y] = -1;
            nbMines++;
        }
    }

    //comptage des mines
    for (int i = 0; i < NB_BLOCS_LARGEUR; i++)
    {
        for (int j = 0; j < NB_BLOCS_HAUTEUR; j++)
        {
            if (grille[i][j] != -1)
            {
                for (int tempI = -1; tempI < 2; tempI++)
                {
                    for (int tempJ = -1; tempJ < 2; tempJ++)
                    {
                        if (test(i+tempI, j+tempJ))
                        {
                            if (grille[i + tempI][j + tempJ] == -1)
                                grille[i][j]++;
                        }
                    }
                }
            }
        }
    }
    return grille;
}

//fonction qui génère le tableau qui va définir si une case est affichée ou non
int** genererMask ()
{
    int **mask = NULL;

    //allocation dynamique du tableau
    mask = (int**) malloc(NB_BLOCS_LARGEUR * sizeof(int*));

    if (mask == NULL)
        exit(-1);

    for (int i = 0; i < NB_BLOCS_LARGEUR; i++)
    {
        mask[i] = (int*) malloc(NB_BLOCS_HAUTEUR * sizeof(int));
        if (mask[i] == NULL)
            exit(-1);
    }

    //initialisation du tableau
    for (int i = 0; i < NB_BLOCS_LARGEUR; i++)
    {
        for (int j = 0; j < NB_BLOCS_HAUTEUR; j++)
        {
            mask[i][j] = 0;
        }
    }

    return mask;
}


void discover (int** grille, int** mask, int xPos, int yPos, int *continuer)
{
    //si l'on est sur un drapeau on ne fait rien
    if (mask[xPos][yPos] == 2)
        return;

    //on découvre la case
    mask[xPos][yPos] = 1;

    //si l'on est sur une mine
    if (grille[xPos][yPos] == -1)
    {
        *continuer = 0;
        return;
    }

    //si l'on est sur une case adjacente à une bombe
    if (grille[xPos][yPos] != 0)
    {
        testBombe(mask, grille[xPos][yPos], xPos, yPos);
        return;
    }

    int **checkCases = genererMask();
    discover_(grille, mask, checkCases, xPos, yPos);

}

void testBombe (int** mask, int nbBombes, int xPos, int yPos)
{
    int count = 0;

    for (int x = -1; x < 2; x++)
    {
        for (int y = -1; y < 2; y++)
        {
            if (test(xPos + x, yPos + y))
                if (mask[xPos + x][yPos + y] == 2)
                    count++;
        }
    }

    if (count == nbBombes)
    {
        for (int x = -1; x < 2; x++)
        {
            for (int y = -1; y < 2; y++)
            {
                if (test(xPos + x, yPos + y))
                    if (mask[xPos + x][yPos + y] != 2)
                        mask[xPos + x][yPos + y] = 1;
            }
        }
    }
}


void discover_ (int** grille, int** mask, int** checkCases, int xPos, int yPos)
{
    checkCases[xPos][yPos] = 1;

    for (int x = -1; x < 2; x++)
    {
        for (int y = -1; y < 2; y++)
        {
            if (test(xPos + x, yPos + y))
            {
                if (checkCases[xPos+x][yPos+y] == 0)
                {
                    mask[xPos+x][yPos+y] = 1;
                    checkCases[xPos+x][yPos+y] = 1;
                    if (grille[xPos+x][yPos+y] == 0)
                        discover_(grille, mask, checkCases, xPos+x, yPos+y);
                }
            }
        }
    }
}

//fonction qui teste si l'on ne sort pas du tableau
int test (int x, int y)
{
    if ((x >= 0 && x < NB_BLOCS_LARGEUR) && (y >= 0 && y < NB_BLOCS_HAUTEUR))
        return 1;
    return 0;
}

//fonction qui permet de placer un drapeau sur une des cases
int placeFlag (int** mask, int xPos, int yPos)
{
    if (mask[xPos][yPos] == 1)
        return 0;

    if (mask[xPos][yPos] == 0)
    {
        mask[xPos][yPos] = 2;
        return -1;
    }

    mask[xPos][yPos] = 0;
    return 1;
}

//fonction qui teste si le joueur a gagné
int testWin (int** grille, int** mask)
{
    for (int i = 0; i < NB_BLOCS_LARGEUR; i++)
    {
        for (int j = 0; j < NB_BLOCS_HAUTEUR; j++)
        {
            if (grille[i][j] == -1 && mask[i][j] != 2)
                return 0;
        }
    }

    return 1;
}
