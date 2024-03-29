#include "struct.h"

extern int screen_height;
extern int screen_width; // a changer dans init et affichage et logique.c si on change
extern int grid_size;    // 800*0.6
extern int margin_x;
extern int margin_y;
extern int cell_size;
extern int grid_x; //=margin x
extern int grid_y; //=margin y

// fonction qui bloque toutes les entrées utilisateurs pendant 2 secondes
void wait_two_seconds()
{
    SDL_Event event;
    Uint32 start_time = SDL_GetTicks();
    Uint32 current_time = start_time;
    SDL_PumpEvents();
    SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT);
    while (current_time - start_time < 2000)
    {
        SDL_PumpEvents();
        while (SDL_PollEvent(&event))
        {
            /* Bloquer les entrées utilisateur */
        }
        SDL_Delay(10);
        current_time = SDL_GetTicks();
    }
    SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT);
    SDL_PumpEvents();
}

void afficher_plateau(Board *board, int choix)
{ // le choix correspond a si l'on charge une partie ou non, pour savoir si on affiche les 4 pions au centre ou non
    SDL_RenderClear(renderer);
    SDL_Texture *menu = IMG_LoadTexture(renderer, "image/home.png");
    SDL_Texture *previous = IMG_LoadTexture(renderer, "image/back.png");
    SDL_Texture *information = IMG_LoadTexture(renderer, "image/info.png");

    // Calculer la taille de la bordure en fonction de la taille de la grille
    int border_size = (int)(grid_size * 0.07); // Par exemple, la bordure peut faire 5% de la taille de la grille
    int border_x = margin_x - border_size;
    int border_y = margin_y - border_size;
    int border_width = grid_size + border_size * 2;
    int border_height = grid_size + border_size * 2;

    // Dessiner le contour du plateau
    SDL_Rect border_rect = {border_x, border_y, border_width, border_height};
    // rect pour le fond
    int largeur_fenetre, hauteur_fenetre;
    SDL_GetWindowSize(window, &largeur_fenetre, &hauteur_fenetre);
    SDL_Rect rect_fenetre = {0, 0, largeur_fenetre, hauteur_fenetre};

    // dessine le quadrillage
    SDL_Rect grille_rect = {grid_x, grid_y, grid_size, grid_size};

    SDL_Rect rect_menu = {300, 40, 200, 75};     // bouton menu
    SDL_Rect rect_previous = {650, 350, 150, 75}; // bouton precedent
    SDL_Rect rect_info = {60, 60, 100, 50};     // bouton information
    SDL_RenderClear(renderer);
    // dessine le fond
    SDL_RenderCopy(renderer, fond_board, NULL, &rect_fenetre);
    // Dessiner l'image de fond pour la grille et pour le contour de la grille
    SDL_RenderCopy(renderer, contour_texture, NULL, &border_rect); // dessine le contour du plateau
    SDL_RenderCopy(renderer, grille_texture, NULL, &grille_rect);  // dessine le plateau
    SDL_RenderCopy(renderer, previous, NULL, &rect_previous);      // dessine le bouton "precedent"
    SDL_RenderCopy(renderer, menu, NULL, &rect_menu);              // dessine le bouton menu
    SDL_RenderCopy(renderer, information, NULL, &rect_info);       // dessine le bouton information

    // Dessiner le cadrillage
    int line_width = 5; // Définir la largeur de la ligne
    SDL_Rect rect;

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    for (int i = 1; i < BOARD_SIZE; i++)
    {
        rect.x = grid_x + i * cell_size - line_width / 2;
        rect.y = grid_y;
        rect.w = line_width;
        rect.h = grid_size;
        SDL_RenderFillRect(renderer, &rect);

        rect.x = grid_x;
        rect.y = grid_y + i * cell_size - line_width / 2;
        rect.w = grid_size;
        rect.h = line_width;
        SDL_RenderFillRect(renderer, &rect);
    }

    SDL_RenderDrawRect(renderer, &grille_rect);
    // Dessiner les pions
    if (choix == 1)
    {
        for (int i = 0; i < BOARD_SIZE; i++)
        {
            for (int j = 0; j < BOARD_SIZE; j++)
            {
                PlayerColor player = board->cells[i][j].player;
                if (player == BLACK)
                {

                    SDL_Rect rect = board->cells[i][j].rect;
                    rect.x = grid_x + i * cell_size + 3;
                    rect.y = grid_y + j * cell_size + 3;
                    rect.w = cell_size - 5;
                    rect.h = cell_size - 5;
                    SDL_RenderCopy(renderer, black_texture, NULL, &rect);
                }
                else if (player == WHITE)
                {

                    SDL_Rect rect = board->cells[i][j].rect;
                    rect.x = grid_x + i * cell_size + 3;
                    rect.y = grid_y + j * cell_size + 3;
                    rect.w = cell_size - 5;
                    rect.h = cell_size - 5;
                    SDL_RenderCopy(renderer, white_texture, NULL, &rect);
                }
            }
        }
    }

    SDL_RenderPresent(renderer);
}

