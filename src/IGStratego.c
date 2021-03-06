/**
 * Projet Stratego, 3ème année de polytech
 * Réalisé par : -Guinard Arnaud
 * 				 -Mouden Benjamin
 * 				 -Evo Brieuc
 * 				 -Chotard François
 */

#include "PolyLib.h"
#include "IGStratego.h"
#define TAILLE_CASE 60	//Taille d'une case en pixels
#define MARGE_GAUCHE 160	//Marge gauche
#define MARGE_DROITE 160	//Marge droite
#define MARGE_BAS 60	//Marge bas
#define MARGE_HAUT 10	//Marge haut


SDL_Surface *imageFond = NULL;	//Surface qui contient l'image de fond
SDL_Surface *pionsRouges[13] = {NULL};	//Tableau de surfaces pour les pions rouges
SDL_Surface *pionsBleus[13] = {NULL};	//Tableau de surfaces pour les pions bleus
SDL_Surface *ecran = NULL;		//Ecran principal
SDL_Surface *pionsRedLeft[11] = {NULL};	//Tableau de surfaces pour les pions restants rouges
SDL_Surface *pionsBlueLeft[11] = {NULL};//Tableau de surfaces pour les pions restants bleus
SDL_Surface *texteBlue[11] = {NULL};	//Tableau de surfaces pour les textes qui affichent le nombre de pions bleus restantes
SDL_Surface *texteRed[11] = {NULL};	//Tableau de surfaces pour les textes qui affichent le nombre de pions rouges restantes

//Méthode qui permet d'afficher un message dans l'encadré en bas de l'écran
void afficherMessageEcran(char *message, int delay){
	//Déclaration des variables
	SDL_Surface *texteTitre = NULL, *ecrantmp = ecran;
	SDL_Rect position;
	TTF_Font *police = NULL;
	SDL_Color couleurNoire = {0, 0, 0};

	TTF_Init();	//Initialisation de SDL_TTF
	
	police = TTF_OpenFont("fonts/FreeMonoBold.ttf", 15);	//Chargement de la police
	texteTitre = TTF_RenderText_Blended(police, message, couleurNoire);	//Ecriture du texte dans la SDL_Surface texteTitre
	position.y = ecran->h - MARGE_BAS + (texteTitre->h)/2 + 22;
    	position.x = ecran->w/2 - texteTitre->w/2;
	SDL_BlitSurface(texteTitre, NULL, ecran, &position);	//On Blit le texteTitre à l'écran

	SDL_Flip(ecran);	//On rafraichit l'écran
	SDL_Delay(delay);	//Délai d'affichage du message
	TTF_CloseFont(police);
	TTF_Quit();
	ecran = ecrantmp;	//On réaffiche l'écran comme il était avant
	SDL_Flip(ecran);
	
	SDL_FreeSurface(texteTitre);
	SDL_FreeSurface(ecrantmp);
	
}

