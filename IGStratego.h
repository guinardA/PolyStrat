#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>

SMove renvoieCoordonnees();
void initBoard(SGameState gameState, SDL_Surface *ecran, SDL_Surface *pionsRouges[12], SDL_Surface *pionsBleus[12]);
int interfaceGraphique(SGameState gameState);
void afficheMessage(char* message);
void selectionnerPion(SDL_Surface *ecran, SPos selected);
