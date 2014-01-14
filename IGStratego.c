#include "PolyLib.h"
#include "IGStratego.h"
#define TAILLE_CASE 38	//Taille d'une case en pixels

SDL_Surface *ecran = NULL, *imageFond = NULL, *pionRouge = NULL, *pionBleu = NULL;	//création des variables


void selectionnerPion(SDL_Surface *ecran, SPos selected){
	SDL_Surface *cursor = NULL;
	SDL_Rect position;
	
	position.x = (selected.col)*TAILLE_CASE;
	position.y = (selected.line)*TAILLE_CASE;

	cursor = IMG_Load("icons/iconeSelect.png");
	SDL_BlitSurface(cursor, NULL, ecran, &position);
}

void afficheMessage(char* message){

    SDL_Surface *ecran = NULL, *texte = NULL, *fond = NULL;
    SDL_Rect position;
    SDL_Event event;
    TTF_Font *police = NULL;
    SDL_Color couleurNoire = {255, 255, 255};
    int continuer = 1;

    SDL_Init(SDL_INIT_VIDEO); //démarrage de la SDL
    TTF_Init(); //Initialisation de SDL_TTF

    ecran = SDL_SetVideoMode(400, 100, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    //SDL_WM_SetCaption("Gestion du texte avec SDL_ttf", NULL);

    /* Chargement de la police */
    police = TTF_OpenFont("fonts/FreeMonoBold.ttf", 20);
    /* Écriture du texte dans la SDL_Surface texte en mode Blended (optimal) */
    texte = TTF_RenderText_Blended(police, message, couleurNoire);
   
    while (continuer)
    {
        SDL_WaitEvent(&event);
        switch(event.type)
        {
            case SDL_QUIT:	//cas on clique sur la croix
                continuer = 0;
                break;
	    case SDL_KEYDOWN:	//cas : appuie sur une touche
		switch(event.key.keysym.sym){
			case SDLK_ESCAPE:
				continuer = 0;
				break;
			default:
				break;
		}
        }

        SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));

        position.x = ecran->w/2 - texte->w/2;
        position.y = ecran->h/2 - texte->h/2;
        SDL_BlitSurface(texte, NULL, ecran, &position); /* Blit du texte */
        SDL_Flip(ecran);
    }

    TTF_CloseFont(police);
    TTF_Quit();

    SDL_FreeSurface(texte);
    SDL_Quit();
}

	
void initBoard(SGameState gameState, SDL_Surface *ecran, SDL_Surface *pionsRouges[12], SDL_Surface *pionsBleus[12] ){
	
	SDL_Rect position;
	int i=0, j=0;

	for(i = 0; i<10; i++){
		for(j=0; j<10; j++){
			switch(gameState.board[i][j].content){
				case ECred:
					switch(gameState.board[i][j].piece){
						case EPbomb:
							position.x = j*(ecran->w/10);
							position.y = i*(ecran->h/10);
							SDL_BlitSurface(pionsRouges[0], NULL, ecran, &position);
							break;
						case EPspy:
							position.x = j*(ecran->w/10);
							position.y = i*(ecran->h/10);
							SDL_BlitSurface(pionsRouges[1], NULL, ecran, &position);
							break;
						case EPscout:
							position.x = j*(ecran->w/10);
							position.y = i*(ecran->h/10);
							SDL_BlitSurface(pionsRouges[2], NULL, ecran, &position);
							break;
						case EPminer:
							position.x = j*(ecran->w/10);
							position.y = i*(ecran->h/10);
							SDL_BlitSurface(pionsRouges[3], NULL, ecran, &position);
							break;
						case EPsergeant:
							position.x = j*(ecran->w/10);
							position.y = i*(ecran->h/10);
							SDL_BlitSurface(pionsRouges[4], NULL, ecran, &position);
							break;
						case EPlieutenant:
							position.x = j*(ecran->w/10);
							position.y = i*(ecran->h/10);
							SDL_BlitSurface(pionsRouges[5], NULL, ecran, &position);
							break;
						case EPcaptain:
							position.x = j*(ecran->w/10);
							position.y = i*(ecran->h/10);
							SDL_BlitSurface(pionsRouges[6], NULL, ecran, &position);
							break;
						case EPmajor:
							position.x = j*(ecran->w/10);
							position.y = i*(ecran->h/10);
							SDL_BlitSurface(pionsRouges[7], NULL, ecran, &position);
							break;
						case EPcolonel:
							position.x = j*(ecran->w/10);
							position.y = i*(ecran->h/10);
							SDL_BlitSurface(pionsRouges[8], NULL, ecran, &position);
							break;
						case EPgeneral:
							position.x = j*(ecran->w/10);
							position.y = i*(ecran->h/10);
							SDL_BlitSurface(pionsRouges[9], NULL, ecran, &position);
							break;
						case EPmarshal:
							position.x = j*(ecran->w/10);
							position.y = i*(ecran->h/10);
							SDL_BlitSurface(pionsRouges[10], NULL, ecran, &position);
							break;
						case EPflag:
							position.x = j*(ecran->w/10);
							position.y = i*(ecran->h/10);
							SDL_BlitSurface(pionsRouges[11], NULL, ecran, &position);
							break;
						default:
							break;
					}
					break;

				case ECblue:
					switch(gameState.board[i][j].piece){
						case EPbomb:
							position.x = j*(ecran->w/10);
							position.y = i*(ecran->h/10);
							SDL_BlitSurface(pionsBleus[0], NULL, ecran, &position);
							break;
						case EPspy:
							position.x = j*(ecran->w/10);
							position.y = i*(ecran->h/10);
							SDL_BlitSurface(pionsBleus[1], NULL, ecran, &position);
							break;
						case EPscout:
							position.x = j*(ecran->w/10);
							position.y = i*(ecran->h/10);
							SDL_BlitSurface(pionsBleus[2], NULL, ecran, &position);
							break;
						case EPminer:
							position.x = j*(ecran->w/10);
							position.y = i*(ecran->h/10);
							SDL_BlitSurface(pionsBleus[3], NULL, ecran, &position);
							break;
						case EPsergeant:
							position.x = j*(ecran->w/10);
							position.y = i*(ecran->h/10);
							SDL_BlitSurface(pionsBleus[4], NULL, ecran, &position);
							break;
						case EPlieutenant:
							position.x = j*(ecran->w/10);
							position.y = i*(ecran->h/10);
							SDL_BlitSurface(pionsBleus[5], NULL, ecran, &position);
							break;
						case EPcaptain:
							position.x = j*(ecran->w/10);
							position.y = i*(ecran->h/10);
							SDL_BlitSurface(pionsBleus[6], NULL, ecran, &position);
							break;
						case EPmajor:
							position.x = j*(ecran->w/10);
							position.y = i*(ecran->h/10);
							SDL_BlitSurface(pionsBleus[7], NULL, ecran, &position);
							break;
						case EPcolonel:
							position.x = j*(ecran->w/10);
							position.y = i*(ecran->h/10);
							SDL_BlitSurface(pionsBleus[8], NULL, ecran, &position);
							break;
						case EPgeneral:
							position.x = j*(ecran->w/10);
							position.y = i*(ecran->h/10);
							SDL_BlitSurface(pionsBleus[9], NULL, ecran, &position);
							break;
						case EPmarshal:
							position.x = j*(ecran->w/10);
							position.y = i*(ecran->h/10);
							SDL_BlitSurface(pionsBleus[10], NULL, ecran, &position);
							break;
						case EPflag:
							position.x = j*(ecran->w/10);
							position.y = i*(ecran->h/10);
							SDL_BlitSurface(pionsBleus[11], NULL, ecran, &position);
							break;
						default:
							break;
					}
				break;

				default:
					break;
			}
		}
	}

	/*SDL_Rect position;
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
	}*/
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
	
	

	
	SDL_Rect positionFond;
	int continuer = 1;
	SDL_Event event;
	SDL_Surface *pionsRouges[12] = {NULL};
	SDL_Surface *pionsBleus[12] = {NULL};

	SDL_Init(SDL_INIT_VIDEO);	//initialisation de la SDL

	ecran = SDL_SetVideoMode(380, 377, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);	//chargement des surfaces
	SDL_WM_SetCaption("Stratego", NULL);
	
	imageFond = IMG_Load("board.png");
	
	//On remplit les deux tableaux avec les images qui vont bien
	//***************** PIONS ROUGES ***********************
	pionsRouges[0] = IMG_Load("icons/Bombe0r.png");
	SDL_SetColorKey(pionsRouges[0], SDL_SRCCOLORKEY, SDL_MapRGB(pionsRouges[0]->format, 255, 255, 255));

	pionsRouges[1] = IMG_Load("icons/spy1r.png");
	SDL_SetColorKey(pionsRouges[1], SDL_SRCCOLORKEY, SDL_MapRGB(pionsRouges[1]->format, 255, 255, 255));

	pionsRouges[2] = IMG_Load("icons/scout2r.png");
	SDL_SetColorKey(pionsRouges[2], SDL_SRCCOLORKEY, SDL_MapRGB(pionsRouges[2]->format, 255, 255, 255));

	pionsRouges[3] = IMG_Load("icons/miner3r.png");
	SDL_SetColorKey(pionsRouges[3], SDL_SRCCOLORKEY, SDL_MapRGB(pionsRouges[3]->format, 255, 255, 255));

	pionsRouges[4] = IMG_Load("icons/sergeant4r.png");
	SDL_SetColorKey(pionsRouges[4], SDL_SRCCOLORKEY, SDL_MapRGB(pionsRouges[4]->format, 255, 255, 255));

	pionsRouges[5] = IMG_Load("icons/lieutenant5r.png");
	SDL_SetColorKey(pionsRouges[5], SDL_SRCCOLORKEY, SDL_MapRGB(pionsRouges[5]->format, 255, 255, 255));

	pionsRouges[6] = IMG_Load("icons/captain6r.png");
	SDL_SetColorKey(pionsRouges[6], SDL_SRCCOLORKEY, SDL_MapRGB(pionsRouges[6]->format, 255, 255, 255));

	pionsRouges[7] = IMG_Load("icons/major7r.png");
	SDL_SetColorKey(pionsRouges[7], SDL_SRCCOLORKEY, SDL_MapRGB(pionsRouges[7]->format, 255, 255, 255));

	pionsRouges[8] = IMG_Load("icons/colonel8r.png");
	SDL_SetColorKey(pionsRouges[8], SDL_SRCCOLORKEY, SDL_MapRGB(pionsRouges[8]->format, 255, 255, 255));

	pionsRouges[9] = IMG_Load("icons/general9r.png");
	SDL_SetColorKey(pionsRouges[9], SDL_SRCCOLORKEY, SDL_MapRGB(pionsRouges[9]->format, 255, 255, 255));

	pionsRouges[10] = IMG_Load("icons/marshal10r.png");
	SDL_SetColorKey(pionsRouges[10], SDL_SRCCOLORKEY, SDL_MapRGB(pionsRouges[10]->format, 255, 255, 255));

	pionsRouges[11] = IMG_Load("icons/flag11r.png");
	SDL_SetColorKey(pionsRouges[11], SDL_SRCCOLORKEY, SDL_MapRGB(pionsRouges[11]->format, 255, 255, 255));

	//***************** PIONS BLEUS ***********************
	pionsBleus[0] = IMG_Load("icons/Bombe0b.png");
	SDL_SetColorKey(pionsBleus[0], SDL_SRCCOLORKEY, SDL_MapRGB(pionsBleus[0]->format, 255, 255, 255));

	pionsBleus[1] = IMG_Load("icons/spy1b.png");
	SDL_SetColorKey(pionsBleus[1], SDL_SRCCOLORKEY, SDL_MapRGB(pionsBleus[1]->format, 255, 255, 255));

	pionsBleus[2] = IMG_Load("icons/scout2b.png");
	SDL_SetColorKey(pionsBleus[2], SDL_SRCCOLORKEY, SDL_MapRGB(pionsBleus[2]->format, 255, 255, 255));

	pionsBleus[3] = IMG_Load("icons/miner3b.png");
	SDL_SetColorKey(pionsBleus[3], SDL_SRCCOLORKEY, SDL_MapRGB(pionsBleus[3]->format, 255, 255, 255));

	pionsBleus[4] = IMG_Load("icons/sergeant4b.png");
	SDL_SetColorKey(pionsBleus[4], SDL_SRCCOLORKEY, SDL_MapRGB(pionsBleus[4]->format, 255, 255, 255));

	pionsBleus[5] = IMG_Load("icons/lieutenant5b.png");
	SDL_SetColorKey(pionsBleus[5], SDL_SRCCOLORKEY, SDL_MapRGB(pionsBleus[5]->format, 255, 255, 255));

	pionsBleus[6] = IMG_Load("icons/captain6b.png");
	SDL_SetColorKey(pionsBleus[6], SDL_SRCCOLORKEY, SDL_MapRGB(pionsBleus[6]->format, 255, 255, 255));

	pionsBleus[7] = IMG_Load("icons/major7b.png");
	SDL_SetColorKey(pionsBleus[7], SDL_SRCCOLORKEY, SDL_MapRGB(pionsBleus[7]->format, 255, 255, 255));

	pionsBleus[8] = IMG_Load("icons/colonel8b.png");
	SDL_SetColorKey(pionsBleus[8], SDL_SRCCOLORKEY, SDL_MapRGB(pionsBleus[8]->format, 255, 255, 255));

	pionsBleus[9] = IMG_Load("icons/general9b.png");
	SDL_SetColorKey(pionsBleus[9], SDL_SRCCOLORKEY, SDL_MapRGB(pionsBleus[9]->format, 255, 255, 255));

	pionsBleus[10] = IMG_Load("icons/marshal10b.png");
	SDL_SetColorKey(pionsBleus[10], SDL_SRCCOLORKEY, SDL_MapRGB(pionsBleus[10]->format, 255, 255, 255));

	pionsBleus[11] = IMG_Load("icons/flag11b.png");
	SDL_SetColorKey(pionsBleus[11], SDL_SRCCOLORKEY, SDL_MapRGB(pionsBleus[11]->format, 255, 255, 255));


	pionRouge = IMG_Load("pionrouge.png");
	pionBleu = IMG_Load("pionbleu.png");
	
	SDL_SetColorKey(pionRouge, SDL_SRCCOLORKEY, SDL_MapRGB(pionRouge->format, 255, 255, 255));
	SDL_SetColorKey(pionBleu, SDL_SRCCOLORKEY, SDL_MapRGB(pionBleu->format, 255, 255, 255));
	
	positionFond.x = 0;
	positionFond.y = 0;
	
	SDL_BlitSurface(imageFond, NULL, ecran, &positionFond);
	
	initBoard(gameState, ecran, pionsRouges, pionsBleus);
	
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
