#include <stdlib.h>
#include <stdio.h>
#include "PolyLib.h"
#include "IGStratego.h"
#define TAILLE_CASE 38	//Taille d'une case en pixels

void initBoard(SGameState gameState, SDL_Surface *ecran, SDL_Surface *pionRouge, SDL_Surface *pionBleu ){
	
	SDL_Rect position;
	int i=0, j=0;
	for(i = 0; i<10; i++){
		for(j=0; j<10; j++){
			switch(gameState.board[i][j].content){
				case ECred:
					position.x = j*(ecran->w/10);
					position.y = i*(ecran->h/10);
					SDL_BlitSurface(pionRouge, NULL, ecran, &position);
					break;

				case ECblue:
					position.x = j*(ecran->w/10);
					position.y = i*(ecran->h/10);
					SDL_BlitSurface(pionBleu, NULL, ecran, &position);
					break;
				
				default:
					break;
			}
		}
	}
}

SMove renvoieCoordonnees(){
	
	SDL_Event event;
	SPos positionDepart;
	SPos positionArrivee;
	SMove mouvementPion;
	int continuer = 1;

	while(continuer){
		SDL_WaitEvent(&event);
		switch(event.type){
				case SDL_MOUSEBUTTONUP:
					if(event.button.button == SDL_BUTTON_LEFT){
						positionDepart.line = (event.button.y)/TAILLE_CASE;
						positionDepart.col = (event.button.x)/TAILLE_CASE;
						continuer = 0;
					}
					break;

				default:
					break;
		}
	}
	continuer = 1;

	while(continuer){
		SDL_WaitEvent(&event);
		switch(event.type){
				case SDL_MOUSEBUTTONUP:
					if(event.button.button == SDL_BUTTON_LEFT){
						positionArrivee.line = (event.button.y)/TAILLE_CASE;
						positionArrivee.col = (event.button.x)/TAILLE_CASE;
						continuer = 0;					
					}
					break;

				default:
					break;
		}
	}
	mouvementPion.start = positionDepart;
	mouvementPion.end = positionArrivee;
	
	return mouvementPion;
}

int interfaceGraphique(SGameState gameState){
	
	
	
	SDL_Surface *ecran = NULL, *imageFond = NULL, *pionRouge = NULL, *pionBleu = NULL;	//création des variables
	SDL_Rect positionFond;
	int continuer = 1;
	SDL_Event event;
	
	SDL_Init(SDL_INIT_VIDEO);	//initialisation de la SDL

	ecran = SDL_SetVideoMode(380, 377, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);	//chargement des surfaces
	SDL_WM_SetCaption("Stratego", NULL);
	
	imageFond = IMG_Load("board.png");
	pionRouge = IMG_Load("pionrouge.png");
	pionBleu = IMG_Load("pionbleu.png");
	
	if(pionRouge == NULL)
	
	SDL_SetColorKey(pionRouge, SDL_SRCCOLORKEY, SDL_MapRGB(pionRouge->format, 255, 255, 255));
	SDL_SetColorKey(pionBleu, SDL_SRCCOLORKEY, SDL_MapRGB(pionBleu->format, 255, 255, 255));
	
	positionFond.x = 0;
	positionFond.y = 0;
	
	SDL_BlitSurface(imageFond, NULL, ecran, &positionFond);
	
	initBoard(gameState, ecran, pionRouge, pionBleu);	

	SDL_Flip(ecran);
	/*
	while (continuer){
		SDL_WaitEvent(&event);
		switch(event.type){
			case SDL_QUIT:
				continuer = 0;
				break;
			case SDL_KEYDOWN:
				switch(event.key.keysym.sym){
					case SDLK_ESCAPE:
						continuer = 0;
						break;
					
					default:
						break;
				}
				break;
		}
		
	}*/
	/*
	SDL_FreeSurface(pionRouge);
	SDL_FreeSurface(pionBleu);
	SDL_FreeSurface(imageFond);
	SDL_Quit(); //on quitte la SDL
*/
	return EXIT_SUCCESS;
	
}
