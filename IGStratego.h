#include <SDL/SDL.h>
#include <SDL/SDL_image.h>



void deplacerPion(int grille[10][10], int xdepart, int ydepart, int xarrivee, int yarrivee);
void initBoard(SGameState gameState, SDL_Surface *ecran, SDL_Surface *pionRouge, SDL_Surface *pionBleu );
int interfaceGraphique(SGameState gameState);