void affiche_tour(SDL_Renderer *renderer)
{
    // Charger la police d'écriture

    SDL_Rect rect_tour = {250, 700, 300, 85};
    // Créer le message

    if (current_player->couleur == WHITE)
    {
        SDL_RenderCopy(renderer, tour_blanc, NULL, &rect_tour);
    }
    else
    {
        SDL_RenderCopy(renderer, tour_noir, NULL, &rect_tour);
    }

    // Dessiner le texte

    // Rafraîchir l'affichage
    SDL_RenderPresent(renderer);

    // Libérer les ressources


}
void afficher_popup(SDL_Renderer *renderer, const char *message)
{
    // Charger la police d'écriture
    SDL_Texture *error = IMG_LoadTexture(renderer, "image/error.png");

    SDL_Rect rect_error = {250, 700, 300, 85};     // bouton information
    
    // dessine le fond
    SDL_RenderCopy(renderer, error, NULL, &rect_error);

    // Rafraîchir l'affichage
    SDL_RenderPresent(renderer);
    
    // Attendre 2 secondes
    wait_two_seconds();
    affiche_tour(renderer);
    





SDL_DestroyTexture(error);
 
}
// permet de changer la texture de n mporte quelle case
void changer_texture_case(int x, int y, Board *board, SDL_Texture *texture)
{
    // Vérifier que les indices sont valides
    //	printf("\n changement de la texture \n");
    if (x < 0 || x >= BOARD_SIZE || y < 0 || y >= BOARD_SIZE)
    {
        printf("Indices de case invalides\n");
        return;
    }
    // on met a jour la cellule
    Cell *cell = &board->cells[x][y];
    if (current_player == joueur_blanc)
    {
        cell->player = WHITE;
    }
    else
    {
        cell->player = BLACK;
    }

    // Afficher la nouvelle texture
    SDL_Rect dest_rect = {
        board->grid_x + x * board->cell_size + 3,
        board->grid_y + y * board->cell_size + 3,
        board->cell_size - 5,
        board->cell_size - 5};
    SDL_RenderCopy(renderer, texture, NULL, &dest_rect);
    SDL_RenderPresent(renderer);
}
void afficher_texture_coup_jouable(int x, int y, Board *board, SDL_Texture *texture)
{
    // Vérifier que les indices sont valides
    //	printf("\n changement de la texture \n");
    if (x < 0 || x >= BOARD_SIZE || y < 0 || y >= BOARD_SIZE)
    {
        printf("Indices de case invalides\n");
        return;
    }
    // Afficher la nouvelle texture
    SDL_Rect dest_rect = {
        board->grid_x + x * board->cell_size + 3,
        board->grid_y + y * board->cell_size + 3,
        board->cell_size - 5,
        board->cell_size - 5};
    SDL_RenderCopy(renderer, texture, NULL, &dest_rect);
    SDL_RenderPresent(renderer);
}
void afficher_coup_jouable(Board *board, SDL_Texture *texture)
{
    int x, y;

    for (x = 0; x < BOARD_SIZE; x++)
    {
        for (y = 0; y < BOARD_SIZE; y++)
        {
            if (verif_place_horiz_d(x, y, board, texture, 0) == 1 && board->cells[x][y].player == EMPTY)
            {

                afficher_texture_coup_jouable(x, y, board, texture);
            }
            if (verif_place_horiz_g(x, y, board, texture, 0) == 1 && board->cells[x][y].player == EMPTY)
            {

                afficher_texture_coup_jouable(x, y, board, texture);
            }
            /**/
            if (verif_place_verti_h(x, y, board, texture, 0) == 1 && board->cells[x][y].player == EMPTY)
            {

                afficher_texture_coup_jouable(x, y, board, texture);
            }
            if (verif_place_verti_b(x, y, board, texture, 0) == 1 && board->cells[x][y].player == EMPTY)
            {

                afficher_texture_coup_jouable(x, y, board, texture);
            }
            if (verif_place_diag_bg(x, y, board, texture, 0) == 1 && board->cells[x][y].player == EMPTY)
            {

                afficher_texture_coup_jouable(x, y, board, texture);
            }
            if (verif_place_diag_bd(x, y, board, texture, 0) == 1 && board->cells[x][y].player == EMPTY)
            {
                afficher_texture_coup_jouable(x, y, board, texture);
            }
            if (verif_place_diag_hg(x, y, board, texture, 0) == 1 && board->cells[x][y].player == EMPTY)
            {
                afficher_texture_coup_jouable(x, y, board, texture);
            }
            if (verif_place_diag_hd(x, y, board, texture, 0) == 1 && board->cells[x][y].player == EMPTY)
            {
                afficher_texture_coup_jouable(x, y, board, texture);
            }
        }
    }
}
void afficher_image_fin(SDL_Renderer *renderer, int vainqueur) // afficher l image pour le vainqueur
{
    // Charger l'image
    SDL_Texture *texture = NULL;
    SDL_Texture *quit = NULL;
    quit = IMG_LoadTexture(renderer, "image/quit.png");
    // Créer la texture à partir de la surface
    if (vainqueur == 1)
    {
        texture = IMG_LoadTexture(renderer, "image/fin_noir.png");
    }
    else
    {
        texture = IMG_LoadTexture(renderer, "image/fin_blanc.png");
    }
    if (!texture)
    {
        printf("Erreur de création de la texture : %s\n", SDL_GetError());
        return;
    }
    // Récupérer les dimensions de la texture
    int tex_w, tex_h;
    SDL_QueryTexture(texture, NULL, NULL, &tex_w, &tex_h);
     int bt_w, bt_h;
    SDL_QueryTexture(texture, NULL, NULL, &bt_w, &bt_h);
    // Définir la zone d'affichage de l'image centrée
    SDL_Rect dest_rect;
    dest_rect.x = 150;
    dest_rect.y = 100 ;
    dest_rect.w = 600;
    dest_rect.h = 500;

      
    // Définir la zone d'affichage du bouton
    SDL_Rect dest_rect2;
    dest_rect2.x = 275;
    dest_rect2.y = 550;
    dest_rect2.w = 350;
    dest_rect2.h = 100;
    // Dessiner l'image centrée
    SDL_RenderCopy(renderer, texture, NULL, &dest_rect);
    SDL_RenderCopy(renderer, quit, NULL, &dest_rect2);
    // Rafraîchir l'affichage
    SDL_RenderPresent(renderer);

    // Libérer les ressources
    SDL_DestroyTexture(texture);
    SDL_DestroyTexture(quit);
}