//Affiche le menu d'accueil. Renvoie 0 pour Quitter, 1 pour 1Joueur, 2 pour 2Joueurs, 3 pour 2IA
int afficherMenu(){
	
	//Déclaration des variables
	SDL_Surface *texteTitre = NULL, *texte2j = NULL, *texte1j = NULL, *texte2IA = NULL, *texteQuit = NULL;
	SDL_Rect position;
    	SDL_Event event;
    	TTF_Font *policeTitre = NULL, *policeMenu = NULL;
	SDL_Color couleurNoire = {255, 255, 255};
	int continuer = 1, clicMenu = 0;

	SDL_Init(SDL_INIT_VIDEO); //démarrage de la SDL
    	TTF_Init(); //Initialisation de SDL_TTF

	ecran = SDL_SetVideoMode(600, 480, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
	SDL_WM_SetCaption("Stratego", NULL);

	// Chargement des polices 
	policeTitre = TTF_OpenFont("fonts/FreeMonoBold.ttf", 60);
	policeMenu = TTF_OpenFont("fonts/FreeMonoBold.ttf", 30);
	
	char message[50] = "STRATEGO";
	texteTitre = TTF_RenderText_Blended(policeTitre, message, couleurNoire);
	
	//On remplit les différents textes et on charge les polices
	sprintf(message, "2 joueurs");
	texte2j = TTF_RenderText_Blended(policeMenu, message, couleurNoire);

	sprintf(message, "1 joueur");
	texte1j = TTF_RenderText_Blended(policeMenu, message, couleurNoire);

	sprintf(message, "2 IA");
	texte2IA = TTF_RenderText_Blended(policeMenu, message, couleurNoire);

	sprintf(message, "Quitter");
	texteQuit = TTF_RenderText_Blended(policeMenu, message, couleurNoire);
	
	SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));
	
	//Placement des zones de textes à l'écran
	position.x = ecran->w/2 - texteTitre->w/2;
    	position.y = ecran->h/5 - texteTitre->h/2;
   	SDL_BlitSurface(texteTitre, NULL, ecran, &position);

	position.x = ecran->w/2 - texte1j->w/2;
    	position.y = 2*(ecran->h/5) - texte1j->h/2;
   	SDL_BlitSurface(texte1j, NULL, ecran, &position);

	position.x = ecran->w/2 - texte2j->w/2;
	position.y += 50;

   	SDL_BlitSurface(texte2j, NULL, ecran, &position);

	position.x = ecran->w/2 - texte2IA->w/2;
	position.y += 50;
   	SDL_BlitSurface(texte2IA, NULL, ecran, &position);

	position.x = ecran->w/2 - texteQuit->w/2;
	position.y += 50;
   	SDL_BlitSurface(texteQuit, NULL, ecran, &position);

	SDL_Flip(ecran);

	//On attend un évenement de le joueur et on agit en réaction
	while (continuer)
    {
        SDL_WaitEvent(&event);
        switch(event.type)
        {
	    //Cas on clique sur la croix
            case SDL_QUIT:	
                continuer = 0;
                break;
	    case SDL_MOUSEBUTTONUP:
		//Cas ou on clique sur 1 joueur
		if(((event.button.y) > 180) && ((event.button.y) < 205) && ((event.button.x) > 225) && ((event.button.x) < 380)){
			clicMenu = 1;
			continuer = 0;
			break;
		}
		//Cas ou on clique sur 2 joueurs
		else if(((event.button.y) > 230) && ((event.button.y) < 255) && ((event.button.x) > 210) && ((event.button.x) < 385)){
			clicMenu = 2;
			continuer = 0;
			break;
		}
		//Cas ou on appuie sur 2IA
		else if(((event.button.y) > 280) && ((event.button.y) < 305) && ((event.button.x) > 245) && ((event.button.x) < 340)){
			clicMenu = 3;
			continuer = 0;
			break;
		}
		//Cas ou on clique sur quitter
		else if(((event.button.y) > 330) && ((event.button.y) < 360) && ((event.button.x) > 230) && ((event.button.x) < 370)){
			clicMenu = 0;
			continuer = 0;
			break;
		}
		break;
        }
    }

    //Libération des différentes variables
    TTF_CloseFont(policeTitre);
    TTF_CloseFont(policeMenu);
    TTF_Quit();

    SDL_FreeSurface(texteTitre);
    SDL_FreeSurface(texte2j);
    SDL_FreeSurface(texte1j);
    SDL_FreeSurface(texte2IA);
    SDL_FreeSurface(texteQuit);
    SDL_Quit();
    return clicMenu;
}		

//Méthode qui permet de mettre en évidence le pion qui à été sélectionné
void selectionnerPion(SDL_Surface *ecran, SPos selected, SGameState gameState){
	//Déclaration des variables
	SDL_Surface *cursor = NULL;
	SDL_Rect position;
	
	//Calcul de la position du curseur en fonction du paramètre SPos selected
	position.x = ((selected.col)*TAILLE_CASE)+MARGE_GAUCHE;
	position.y = ((selected.line)*TAILLE_CASE)+MARGE_HAUT;

	//Chargement de l'image du curseur
	cursor = IMG_Load("icons/iconSelected.png");

	//Si la position passée en paramètre contient un pion on affiche le curseur
	if((gameState.board[selected.line][selected.col].content == ECred) || (gameState.board[selected.line][selected.col].content == ECblue)){
		SDL_BlitSurface(cursor, NULL, ecran, &position);
		SDL_Flip(ecran);
	}
	SDL_FreeSurface(cursor);
}

