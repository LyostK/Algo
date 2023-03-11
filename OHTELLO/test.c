#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <math.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#define BOARD_SIZE 8
#define CELL_SIZE 64
#define PIECE_SIZE 50 //taille de pion en pixel

typedef enum {
    EMPTY,
    BLACK,
    WHITE
} PlayerColor;

typedef struct {
    PlayerColor player;
    SDL_Rect rect;
} Cell;

typedef struct {
    Cell cells[BOARD_SIZE][BOARD_SIZE];
    int grid_x;
    int grid_y;
    int cell_size;
} Board;
typedef struct {
    char* nom;
    PlayerColor couleur;
    int score;
} Player;

void initialiser_plateau(Board *board) {
    // Calculer les dimensions de la grille
    int screen_height=800;
    int screen_width=800;
    int grid_size = screen_height * 0.6;
    int margin_x = (screen_width - grid_size) / 2;
    int margin_y = (screen_height - grid_size) / 2;
    int cell_size = grid_size / BOARD_SIZE;
    int grid_x = margin_x;
    int grid_y = margin_y;

    board->grid_x = grid_x;
    board->grid_y = grid_y;
    board->cell_size = cell_size;
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            board->cells[i][j].player = EMPTY;
            board->cells[i][j].rect.x = i * CELL_SIZE;
            board->cells[i][j].rect.y = j * CELL_SIZE;
            board->cells[i][j].rect.w = CELL_SIZE;
            board->cells[i][j].rect.h = CELL_SIZE;
        }
    }

    int center = BOARD_SIZE / 2;
    board->cells[center - 1][center - 1].player = WHITE;
    board->cells[center][center].player = WHITE;
    board->cells[center - 1][center].player = BLACK;
    board->cells[center][center - 1].player = BLACK;
}


Player* creer_joueur(char* nom, PlayerColor couleur) {
    Player* joueur = (Player*)malloc(sizeof(Player));
    if (joueur == NULL) {
        fprintf(stderr, "Erreur : impossible d'allouer la mémoire pour le joueur.\n");
        exit(EXIT_FAILURE);
    }
    joueur->nom = nom;
    joueur->couleur = couleur;
    joueur->score = 0;
    return joueur;
}