void placer_pion_chargement_partie(Board *board, int cell_x, int cell_y, int joueur)
{
    SDL_Texture *texture = NULL;
    if (joueur == 1)
    { // joueur noir
        texture = black_texture;
        board->cells[cell_x][cell_y].player = BLACK;
    }
    else if (joueur == 2)
    {
        texture = white_texture;
        board->cells[cell_x][cell_y].player = WHITE;
    }

    // Afficher le pion
    SDL_Rect dest_rect = {
        board->grid_x + cell_x * board->cell_size + 3,
        board->grid_y + cell_y * board->cell_size + 3,
        board->cell_size - 5,
        board->cell_size - 5};
    SDL_RenderCopy(renderer, texture, NULL, &dest_rect);
    SDL_RenderPresent(renderer);

    // printf("\n test fin placer_pion()\n");
}



void showInformation () {
    const int SCREEN_WIDTH = 580;
const int SCREEN_HEIGHT = 480;
    SDL_Window* imageWindow = SDL_CreateWindow("Information", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer* imageRenderer = SDL_CreateRenderer(imageWindow, -1, SDL_RENDERER_ACCELERATED);

    SDL_Surface* imageSurface = IMG_Load("image/regle.png");
    SDL_Texture* imageTexture = SDL_CreateTextureFromSurface(imageRenderer, imageSurface);
    SDL_FreeSurface(imageSurface);

    SDL_Rect imageRect;
    imageRect.x = 0;
    imageRect.y = 0;
    SDL_QueryTexture(imageTexture, NULL, NULL, &imageRect.w, &imageRect.h);

    SDL_Event e;
    bool quit = false;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_CLOSE) {
                quit = true;
                break;
            } else if (e.type == SDL_MOUSEWHEEL) {
                if (e.wheel.y > 0) {
                    imageRect.y += 10;
                } else if (e.wheel.y < 0) {
                    imageRect.y -= 10;
                }
         
            }
        }

        SDL_RenderClear(imageRenderer);
        SDL_RenderCopy(imageRenderer, imageTexture, NULL, &imageRect);
        SDL_RenderPresent(imageRenderer);
    }

    SDL_DestroyTexture(imageTexture);
    SDL_DestroyRenderer(imageRenderer);
    SDL_DestroyWindow(imageWindow);
}