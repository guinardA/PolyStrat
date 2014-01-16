#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>

SMove renvoieCoordonnees(SDL_Surface *ecran, SGameState gameState);
void initBoard(SGameState gameState, SDL_Surface *ecran);
int interfaceGraphique(SGameState gameState);
void afficheMessage(char* message);
void selectionnerPion(SDL_Surface *ecran, SPos selected);
void quitter_sdl();