//A SUPPRIMER
void afficheMessage(char* message){
    SDL_Surface *texte = NULL, *textOk = NULL;
    SDL_Rect position;
    SDL_Event event;
    TTF_Font *police = NULL, *policeOk = NULL;
    SDL_Color couleurNoire = {255, 255, 255};
    int continuer = 1;
    SDL_Init(SDL_INIT_VIDEO); //démarrage de la SDL
    TTF_Init(); //Initialisation de SDL_TTF
    ecran = SDL_SetVideoMode(600, 200, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);

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
    TTF_CloseFont(policeOk);
    TTF_Quit();
    SDL_FreeSurface(texte);
    SDL_FreeSurface(textOk);
    SDL_Quit(); //faut faire gaffe ça va ptet buger içi
}

//Méthode qui permet d'afficher une fenêtre popup
int afficheMessageDemande(char* message){

    //Déclaration des variables
    SDL_Surface *texteQuestion = NULL, *texteOui = NULL, *texteNon = NULL;
    SDL_Rect position;
    SDL_Event event;
    TTF_Font *policeQuestion = NULL, *policeOui = NULL, *policeNon = NULL;
    SDL_Color couleurNoire = {255, 255, 255};
    int continuer = 1, retour;
    char *oui = "Oui", *non = "Non";

    SDL_Init(SDL_INIT_VIDEO); //démarrage de la SDL
    TTF_Init(); //Initialisation de SDL_TTF

    ecran = SDL_SetVideoMode(400, 100, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);

    /* Chargement des police */
    policeQuestion = TTF_OpenFont("fonts/FreeMonoBold.ttf", 20);
    policeOui = TTF_OpenFont("fonts/FreeMonoBold.ttf", 15);
    policeNon = TTF_OpenFont("fonts/FreeMonoBold.ttf", 15);

    /* Écriture des textes dans les SDL_Surface */
    texteQuestion = TTF_RenderText_Blended(policeQuestion, message, couleurNoire);
    texteOui = TTF_RenderText_Blended(policeOui, oui, couleurNoire);
    texteNon = TTF_RenderText_Blended(policeNon, non, couleurNoire);

    SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));

    //Placement des surfaces texte à l'écran
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

    //On attend que le joueur agisse et on réagit en fonction de ce qu'il fait
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
		//cas quand on clique sur non
		if(((event.button.y) > 50) && ((event.button.y) < 80) && ((event.button.x) > 230) && ((event.button.x) < 300)){
			fflush(stdout);
			retour = 0;
			continuer = 0;
		}
		break;
        }
    }

    //Libération de la mémoire
    TTF_CloseFont(policeQuestion);
    TTF_CloseFont(policeOui);
    TTF_CloseFont(policeNon);
    TTF_Quit();

    SDL_FreeSurface(texteQuestion);
    SDL_FreeSurface(texteOui);
    SDL_FreeSurface(texteNon);
    SDL_Quit();
    
    return retour;
}

