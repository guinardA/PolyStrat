#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

SMove renvoieCoordonnees();
void initBoard(SGameState gameState, SDL_Surface *ecran, SDL_Surface *pionRouge, SDL_Surface *pionBleu );
int interfaceGraphique(SGameState gameState);
