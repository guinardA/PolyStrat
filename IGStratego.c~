#include "PolyLib.h"
#include "IGStratego.h"
#define TAILLE_CASE 60	//Taille d'une case en pixels
#define MARGE_GAUCHE 160	//Marge gauche
#define MARGE_DROITE 160	//Marge droite
#define MARGE_BAS 60	//Marge bas
#define MARGE_HAUT 10	//Marge haut


SDL_Surface *imageFond = NULL;	//création des variables
SDL_Surface *pionsRouges[13] = {NULL};
SDL_Surface *pionsBleus[13] = {NULL};

void afficherMenu(){
	
	SDL_Surface *ecran = NULL, *texteTitre = NULL, *texte2j = NULL, *texte1j = NULL, *texte2IA = NULL, *texteQuit = NULL;
	SDL_Rect position;
    	SDL_Event event;
    	TTF_Font *policeTitre = NULL, *policeMenu = NULL;
	SDL_Color couleurNoire = {255, 255, 255};
	int continuer = 1;

	SDL_Init(SDL_INIT_VIDEO); //démarrage de la SDL
    	TTF_Init(); //Initialisation de SDL_TTF

	ecran = SDL_SetVideoMode(600, 480, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
	SDL_WM_SetCaption("Stratego", NULL);

	// Chargement des polices 
	policeTitre = TTF_OpenFont("fonts/FreeMonoBold.ttf", 60);
	policeMenu = TTF_OpenFont("fonts/FreeMonoBold.ttf", 30);
	
	// Écriture du texte dans la SDL_Surface texte en mode Blended (optimal) 
	char message[50] = "STRATEGO";
	texteTitre = TTF_RenderText_Blended(policeTitre, message, couleurNoire);

	sprintf(message, "2 joueurs");

	texte2j = TTF_RenderText_Blended(policeMenu, message, couleurNoire);

	sprintf(message, "1 joueur");
	texte1j = TTF_RenderText_Blended(policeMenu, message, couleurNoire);
	sprintf(message, "2 IA");
	texte2IA = TTF_RenderText_Blended(policeMenu, message, couleurNoire);
	sprintf(message, "Quitter");
	texteQuit = TTF_RenderText_Blended(policeMenu, message, couleurNoire);
	
	SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));
	
	position.x = ecran->w/2 - texteTitre->w/2;
    	position.y = ecran->h/5 - texteTitre->h/2;
   	SDL_BlitSurface(texteTitre, NULL, ecran, &position);

	position.x = ecran->w/2 - texte1j->w/2;
    	position.y = 2*(ecran->h/5) - texte1j->h/2;
   	SDL_BlitSurface(texte1j, NULL, ecran, &position);

	position.x = ecran->w/2 - texte2j->w/2;
    	//position.y = 3*(ecran->h/5) - texte2j->h/2;
	position.y += 50;

   	SDL_BlitSurface(texte2j, NULL, ecran, &position);

	position.x = ecran->w/2 - texte2IA->w/2;
    	//position.y = 4*(ecran->h/5) - texte2IA->h/2;
	position.y += 50;
   	SDL_BlitSurface(texte2IA, NULL, ecran, &position);

	position.x = ecran->w/2 - texteQuit->w/2;
    	//position.y = 5*(ecran->h/5) - texteQuit->h/2;
	position.y += 50;
   	SDL_BlitSurface(texteQuit, NULL, ecran, &position);

	SDL_Flip(ecran);

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
    }

    TTF_CloseFont(policeTitre);
    TTF_CloseFont(policeMenu);
    TTF_Quit();

    SDL_FreeSurface(texteTitre);
    SDL_FreeSurface(texte2j);
    SDL_FreeSurface(texte1j);
    SDL_FreeSurface(texte2IA);
    SDL_FreeSurface(texteQuit);
    SDL_Quit(); //faut faire gaffe ça va ptet buger içi
}		