//Méthode qui permet d'initialiser le plateau de jeu	
void initBoard(SGameState gameState, SDL_Surface *ecran){
	
	//Déclaration des variables
	SDL_Rect position;
	int i=0, j=0;
	SDL_Rect positionFond;
	imageFond = IMG_Load("icons/board.png");
	
	//On remplit les deux tableaux avec les images qui vont bien
	//***************** PIONS ROUGES ***********************
	
	pionsRouges[0] = IMG_Load("icons/bomb0r.png");
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

	//On charge l'image de fond
	SDL_BlitSurface(imageFond, NULL, ecran, &positionFond);

	//Double boucle qui permet de parcourir tout le plateau
	for(i = 0; i<10; i++){
		for(j=0; j<10; j++){
			
			//Calcul de la position de la prochaine pièce
			int widthScreen = j*((ecran->w-(MARGE_DROITE+MARGE_GAUCHE))/10)+MARGE_GAUCHE;
			int heightScreen = i*((ecran->h-(MARGE_HAUT+MARGE_BAS))/10)+MARGE_HAUT;	

			//On vérifie la couleur et le type de la pièce pour charger la bonne image
			switch(gameState.board[i][j].content){
				//Cas ou c'est un pion rouge
				case ECred:
					switch(gameState.board[i][j].piece){
						case EPbomb:	//Bombe
							position.x = widthScreen;
							position.y = heightScreen;
							SDL_BlitSurface(pionsRouges[0], NULL, ecran, &position);
							break;
						case EPspy:	//Espion
							position.x = widthScreen;
							position.y = heightScreen;
							SDL_BlitSurface(pionsRouges[1], NULL, ecran, &position);
							break;
						case EPscout:	//Eclaireur
							position.x = widthScreen;
							position.y = heightScreen;
							SDL_BlitSurface(pionsRouges[2], NULL, ecran, &position);
							break;
						case EPminer:	//Démineur
							position.x = widthScreen;
							position.y = heightScreen;
							SDL_BlitSurface(pionsRouges[3], NULL, ecran, &position);
							break;
						case EPsergeant://Sergent
							position.x = widthScreen;
							position.y = heightScreen;
							SDL_BlitSurface(pionsRouges[4], NULL, ecran, &position);
							break;
						case EPlieutenant://Lieutenant
							position.x = widthScreen;
							position.y = heightScreen;
							SDL_BlitSurface(pionsRouges[5], NULL, ecran, &position);
							break;
						case EPcaptain:	//Capitaine
							position.x = widthScreen;
							position.y = heightScreen;
							SDL_BlitSurface(pionsRouges[6], NULL, ecran, &position);
							break;
						case EPmajor:	//Major
							position.x = widthScreen;
							position.y = heightScreen;
							SDL_BlitSurface(pionsRouges[7], NULL, ecran, &position);
							break;
						case EPcolonel:	//Colonel
							position.x = widthScreen;
							position.y = heightScreen;
							SDL_BlitSurface(pionsRouges[8], NULL, ecran, &position);
							break;
						case EPgeneral:	//Général
							position.x = widthScreen;
							position.y = heightScreen;
							SDL_BlitSurface(pionsRouges[9], NULL, ecran, &position);
							break;
						case EPmarshal:	//Marshal
							position.x = widthScreen;
							position.y = heightScreen;
							SDL_BlitSurface(pionsRouges[10], NULL, ecran, &position);
							break;
						case EPflag:	//Drapeau
							position.x = widthScreen;
							position.y = heightScreen;
							SDL_BlitSurface(pionsRouges[11], NULL, ecran, &position);
							break;
						case EPnone:	//Vide
							position.x = widthScreen;
							position.y = heightScreen;
							SDL_BlitSurface(pionsRouges[12], NULL, ecran, &position);
							break;
						default:
							break;
					}
					break;
				//Cas ou c'est un pion bleu
				case ECblue:
					switch(gameState.board[i][j].piece){
						case EPbomb:	//Bombe
							position.x = widthScreen;
							position.y = heightScreen;
							SDL_BlitSurface(pionsBleus[0], NULL, ecran, &position);
							break;
						case EPspy:	//Espion
							position.x = widthScreen;
							position.y = heightScreen;
							SDL_BlitSurface(pionsBleus[1], NULL, ecran, &position);
							break;
						case EPscout:	//Eclaireur
							position.x = widthScreen;
							position.y = heightScreen;
							SDL_BlitSurface(pionsBleus[2], NULL, ecran, &position);
							break;
						case EPminer:	//Démineur
							position.x = widthScreen;
							position.y = heightScreen;
							SDL_BlitSurface(pionsBleus[3], NULL, ecran, &position);
							break;
						case EPsergeant://Sergent
							position.x = widthScreen;
							position.y = heightScreen;
							SDL_BlitSurface(pionsBleus[4], NULL, ecran, &position);
							break;
						case EPlieutenant://Lieutenant
							position.x = widthScreen;
							position.y = heightScreen;
							SDL_BlitSurface(pionsBleus[5], NULL, ecran, &position);
							break;
						case EPcaptain:	//Capitaine
							position.x = widthScreen;
							position.y = heightScreen;
							SDL_BlitSurface(pionsBleus[6], NULL, ecran, &position);
							break;
						case EPmajor:	//Major
							position.x = widthScreen;
							position.y = heightScreen;
							SDL_BlitSurface(pionsBleus[7], NULL, ecran, &position);
							break;
						case EPcolonel:	//Colonel
							position.x = widthScreen;
							position.y = heightScreen;
							SDL_BlitSurface(pionsBleus[8], NULL, ecran, &position);
							break;
						case EPgeneral:	//Général
							position.x = widthScreen;
							position.y = heightScreen;
							SDL_BlitSurface(pionsBleus[9], NULL, ecran, &position);
							break;
						case EPmarshal:	//Marshal
							position.x = widthScreen;
							position.y = heightScreen;
							SDL_BlitSurface(pionsBleus[10], NULL, ecran, &position);
							break;
						case EPflag:	//Drapeau
							position.x = widthScreen;
							position.y = heightScreen;
							SDL_BlitSurface(pionsBleus[11], NULL, ecran, &position);
							break;
						case EPnone:	//Vide
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

//Méthode qui renvoie les coordonnées du mouvement correspondant à deux clics consecutifs du joueur
SMove renvoieCoordonnees(SGameState gameState){
	
	//Déclaration des variables
	SDL_Event event;
	SPos positionDepart, positionArrivee;
	SMove mouvementPion;
	int continuer = 1;
	
	//On attend que l'utilisateur ait cliqué sur une case de la grille
	while(continuer){
		SDL_WaitEvent(&event);
		switch(event.type){
				//Cas ou on clique sur la croix, on renvoie un SMove avec -2/-2 -2/-2 comme valeurs
				case SDL_QUIT:
					mouvementPion.start.line = -2;
					mouvementPion.start.col = -2;
					mouvementPion.end.line = -2;
					mouvementPion.end.col = -2;
					continuer = 0;
					break;
				//Cas ou le joueur fait un clic de souris
				case SDL_MOUSEBUTTONUP:
					//il faut que ce soit un clic gauche et situé sur une case de la grille de jeu
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
	
	//On vérifie que le joueur n'ait pas cliqué sur la croix
	if((mouvementPion.start.line != -2) && (mouvementPion.start.col != -2) && (mouvementPion.end.col != -2) && (mouvementPion.end.line != -2)){
		selectionnerPion(ecran, positionDepart, gameState); 
		continuer = 1;
	}

	//On attend que le joueur ait cliqué sur une autre case
	while(continuer){
		
		SDL_WaitEvent(&event);
		switch(event.type){
				//Cas ou on clique sur la croix, on renvoie un SMove avec -2/-2 -2/-2 comme valeurs
				case SDL_QUIT:
					mouvementPion.start.line = -2;
					mouvementPion.start.col = -2;
					mouvementPion.end.line = -2;
					mouvementPion.end.col = -2;
					continuer = 0;
					break;
				//Cas ou le joueur fait un clic de souris
				case SDL_MOUSEBUTTONUP:
					//il faut que ce soit un clic gauche et situé sur une case de la grille de jeu
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
	//On vérifie que le joueur n'ait pas cliqué sur la croix avant de changer les valeur pour ne pas écraser celles que l'on a donné dans le switch
	if((mouvementPion.start.line != -2) && (mouvementPion.start.col != -2) && (mouvementPion.end.col != -2) && (mouvementPion.end.line != -2)){
		mouvementPion.start = positionDepart;
		mouvementPion.end = positionArrivee;
	}
	
	return mouvementPion;
}

//Méthode qui initialise l'interface
void initInterface(){
	SDL_Init(SDL_INIT_VIDEO);	//initialisation de la SDL

	ecran = SDL_SetVideoMode(920, 670, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);	//chargement des surfaces
	SDL_WM_SetCaption("Stratego", NULL);

}

//Méthode qui permet rafraichir l'écran par rapport au gamestate passé en paramètre
int interfaceGraphique(SGameState gameState){
	
	initBoard(gameState, ecran);

	afficherPiecesRestantes(gameState, ecran);
	SDL_Flip(ecran);
	return EXIT_SUCCESS;
}

//Retourne la position de la case sur laquelle on a cliqué, si ce n'est pas une case on renvoie une variable SPos avec -1 comme valeur de ligne et colonne
SPos getPos(){
	
	//Déclaration des variables
	int continuer = 1;
	SDL_Event event;
	SPos posClic;	

	//On attend que le joueur clique sur la grille de jeu 
	while(continuer){
		SDL_WaitEvent(&event);
		switch(event.type){
			//Si le joueur clique sur la croix on renvoie un SPos avec -2/-2 comme valeurs
			case SDL_QUIT:
				posClic.line = -2;
				posClic.col = -2;
				continuer = 0;
				break;
			case SDL_MOUSEBUTTONUP:
				//Il faut que ce soit un clic gauche
				if(event.button.button == SDL_BUTTON_LEFT){
					//On vérifie que le clic se situe bien sur la grille
					if(((event.button.y)>MARGE_HAUT) && ((event.button.y)<(MARGE_HAUT+10*TAILLE_CASE)) && ((event.button.x)>MARGE_GAUCHE) && ((event.button.x)<(MARGE_GAUCHE+10*TAILLE_CASE))){
						posClic.line = ((event.button.y-MARGE_HAUT)/TAILLE_CASE);
						posClic.col = ((event.button.x-MARGE_GAUCHE)/TAILLE_CASE);
						continuer = 0;
					}
					//Si le clic ne se situe pas sur la grille on renvoie un SPos avec -1/-1 comme valeurs
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

//Méthode qui permet de libérer la mémoire des différentes variables utilisées et de quitter la SDL
void quitter_sdl(){
	
	int i;

	for(i=0;i<12;i++){
		SDL_FreeSurface(pionsRouges[i]);
		SDL_FreeSurface(pionsBleus[i]);
	}

	i = 0;
	for(i=0; i<11; i++){
		SDL_FreeSurface(texteBlue[i]);
		SDL_FreeSurface(texteRed[i]);
		SDL_FreeSurface(pionsBlueLeft[i]);
		SDL_FreeSurface(pionsRedLeft[i]);
	}
	
	SDL_FreeSurface(imageFond);
	SDL_Quit(); //on quitte la SDL
	
}

//Méthode qui permet d'afficher les piéces restantes sur les côtés en fonction du gamestate passé en paramètre
void afficherPiecesRestantes(SGameState gameState,SDL_Surface *ecran){

    //Déclaration des variables
    SDL_Rect positionJ1, positionJ2, positionCptRed, positionCptBlue;
    int i = 0;
    TTF_Font *police = NULL;
    SDL_Color couleurPolice = {255, 255, 255};
    int nbPiecesInitial[11];

    //Initialisation de SDL_TTF
    TTF_Init();

    //Calcul de la position de la première pièce
    positionJ1.x = MARGE_GAUCHE-TAILLE_CASE - 24;
    positionJ1.y = MARGE_HAUT;
    
    positionJ2.x = ecran->w - (MARGE_DROITE) + 30;
    positionJ2.y = MARGE_HAUT;

    //Chargement de la police
    police = TTF_OpenFont("fonts/FreeMonoBold.ttf", 20);
 
    //Initialisation du nb initial de pieces
    nbPiecesInitial[0] = 6;	//bomb
    nbPiecesInitial[1] = 1;	//spy
    nbPiecesInitial[2] = 8;	//scout
    nbPiecesInitial[3] = 5;	//miner
    nbPiecesInitial[4] = 4;	//sergeant
    nbPiecesInitial[5] = 4;	//lieutenant
    nbPiecesInitial[6] = 4;	//captain
    nbPiecesInitial[7] = 3;	//major
    nbPiecesInitial[8] = 2;	//colonel
    nbPiecesInitial[9] = 1;	//general
    nbPiecesInitial[10] = 1;//marshal  

    //Chargement des icones des pieces restantes rouges
    pionsRedLeft[0] = IMG_Load("iconsLeft/bomb0r.png");
    pionsRedLeft[1] = IMG_Load("iconsLeft/spy1r.png");
    pionsRedLeft[2] = IMG_Load("iconsLeft/scout2r.png");
    pionsRedLeft[3] = IMG_Load("iconsLeft/miner3r.png");
    pionsRedLeft[4] = IMG_Load("iconsLeft/sergeant4r.png");
    pionsRedLeft[5] = IMG_Load("iconsLeft/lieutenant5r.png");
    pionsRedLeft[6] = IMG_Load("iconsLeft/captain6r.png");
    pionsRedLeft[7] = IMG_Load("iconsLeft/major7r.png");
    pionsRedLeft[8] = IMG_Load("iconsLeft/colonel8r.png");
    pionsRedLeft[9] = IMG_Load("iconsLeft/general9r.png");
    pionsRedLeft[10] = IMG_Load("iconsLeft/marshal10r.png");

    //Chargement des icones des pieces restantes bleues
    pionsBlueLeft[0] = IMG_Load("iconsLeft/bomb0b.png");
    pionsBlueLeft[1] = IMG_Load("iconsLeft/spy1b.png");
    pionsBlueLeft[2] = IMG_Load("iconsLeft/scout2b.png");
    pionsBlueLeft[3] = IMG_Load("iconsLeft/miner3b.png");
    pionsBlueLeft[4] = IMG_Load("iconsLeft/sergeant4b.png");
    pionsBlueLeft[5] = IMG_Load("iconsLeft/lieutenant5b.png");
    pionsBlueLeft[6] = IMG_Load("iconsLeft/captain6b.png");
    pionsBlueLeft[7] = IMG_Load("iconsLeft/major7b.png");
    pionsBlueLeft[8] = IMG_Load("iconsLeft/colonel8b.png");
    pionsBlueLeft[9] = IMG_Load("iconsLeft/general9b.png");
    pionsBlueLeft[10] = IMG_Load("iconsLeft/marshal10b.png");
    
    //Boucle qui permet de placer les images de pièces ainsi que le nombre restant sur le côtés du plateau de jeu
    i = 0;
    for(i = 0; i<11; i++){
	char cptRed[10], cptBlue[10];

	positionJ1.y = MARGE_HAUT + i*TAILLE_CASE;
	positionJ2.y = MARGE_HAUT + i*TAILLE_CASE;
	
	SDL_BlitSurface(pionsRedLeft[i], NULL, ecran, &positionJ1);
	SDL_BlitSurface(pionsBlueLeft[i], NULL, ecran, &positionJ2);

	//On calcul la différence entre le nombre de pièces maximum et le nombre de pièces éliminées et on place le résultat dans une variable
	sprintf(cptBlue, "%d", (nbPiecesInitial[i]-gameState.blueOut[i]));
	sprintf(cptRed, "%d", (nbPiecesInitial[i]-gameState.redOut[i]));

	texteRed[i] = TTF_RenderText_Blended(police, cptRed, couleurPolice);
	texteBlue[i] = TTF_RenderText_Blended(police, cptBlue, couleurPolice);

	//Calcul de la position des textes indiquant le nombre de pièces restantes
	positionCptRed.x = positionJ1.x + (TAILLE_CASE-5);
        positionCptBlue.x = positionJ2.x - 5;

	positionCptRed.y = positionJ1.y + 5;
        positionCptBlue.y = positionJ2.y + 5;
	
	SDL_BlitSurface(texteRed[i], NULL, ecran, &positionCptRed);
	SDL_BlitSurface(texteBlue[i], NULL, ecran, &positionCptBlue);
    }

    TTF_CloseFont(police);
    TTF_Quit();
}	
