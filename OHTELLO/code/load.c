#include "struct.h"
#include "stdbool.h"

void save_board(Board *board,const char  *filename)
{
    FILE *fp = NULL;
    printf("enregistrement du plateau");
    fp = fopen(filename, "w");
    if (fp == NULL)
    {
        printf("Erreur : impossible d'ouvrir le fichier ");
        return;
    }
   // printf("enregistrement");

    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            if (board->cells[i][j].player != 0)
            {
                fprintf(fp, "%d %d %d\n", i, j, board->cells[i][j].player);
            }
        }
    }

    fprintf(fp, "%d\n", current_player->couleur);

    fclose(fp);
}

int charger_pions(Board *board,const char  *filename)
{
    FILE *fp = NULL;
    printf("\n chargement en cours \n");
    fp = fopen(filename, "r");
    if (fp == NULL)
    {
        printf("\nErreur : impossible d'ouvrir le fichier\n");
        return 3;
    }

    char buffer[128];
    int x, y, player, couleur;

    // Lecture des lignes du fichier
    while (fgets(buffer, 128, fp) != NULL)
    {
        if (sscanf(buffer, "%d %d %d", &x, &y, &player) != 3)
        {
            continue;
        }
        if (feof(fp))
        {
            break;
        }

        printf(" %d %d %d\n", x, y, player);
        placer_pion_chargement_partie(board, x, y, player);
    }

    // Lecture de la dernière valeur
    fseek(fp, -2, SEEK_END);   // Positionnement du curseur à l'avant-dernier caractère
    fgets(buffer, 2, fp);      // Lecture du dernier caractère
    couleur = buffer[0] - '0'; // Conversion du caractère en entier
    // printf("%d \n",couleur);

    fclose(fp);
    return couleur;
}
void efface_fichier(const char  *filename)
{
    FILE *fp = fopen(filename, "w");

    fclose(fp);
}

int is_file_empty(const char  *filename) {
    FILE* fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Erreur : impossible d'ouvrir le fichier\n");
        return 1;
    }

    int is_empty = 1;
    char c = fgetc(fp);
    if (c != EOF) {
        is_empty = 0;
    }

    fclose(fp);
    return is_empty;
}
int charger_partie(Board *board, int chargement,const char *filename)
{
	// Initialiser le plateau de jeu et l'afficher
	
	initialiser_plateau(board);

	if (chargement == 0) // int chargement correspond a si l'on charge ou non la partie, 0 = non
	{
        printf("chargement = 0");
		afficher_plateau(board, 1);
		current_player = joueur_noir;
		save_board(board,filename);
	}

	else if (chargement == 1)
	{       
        
		int couleur_joueur;
		afficher_plateau(board, 0);
		couleur_joueur = charger_pions(board,filename);
		if (couleur_joueur == 1)
		{
			current_player = joueur_noir;
		}
		else if (couleur_joueur == 2)
		{
			current_player = joueur_blanc;
		}
		else
		{
			printf("\n ERROR \n");
			return 1;
		}
	}
	return 0;
}