void afficher_plateau(SDL_Renderer* renderer, Board* board, SDL_Texture* black_texture, SDL_Texture* white_texture, SDL_Texture* grille_texture,SDL_Texture* contour) {
    SDL_RenderClear(renderer);

    // Définir la taille de la fenêtre
    int SCREEN_WIDTH = 800;
    int SCREEN_HEIGHT = 800;

    // Calculer les dimensions de la grille
    int grid_size = SCREEN_HEIGHT * 0.6;//taille de la grille comme 70% de la hauteur de la fenetre
    int margin_x = (SCREEN_WIDTH - grid_size) / 2; //pour la marge sur les cotés
    int margin_y = (SCREEN_HEIGHT - grid_size) / 2;//pour la marge haut et bas
    int cell_size = grid_size / BOARD_SIZE ; // taille de chaque case
    int grid_x = margin_x; //pour placer les pions, on aurait pu utiliser margin_x direct mais c est une question de lisibilité
    int grid_y = margin_y;
    // Calculer la taille de la bordure en fonction de la taille de la grille
    int border_size = (int)(grid_size * 0.05); // Par exemple, la bordure peut faire 5% de la taille de la grille
    int border_x = margin_x - border_size;
    int border_y = margin_y - border_size;
    int border_width = grid_size + border_size * 2;
    int border_height = grid_size + border_size * 2;

    // Dessiner le contour du plateau
    SDL_Rect border_rect = {border_x, border_y, border_width, border_height};

   
   
    // Dessiner le fond vert
    SDL_SetRenderDrawColor(renderer, 50, 128, 65, 255);
    SDL_RenderClear(renderer);

    // Dessiner l'image de fond pour la grille et pour le contour de la grille
    SDL_Rect grille_rect = {grid_x, grid_y, grid_size, grid_size};
    SDL_RenderCopy(renderer, contour, NULL, &border_rect);
    SDL_RenderCopy(renderer, grille_texture, NULL, &grille_rect);


    //Dessiner le cadrillage
    SDL_SetRenderDrawColor(renderer, 150, 0, 0, 255);
    for (int i = 1; i < BOARD_SIZE; i++) {
        SDL_RenderDrawLine(renderer, grid_x + i * cell_size, grid_y, grid_x + i * cell_size, grid_y + grid_size);
        SDL_RenderDrawLine(renderer, grid_x, grid_y + i * cell_size, grid_x + grid_size, grid_y + i * cell_size);
    }

    // Dessiner les pions
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            PlayerColor player = board->cells[i][j].player;
            if (player == BLACK) {
                SDL_Rect rect = board->cells[i][j].rect;
                rect.x = grid_x + i * cell_size+3;
                rect.y = grid_y + j * cell_size+3;
                rect.w = cell_size-5;
                rect.h = cell_size-5;
                SDL_RenderCopy(renderer, black_texture, NULL, &rect);
            } else if (player == WHITE) {
                SDL_Rect rect = board->cells[i][j].rect;
                rect.x = grid_x + i * cell_size+3;
                rect.y = grid_y + j * cell_size+3;
                rect.w = cell_size-5;
                rect.h = cell_size-5;
                SDL_RenderCopy(renderer, white_texture, NULL, &rect);
            }
        }
    }

    SDL_RenderPresent(renderer);
}
void cancel_affiche_tour(){
    
}
void affiche_tour(SDL_Renderer* renderer,Player* current_player) {
    // Charger la police d'écriture
        TTF_Init();
    TTF_Font* font = TTF_OpenFont("ASMAN.TTF", 24); // Modifier le chemin vers la police de votre choix
    if (!font) {
        printf("Erreur de chargement de la police : %s\n", TTF_GetError());
        return;
    }

    // Créer le message
    const char* message;
    message="au noir de commencer";
    if (current_player->couleur == WHITE) {
        message = "Tour du joueur blanc";
    } else {
        message = "Tour du joueur noir";
    }

    // Créer une surface avec le message
    SDL_Color color = { 255, 255, 255 }; // Couleur du texte (blanc)
    SDL_Surface* surface = TTF_RenderText_Solid(font, message, color);
    if (!surface) {
        printf("Erreur de création de la surface : %s\n", SDL_GetError());
        TTF_CloseFont(font);
        return;
    }

    // Créer une texture à partir de la surface
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        printf("Erreur de création de la texture : %s\n", SDL_GetError());
        SDL_FreeSurface(surface);
        TTF_CloseFont(font);
        return;
    }

    // Définir la zone d'affichage du texte
    SDL_Rect text_rect;
    text_rect.x = 20; // Position x du texte (20 pixels de marge)
    text_rect.y = 20; // Position y du texte (20 pixels de marge)
    text_rect.w = surface->w +20; // Largeur du texte
    text_rect.h = surface->h; // Hauteur du texte

    // Effacer la zone de texte précédente
    SDL_SetRenderDrawColor(renderer, 50, 128, 65, 255); // Couleur de fond de la zone de texte (noir)
    SDL_RenderFillRect(renderer, &text_rect);
 text_rect.w = surface->w;
    // Dessiner le texte
    SDL_RenderCopy(renderer, texture, NULL, &text_rect);

    // Rafraîchir l'affichage
    SDL_RenderPresent(renderer);

    // Libérer les ressources
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
    TTF_CloseFont(font);
}
void afficher_popup(SDL_Renderer* renderer, const char* message) {
    // Charger la police d'écriture
    TTF_Init();
    TTF_Font* font = TTF_OpenFont("ASMAN.TTF", 24); // Modifier le chemin vers la police de votre choix
    if (!font) {
        printf("Erreur de chargement de la police : %s\n", TTF_GetError());
        return;
    }
    
    // Créer une surface avec le message
    SDL_Color color = { 255, 255, 255 }; // Couleur du texte (blanc)
    SDL_Surface* surface = TTF_RenderText_Solid(font, message, color);
    if (!surface) {
        printf("Erreur de création de la surface : %s\n", SDL_GetError());
        TTF_CloseFont(font);
        return;
    }
    
    // Créer une texture à partir de la surface
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        printf("Erreur de création de la texture : %s\n", SDL_GetError());
        SDL_FreeSurface(surface);
        TTF_CloseFont(font);
        return;
    }
    
    // Créer un rectangle pour la boîte de dialogue
    SDL_Rect rect;
    rect.x = (800 - surface->w - 20) / 2; // Position x de la boîte de dialogue (centrée horizontalement)
    rect.y = 50; // Position y de la boîte de dialogue (en haut de l'écran)
    rect.w = surface->w + 20; // Largeur de la boîte de dialogue (surface + 20 pixels de padding)
    rect.h = surface->h + 20; // Hauteur de la boîte de dialogue (surface + 20 pixels de padding)
    
    // Dessiner la boîte de dialogue
    SDL_SetRenderDrawColor(renderer, 50, 128, 65, 255); // Couleur de fond de la boîte de dialogue (noir)
    SDL_RenderFillRect(renderer, &rect);
    
    // Dessiner le texte
    SDL_Rect text_rect;
    text_rect.x = rect.x + 10; // Position x du texte (10 pixels de padding)
    text_rect.y = rect.y + 10; // Position y du texte (10 pixels de padding)
    text_rect.w = surface->w;
    text_rect.h = surface->h;
    SDL_RenderCopy(renderer, texture, NULL, &text_rect);
    
    // Rafraîchir l'affichage
    SDL_RenderPresent(renderer);
    
    // Attendre 2 secondes
    SDL_Delay(1000);
    
    // Dessiner un rectangle noir pour effacer le texte
    SDL_SetRenderDrawColor(renderer, 50, 128, 65, 205);
    SDL_RenderFillRect(renderer, &rect);
    SDL_RenderPresent(renderer);
    
    // Libérer les ressources
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
    TTF_CloseFont(font);
}

