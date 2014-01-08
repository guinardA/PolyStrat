#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
enum{ RED, BLUE, NONE };

void deplacerPion(int grille[10][10], int xdepart, int ydepart, int xarrivee, int yarrivee){
	printf("Valeur coordonnées départ: %d/%d\n", xdepart, ydepart);
	printf("Valeur coordonnées arrivée: %d/%d\n", xarrivee, yarrivee);
	grille[xarrivee][yarrivee] = grille[xdepart][ydepart];
	grille[xdepart][ydepart] = NONE;

}

int main(int argc, char *argv[]){

	SDL_Surface *ecran = NULL, *imageFond = NULL, *pionRouge = NULL, *pionBleu = NULL;	//création des variables
	SDL_Rect board, positionFond, position, positionClick, positionClickTMP;
	int continuer = 1, i = 0, j = 0;
	SDL_Event event;
	int grille[10][10] = {0};
	int select = 0;
	
	
	SDL_Init(SDL_INIT_VIDEO);	//initialisation de la SDL

	ecran = SDL_SetVideoMode(380, 377, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);	//chargement des surfaces
	SDL_WM_SetCaption("Stratego", NULL);
	
	imageFond = IMG_Load("board.png");
	pionRouge = IMG_Load("pionrouge.png");
	pionBleu = IMG_Load("pionbleu.png");

	SDL_SetColorKey(pionRouge, SDL_SRCCOLORKEY, SDL_MapRGB(pionRouge->format, 255, 255, 255));
	SDL_SetColorKey(pionBleu, SDL_SRCCOLORKEY, SDL_MapRGB(pionBleu->format, 255, 255, 255));

	positionFond.x = 0;
	positionFond.y = 0;
	
	SDL_BlitSurface(imageFond, NULL, ecran, &positionFond);

	for(i = 0; i<4; i++){
		for(j=0; j<10; j++){
			grille[i][j] = RED;
		}
	}
	i=4; j=0;
	for(i = 4; i<6; i++){
		for(j=0; j<10; j++){
			grille[i][j] = NONE;
		}
	}
	
	i=6; j=0;
	for(i = 6; i<10; i++){
		for(j=0; j<10; j++){
			grille[i][j] = BLUE;
		}
	}
	

	i=0; j=0;
	for(i = 0; i<10; i++){
		for(j=0; j<10; j++){
			switch(grille[i][j]){
				case RED:
					position.x = j*(ecran->w/10);
					position.y = i*(ecran->h/10);
					SDL_BlitSurface(pionRouge, NULL, ecran, &position);
					break;

				case BLUE:
					position.x = j*(ecran->w/10);
					position.y = i*(ecran->h/10);
					SDL_BlitSurface(pionBleu, NULL, ecran, &position);
					break;
			}
		}
	}	

	SDL_Flip(ecran);
	
	positionClick.x = 0;
	positionClick.y = 0;

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

			case SDL_MOUSEBUTTONUP:
				if(event.button.button == SDL_BUTTON_LEFT){
					positionClick.x = event.button.x;
					positionClick.y = event.button.y;
					printf("X:%d----Y:%d\n",(positionClick.x)/38,(positionClick.y)/38);
					printf("Valeur de la case: %d\n", grille[positionClick.x][positionClick.y]);

//continuer = 0;
					if(grille[(positionClick.x)/38][(positionClick.x)/38] != NONE){ //Si y a un pion la ou on clique
						select = 1;
						positionClickTMP.x = event.button.x;
						positionClickTMP.y = event.button.y;
					}
					if((grille[(positionClick.x)/38][(positionClick.x)/38] == NONE) && select == 1){//Si la case est vide et que l'on a déjà sélectionné une case
						
						deplacerPion(grille, (positionClickTMP.x)/38, (positionClickTMP.y)/38, (event.button.x)/38, (event.button.y)/38);
						select = 0;
					}
					printf("Select: %d\n", select);
				}
				break;
		}
		SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 255, 255, 255)); //on efface l'écran pour ne pas laisser de traces
		SDL_BlitSurface(imageFond, NULL, ecran, &positionFond);
		

		//
		for(i = 0; i<10; i++){
			for(j=0; j<10; j++){
				switch(grille[i][j]){
					case RED:
						position.x = j*(ecran->h/10);
						position.y = i*(ecran->w/10);
						SDL_BlitSurface(pionRouge, NULL, ecran, &position);
						break;

					case BLUE:
						position.x = j*(ecran->h/10);
						position.y = i*(ecran->w/10);
						SDL_BlitSurface(pionBleu, NULL, ecran, &position);
						break;
				}
			}
		}
		SDL_Flip(ecran);
		
	}
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
	SDL_FreeSurface(pionRouge);
	SDL_FreeSurface(pionBleu);
	SDL_FreeSurface(imageFond);
	SDL_Quit(); //on quitte la SDL

	return EXIT_SUCCESS;


}