void selectionnerPion(SDL_Surface *ecran, SPos selected){
	SDL_Surface *cursor = NULL;
	SDL_Rect position;
	
	position.x = ((selected.col)*TAILLE_CASE)+MARGE_GAUCHE;
	position.y = ((selected.line)*TAILLE_CASE)+MARGE_HAUT;

	cursor = IMG_Load("icons/iconSelected.png");
	SDL_BlitSurface(cursor, NULL, ecran, &position);
}

void afficheMessage(char* message){
    SDL_Surface *ecran = NULL, *texte = NULL, *textOk = NULL;
    SDL_Rect position;
    SDL_Event event;
    TTF_Font *police = NULL, *policeOk = NULL;
    SDL_Color couleurNoire = {255, 255, 255};
    int continuer = 1;
    SDL_Init(SDL_INIT_VIDEO); //démarrage de la SDL
    TTF_Init(); //Initialisation de SDL_TTF
    ecran = SDL_SetVideoMode(600, 200, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    //SDL_WM_SetCaption("Gestion du texte avec SDL_ttf", NULL);
    /* Chargement de la police */
    police = TTF_OpenFont("fonts/FreeMonoBold.ttf", 30);
    policeOk = TTF_OpenFont("fonts/FreeMonoBold.ttf", 15);
    /* Écriture du texte dans la SDL_Surface texte en mode Blended (optimal) */
    texte = TTF_RenderText_Blended(police, message, couleurNoire);
    char *messageOk = "Ok";
    textOk = TTF_RenderText_Blended(police, messageOk, couleurNoire);
   
    SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));
    position.x = ecran->w/2 - texte->w/2;
    position.y = ecran->h/3 - texte->h/2;
    SDL_BlitSurface(texte, NULL, ecran, &position); /* Blit du texte */
    position.x = ecran->w/2 - textOk->w/2;
    position.y = 2*(ecran->h/3) - textOk->h/2;
    SDL_BlitSurface(textOk, NULL, ecran, &position);
    SDL_Flip(ecran);
    while (continuer)
    {
        SDL_WaitEvent(&event);
        switch(event.type)
        {	
	    case SDL_MOUSEBUTTONUP:
		if(((event.button.y) > 115) && ((event.button.y) < 145) && ((event.button.x) > 280) && ((event.button.x) < 320)){
			
		}
	    default:
		break;	
        }
    }
    TTF_CloseFont(police);
    TTF_Quit();
    SDL_FreeSurface(texte);
    SDL_Quit(); //faut faire gaffe ça va ptet buger içi
}