void placer_pion(int x, int y, SDL_Renderer* renderer, SDL_Texture* texture, Board* board) {
    //permet d'avoir la cellule du tableau ou l'on clique
    int cell_x = (x - board->grid_x) / board->cell_size;
    int cell_y = (y - board->grid_y) / board->cell_size;
    printf("\ncell_x:%d et celle_y:%d\n",cell_x,cell_y);

    if (cell_x < 0  ||cell_x >= BOARD_SIZE || cell_y < 0 || cell_y >= BOARD_SIZE) {
        // Click en dehors de la grille
        return;
    }

    Cell* cell = &board->cells[cell_x][cell_y];

    if (cell->player != EMPTY) {
        // Case déjà occupée
        afficher_popup(renderer,"vous ne pouvez pas jouer le coup ");
        return;
    }

    // Mettre à jour le plateau
    cell->player = BLACK;

    // Afficher le pion
 
    SDL_Rect dest_rect = {
        board->grid_x + cell_x * board->cell_size+3,
        board->grid_y + cell_y * board->cell_size+3,
        board->cell_size-5,
        board->cell_size-5
    };
    SDL_RenderCopy(renderer, texture, NULL, &dest_rect);
    SDL_RenderPresent(renderer);
}

void init_SDL(){

    
}
void Init_texture(SDL_Texture *black_texture,SDL_Texture *white_texture,SDL_Surface* grille_surface,SDL_Surface* contour_surface,SDL_Window* window,SDL_Renderer *renderer){
if (!black_texture) {
    printf("Erreur de chargement de la texture pour les pions noirs: %s\n", IMG_GetError());
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

}
if (!white_texture) {
    printf("Erreur de chargement de la texture pour les pions blancs: %s\n", IMG_GetError());
    SDL_DestroyTexture(black_texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

}
if (grille_surface == NULL) {
    printf("Erreur de chargement de l'image de fond pour la grille : %s\n", IMG_GetError());

}
if (contour_surface == NULL) {
    printf("Erreur de chargement de l'image de fond pour la grille : %s\n", IMG_GetError());

}
if (!renderer) {
    printf("Erreur de création du rendu SDL: %s\n", SDL_GetError());
    SDL_DestroyWindow(window);
    SDL_Quit();

}
if (!window) {
    printf("Erreur de création de la fenêtre: %s\n", SDL_GetError());
    SDL_Quit();

}
printf("coucou");
}

int main(int argc, char *argv[]) {
    
 // Créer la fenêtre et le rendu SDL
Player* joueur_noir = creer_joueur("Noir", BLACK);
Player* joueur_blanc = creer_joueur("Blanc", WHITE);
Player* current_player = joueur_blanc; // Le joueur courant est le joueur 1
//creation de la fenetre
SDL_Window* window = SDL_CreateWindow("Othello", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, BOARD_SIZE * CELL_SIZE+300, BOARD_SIZE * CELL_SIZE+300,0);
//creation du renderer, ce qu on va mettre a jour pour afficher des choses
SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
// Charger les textures pour les pions noirs et blancs
SDL_Texture *black_texture = IMG_LoadTexture(renderer, "pion_noir.png");
SDL_Texture *white_texture = IMG_LoadTexture(renderer, "pion_blanc.png");
// Charger les textures la grille et le contour de jeu
SDL_Surface* grille_surface = IMG_Load("Board.png");
SDL_Texture* grille_texture = SDL_CreateTextureFromSurface(renderer, grille_surface);
SDL_Surface* contour_surface = IMG_Load("contour_board.png");
SDL_Texture* contour_texture = SDL_CreateTextureFromSurface(renderer, contour_surface);
// verifier que il y a pas eu d erreur
Init_texture(black_texture,white_texture,grille_surface,contour_surface,window,renderer);
// Initialiser le plateau de jeu et l'afficher
Board board;
initialiser_plateau(&board);
afficher_plateau(renderer, &board, black_texture, white_texture,grille_texture,contour_texture);

// Attendre que l'utilisateur ferme la fenêtre
SDL_Event event;
int quit = 0;
while (!quit) {
        affiche_tour(renderer,current_player);

    while (SDL_PollEvent(&event)) {

        //printf("la case est de type : %d", board.cells[3][3].player);
        if(event.type == SDL_QUIT){
                quit=1;
            }

    switch (event.type) {
        case SDL_QUIT:
            quit=1;
            break;
        
        case SDL_MOUSEBUTTONUP:
            if (event.button.button == SDL_BUTTON_LEFT) {
                int x = event.button.x;
                int y = event.button.y;
                printf("%d =x et %d = y\n,",x,y);
                //verification_mouvement();
                if(current_player==joueur_blanc){
                    printf("joueur blanc");
                placer_pion(x, y,renderer, white_texture , &board); // la texture definit la couleur du pion qu'on va placer
                current_player = joueur_noir;
                }
                else{
                   placer_pion(x, y,renderer, black_texture , &board); // la texture definit la couleur du pion qu'on va placer 
                   current_player = joueur_blanc;
                }
            }
            break;
            }
    }
    SDL_RenderPresent(renderer);
}
printf("coucoufefezefzgred");

// Libérer les ressources allouées
SDL_DestroyTexture(black_texture);
SDL_DestroyTexture(white_texture);
SDL_DestroyRenderer(renderer);
SDL_DestroyWindow(window);
IMG_Quit();
SDL_Quit();
return 0;
}
