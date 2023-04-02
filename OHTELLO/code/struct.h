#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <math.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <stdlib.h>
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

//init.c
extern void initialiser_plateau(Board *board);
extern void Init_texture();
Player* creer_joueur(char* nom, PlayerColor couleur) ;
extern void init_All();

//menu.c
extern void afficher_menu() ;

//game.c
extern int start();


//logique.c
int verif_place_horiz_d(int x, int y, Board* board,SDL_Texture* texture,int changerTexture);
int verif_place_horiz_g(int x, int y, Board* board,SDL_Texture* texture, int changerTexture);
int verif_place_verti_h(int x, int y, Board* board,SDL_Texture* texture, int changerTexture);
int verif_place_verti_b(int x, int y, Board* board,SDL_Texture* texture, int changerTexture);
int verif_place_diag_bg(int x, int y, Board* board,SDL_Texture* texture, int changerTexture);
int verif_place_diag_bd(int x, int y, Board* board,SDL_Texture* texture, int changerTexture);
int verif_place_diag_hg(int x, int y, Board* board,SDL_Texture* texture, int changerTexture);
int verif_place_diag_hd(int x, int y, Board* board,SDL_Texture* texture, int changerTexture);
int coup_jouable_ou_non(Board* board, SDL_Texture* texture);
int Detection_Fin(Board* board, SDL_Texture* texture);
char placer_pion(int x, int y, SDL_Texture* texture, Board* board);


//affichage.c
extern void afficher_plateau(Board* board) ;
extern void affiche_tour(SDL_Renderer* renderer);
extern void afficher_popup(SDL_Renderer* renderer, const char* message);
extern void wait_two_seconds() ;
void afficher_texture_coup_jouable(int x, int y, Board* board,SDL_Texture* texture);
void afficher_coup_jouable(Board* board, SDL_Texture* texture);
void changer_texture_case(int x, int y, Board* board,SDL_Texture* texture);


//declaration pour ne pas a les placer en parametre

SDL_Window* window;
SDL_Renderer *renderer;
// Charger les textures pour les pions noirs et blancs
SDL_Texture *black_texture;
SDL_Texture *white_texture ;
SDL_Texture *grey_texture ;
SDL_Texture *carre_grille_texture ;
// Charger les textures la grille et le contour de jeu
SDL_Texture* fond_board;
SDL_Texture* grille_texture ;
SDL_Surface* contour_surface ;
SDL_Texture* contour_texture ;
Player* joueur_noir ;
Player* joueur_blanc;
Player* current_player;
TTF_Font* font; // police d ecriture