int afficheMessageDemande(char* message){

    SDL_Surface *ecran = NULL, *texteQuestion = NULL, *texteOui = NULL, *texteNon = NULL;
    SDL_Rect position;
    SDL_Event event;
    TTF_Font *policeQuestion = NULL, *policeOui = NULL, *policeNon = NULL;
    SDL_Color couleurNoire = {255, 255, 255};
    int continuer = 1, retour;
    char *oui = "Oui", *non = "Non";

    SDL_Init(SDL_INIT_VIDEO); //démarrage de la SDL
    TTF_Init(); //Initialisation de SDL_TTF

    ecran = SDL_SetVideoMode(400, 100, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    //SDL_WM_SetCaption("Gestion du texte avec SDL_ttf", NULL);

    /* Chargement de la police */
    policeQuestion = TTF_OpenFont("fonts/FreeMonoBold.ttf", 20);
    policeOui = TTF_OpenFont("fonts/FreeMonoBold.ttf", 15);
    policeNon = TTF_OpenFont("fonts/FreeMonoBold.ttf", 15);
    /* Écriture du texte dans la SDL_Surface texte en mode Blended (optimal) */
    texteQuestion = TTF_RenderText_Blended(policeQuestion, message, couleurNoire);
    texteOui = TTF_RenderText_Blended(policeOui, oui, couleurNoire);
    texteNon = TTF_RenderText_Blended(policeNon, non, couleurNoire);

    SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));

    position.x = ecran->w/2 - texteQuestion->w/2;
    position.y = ecran->h/3 - texteQuestion->h/3;
    SDL_BlitSurface(texteQuestion, NULL, ecran, &position); /* Blit du texte */

	position.x = ecran->w/3 - texteOui->w/2;
        position.y = 2*(ecran->h/3 - texteOui->h/3);

	SDL_BlitSurface(texteOui, NULL, ecran, &position);

	position.x = 2*(ecran->w/3 - texteNon->w/2);
        position.y = 2*(ecran->h/3 - texteNon->h/3);

	SDL_BlitSurface(texteNon, NULL, ecran, &position);
        SDL_Flip(ecran);

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
	    case SDL_MOUSEBUTTONUP:	
		//cas quand on clique sur oui
		if(((event.button.y) > 50) && ((event.button.y) < 80) && ((event.button.x) > 100) && ((event.button.x) < 160)){
			fflush(stdout);
			retour = 1;
			continuer = 0;
		}

		if(((event.button.y) > 50) && ((event.button.y) < 80) && ((event.button.x) > 230) && ((event.button.x) < 300)){
			fflush(stdout);
			retour = 0;
			continuer = 0;
		}
		break;
        }
    }

    TTF_CloseFont(policeQuestion);
    TTF_CloseFont(policeOui);
    TTF_CloseFont(policeNon);
    TTF_Quit();

    SDL_FreeSurface(texteQuestion);
    SDL_FreeSurface(texteOui);
    SDL_FreeSurface(texteNon);
    SDL_Quit(); //faut faire gaffe ça va ptet buger içi
    
    return retour;
}

	
void initBoard(SGameState gameState, SDL_Surface *ecran){
	
	SDL_Rect position;
	int i=0, j=0;

	SDL_Rect positionFond;

	imageFond = IMG_Load("icons/board.png");
	
	//On remplit les deux tableaux avec les images qui vont bien
	//***************** PIONS ROUGES ***********************
	
	pionsRouges[0] = IMG_Load("icons/bomb0r.png");
	SDL_SetColorKey(pionsRouges[0], SDL_SRCCOLORKEY, SDL_MapRGB(pionsRouges[0]->format, 255, 255, 255));
printf("chargement\n");
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
	
	pionsRouges[12] = IMG_Load("icons/noner.png");
	SDL_SetColorKey(pionsRouges[12], SDL_SRCCOLORKEY, SDL_MapRGB(pionsRouges[12]->format, 255, 255, 255));

	//***************** PIONS BLEUS ***********************
	pionsBleus[0] = IMG_Load("icons/bomb0b.png");
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
	
	pionsBleus[12] = IMG_Load("icons/noneb.png");
	SDL_SetColorKey(pionsBleus[12], SDL_SRCCOLORKEY, SDL_MapRGB(pionsBleus[12]->format, 255, 255, 255));
	
	positionFond.x = 0;
	positionFond.y = 0;

	SDL_BlitSurface(imageFond, NULL, ecran, &positionFond);

	for(i = 0; i<10; i++){
		for(j=0; j<10; j++){
			
			int widthScreen = j*((ecran->w-(MARGE_DROITE+MARGE_GAUCHE))/10)+MARGE_GAUCHE;
			int heightScreen = i*((ecran->h-(MARGE_HAUT+MARGE_BAS))/10)+MARGE_HAUT;	

			switch(gameState.board[i][j].content){
				case ECred:
					switch(gameState.board[i][j].piece){
						case EPbomb:
							position.x = widthScreen;
							position.y = heightScreen;
							SDL_BlitSurface(pionsRouges[0], NULL, ecran, &position);
							break;
						case EPspy:
							position.x = widthScreen;
							position.y = heightScreen;
							SDL_BlitSurface(pionsRouges[1], NULL, ecran, &position);
							break;
						case EPscout:
							position.x = widthScreen;
							position.y = heightScreen;
							SDL_BlitSurface(pionsRouges[2], NULL, ecran, &position);
							break;
						case EPminer:
							position.x = widthScreen;
							position.y = heightScreen;
							SDL_BlitSurface(pionsRouges[3], NULL, ecran, &position);
							break;
						case EPsergeant:
							position.x = widthScreen;
							position.y = heightScreen;
							SDL_BlitSurface(pionsRouges[4], NULL, ecran, &position);
							break;
						case EPlieutenant:
							position.x = widthScreen;
							position.y = heightScreen;
							SDL_BlitSurface(pionsRouges[5], NULL, ecran, &position);
							break;
						case EPcaptain:
							position.x = widthScreen;
							position.y = heightScreen;
							SDL_BlitSurface(pionsRouges[6], NULL, ecran, &position);
							break;
						case EPmajor:
							position.x = widthScreen;
							position.y = heightScreen;
							SDL_BlitSurface(pionsRouges[7], NULL, ecran, &position);
							break;
						case EPcolonel:
							position.x = widthScreen;
							position.y = heightScreen;
							SDL_BlitSurface(pionsRouges[8], NULL, ecran, &position);
							break;
						case EPgeneral:
							position.x = widthScreen;
							position.y = heightScreen;
							SDL_BlitSurface(pionsRouges[9], NULL, ecran, &position);
							break;
						case EPmarshal:
							position.x = widthScreen;
							position.y = heightScreen;
							SDL_BlitSurface(pionsRouges[10], NULL, ecran, &position);
							break;
						case EPflag:
							position.x = widthScreen;
							position.y = heightScreen;
							SDL_BlitSurface(pionsRouges[11], NULL, ecran, &position);
							break;
						case EPnone:
							position.x = widthScreen;
							position.y = heightScreen;
							SDL_BlitSurface(pionsRouges[12], NULL, ecran, &position);
							break;
						default:
							break;
					}
					break;

				case ECblue:
					switch(gameState.board[i][j].piece){
						case EPbomb:
							position.x = widthScreen;
							position.y = heightScreen;
							SDL_BlitSurface(pionsBleus[0], NULL, ecran, &position);
							break;
						case EPspy:
							position.x = widthScreen;
							position.y = heightScreen;
							SDL_BlitSurface(pionsBleus[1], NULL, ecran, &position);
							break;
						case EPscout:
							position.x = widthScreen;
							position.y = heightScreen;
							SDL_BlitSurface(pionsBleus[2], NULL, ecran, &position);
							break;
						case EPminer:
							position.x = widthScreen;
							position.y = heightScreen;
							SDL_BlitSurface(pionsBleus[3], NULL, ecran, &position);
							break;
						case EPsergeant:
							position.x = widthScreen;
							position.y = heightScreen;
							SDL_BlitSurface(pionsBleus[4], NULL, ecran, &position);
							break;
						case EPlieutenant:
							position.x = widthScreen;
							position.y = heightScreen;
							SDL_BlitSurface(pionsBleus[5], NULL, ecran, &position);
							break;
						case EPcaptain:
							position.x = widthScreen;
							position.y = heightScreen;
							SDL_BlitSurface(pionsBleus[6], NULL, ecran, &position);
							break;
						case EPmajor:
							position.x = widthScreen;
							position.y = heightScreen;
							SDL_BlitSurface(pionsBleus[7], NULL, ecran, &position);
							break;
						case EPcolonel:
							position.x = widthScreen;
							position.y = heightScreen;
							SDL_BlitSurface(pionsBleus[8], NULL, ecran, &position);
							break;
						case EPgeneral:
							position.x = widthScreen;
							position.y = heightScreen;
							SDL_BlitSurface(pionsBleus[9], NULL, ecran, &position);
							break;
						case EPmarshal:
							position.x = widthScreen;
							position.y = heightScreen;
							SDL_BlitSurface(pionsBleus[10], NULL, ecran, &position);
							break;
						case EPflag:
							position.x = widthScreen;
							position.y = heightScreen;
							SDL_BlitSurface(pionsBleus[11], NULL, ecran, &position);
							break;
						case EPnone:
							position.x = widthScreen;
							position.y = heightScreen;
							SDL_BlitSurface(pionsBleus[12], NULL, ecran, &position);
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

}

SMove renvoieCoordonnees(){
	
	SDL_Event event;
	SPos positionDepart, positionArrivee;
	SMove mouvementPion;
	int continuer = 1;
	while(continuer){
		SDL_WaitEvent(&event);
		switch(event.type){
				case SDL_MOUSEBUTTONUP:
					//il faut que ce soit un clic gauche et qu'il se situe dans la grille de jeu
					if((event.button.button == SDL_BUTTON_LEFT) && ((event.button.y)>MARGE_HAUT) && ((event.button.y)<(MARGE_HAUT+10*TAILLE_CASE)) && ((event.button.x)>MARGE_GAUCHE) && ((event.button.x)<(MARGE_GAUCHE+10*TAILLE_CASE))){
						positionDepart.line = ((event.button.y-(MARGE_HAUT))/TAILLE_CASE);
						positionDepart.col = ((event.button.x-(MARGE_GAUCHE))/TAILLE_CASE);
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
					if((event.button.button == SDL_BUTTON_LEFT) && ((event.button.y)>MARGE_HAUT) && ((event.button.y)<(MARGE_HAUT+10*TAILLE_CASE)) && ((event.button.x)>MARGE_GAUCHE) && ((event.button.x)<(MARGE_GAUCHE+10*TAILLE_CASE))){
						positionArrivee.line = ((event.button.y-MARGE_HAUT)/TAILLE_CASE);
						positionArrivee.col = ((event.button.x-MARGE_GAUCHE)/TAILLE_CASE);
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
	
	
	SDL_Surface *ecran = NULL;	//création des variables
	int continuer = 1;
	SDL_Event event;


	SDL_Init(SDL_INIT_VIDEO);	//initialisation de la SDL

	ecran = SDL_SetVideoMode(920, 670, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);	//chargement des surfaces
	SDL_WM_SetCaption("Stratego", NULL);
	
	initBoard(gameState, ecran);
	
	SDL_Flip(ecran);
	//Boucle pour tester la selection de pions
	/*while (continuer)
    {renvoieCoordonnees(ecran,gameState);
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
    }*/
	return EXIT_SUCCESS;
	
}

//Retourne la position de la case sur laquelle on a cliqué, si ce n'est pas une case on renvoie une variable SPos avec -1 comme valeur de ligne et colonne
SPos getPos(){
	int continuer = 1;
	SDL_Event event;
	SPos posClic;	

	while(continuer){
		SDL_WaitEvent(&event);
		switch(event.type){
			case SDL_MOUSEBUTTONUP:
				if(event.button.button == SDL_BUTTON_LEFT){
					if(((event.button.y)>MARGE_HAUT) && ((event.button.y)<(MARGE_HAUT+10*TAILLE_CASE)) && ((event.button.x)>MARGE_GAUCHE) && ((event.button.x)<(MARGE_GAUCHE+10*TAILLE_CASE))){
						posClic.line = ((event.button.y-MARGE_HAUT)/TAILLE_CASE);
						posClic.col = ((event.button.x-MARGE_GAUCHE)/TAILLE_CASE);
						continuer = 0;
					}
					else{
						posClic.line = -1;
						posClic.col = -1;
						continuer = 0;
					}
					break;
				}
				else{
					break;
				}
			default:
				break;
		}
	}
	return posClic;
}

void quitter_sdl(){
	
	int i;

	for(i=0;i<12;i++){
		SDL_FreeSurface(pionsRouges[i]);
		SDL_FreeSurface(pionsBleus[i]);
	}
	
	SDL_FreeSurface(imageFond);
	SDL_Quit(); //on quitte la SDL
	
}
