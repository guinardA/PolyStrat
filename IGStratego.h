#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>

int afficherMenu();
SMove renvoieCoordonnees();
void initBoard(SGameState gameState, SDL_Surface *ecran);
int interfaceGraphique(SGameState gameState);
void afficheMessage(char* message);
int afficheMessageDemande(char* message);
void selectionnerPion(SDL_Surface *ecran, SPos selected, SGameState gameState);
SPos getPos();
void quitter_sdl();
void afficherMessageEcran(char *message, int delay);
void afficherPiecesRestantes(SGameState gameState,SDL_Surface *ecran);
void initInterface();
