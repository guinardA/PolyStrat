#include "PolyLib.h"
#include <stdio.h>
#include <string.h>

int verificationMouvement(SMove move, SGameState gameState,EColor color);
void choixStrategieIA(int choix,EPiece boardInit[4][10]);
void majContextePerso(const SGameState * const gameState);
int vaEtViens(SMove move);

EColor couleur,couleurAdverse;

//attaque à 0 quand le mouvement réalisé précédement n'est pas une attaque, sinon 1
int penalite=0,attaque=0;
SPos positionPiece[40];
SGameState contextPerso;
int var =0;

//Ensemble des fonctions communes a toutes les groupes
void InitLibrary(char name[50])
{
	printf("Initialisation des variables de la librairie 1\n");
	strcpy(name,"Polylib");
}

void StartMatch()
{	
	printf("Début du match\n");
}

void StartGame(const EColor color,EPiece boardInit[4][10])
{
	
	printf("Lancement d'une partie\n");
	printf("Placement des pions sur le terrain\n");
	
	//Initialisation du srand pour les coups de IA	
	srand(time(NULL));	
		
	//FAIRE ATTENTION AVEC LA POSITION DE LA COULEUR	
	int i,j,k = 0;
	SBox box;
	
	//Définition de la couleur
	couleur = color;
	if(couleur == ECred){
		couleurAdverse = ECblue;	
	}
	else{
		couleurAdverse = ECred;
	}

	//Choix stratégie de placement de pion
	//Actuellement une seul strategie
	int choix = (int)rand()%1;
	choixStrategieIA(choix,boardInit);

	//Initialisation du contexte de jeu interne
	for(i=0; i<4 ; i++){
		for(j=0 ; j<10 ; j++){
			box.content = couleur;
			box.piece = boardInit[i][j];
			contextPerso.board[i][j] = box;
			//Mise en place des pions du joueur
			positionPiece[k].line = i;
			positionPiece[k].col = j;
			k++;
		}
	}
	
	for(i=4; i<6 ; i++){
		for(j=0 ; j<10 ; j++){
			if((i==4 && j==2) || (i==4 && j==3) || (i==4 && j==6) || (i==4 && j==7) ||(i==5 && j==2) || (i==5 && j==3) || (i==5 && j==6) || (i==5 && j==7)){
				box.content = EClake;
				box.piece = EPnone;
				contextPerso.board[i][j] = box;
			} 
			else{
				box.content = ECnone;
				box.piece = EPnone;
				contextPerso.board[i][j] = box;
			}
		}
	}
	
	//Au démarrage on sait qu'il y a 4 lignes de pion adverse mais on connait pas leur force
	for(i=6; i<10 ; i++){
		for(j=0 ; j<10 ; j++){
			box.content = couleurAdverse;
			box.piece = EPnone;
			contextPerso.board[i][j] = box;
		}
	}
	
	//Nombre de pions détruit
	for(i= 0; i<11 ; i++){
		contextPerso.redOut[i] = 0;
		contextPerso.blueOut[i] = 0;
	}
	/*
	printf("Couleur du joueur : %i\n", couleur);
	for(i=0; i<10 ; i++){
		for(j=0 ; j<10 ; j++){
			printf("%i ", contextPerso.board[i][j].content);
		}
		printf("\n");
	}
	
	while(1){
	}*/
	
}

void EndGame()
{
	printf("EndGame\n");
	//On remet les pénalités a 0
	penalite = 0;
}

void EndMatch()
{
	printf("EndMatch\n");
}

SMove NextMove(const SGameState * const gameState)
{
	
	printf("Deplacement d'un pion\n");
	//Maj du contextePerso
	majContextePerso(gameState);

	SMove move; 
	int line, col, i, j;
	SBox board[10][10];
	
	do
	{
		//On sélectionne la pièce a bouger
		do
		{
			i = (int)rand()%40;
			j = (int)rand()%4;
			line = positionPiece[i].line;
			col = positionPiece[i].col;
		}while( (contextPerso.board[line][col].piece == EPbomb) || (contextPerso.board[line][col].piece == EPflag) || (line <0));

		//Position de départ
		move.start.line = line;
		move.start.col = col;
		
		if(j == 0){
			move.end.line = line+1;
			move.end.col = col;
		}
		
		else if(j == 1){
			move.end.line = line-1;
			move.end.col = col;
		}
		
		else if(j == 2){
			move.end.line = line;
			move.end.col = col+1;
		}
		
		else if(j == 3){
			move.end.line = line;
			move.end.col = col-1;
		}
	}
	while(verificationMouvement(move, *gameState, couleur)!=0);

	//Cas ou le mouvement va entrainer une attaque
	if(gameState->board[move.end.line][move.end.col].content == couleurAdverse)
	{
		attaque = 1;
	}
	//Cas ou aucune attaque va se passer
	else
	{
		attaque = 0;
		//On met a jour le contexte perso
		contextPerso.board[move.end.line][move.end.col].content = contextPerso.board[line][col].content;
		contextPerso.board[move.end.line][move.end.col].piece = contextPerso.board[line][col].piece;
		contextPerso.board[line][col].content = ECnone;
		contextPerso.board[line][col].piece = EPnone;
			
		//On a mis a jour la position du pion déplacer
		positionPiece[i].line = move.end.line;
		positionPiece[i].col = move.end.col;
	}
	
	 //Affichage du contexte perso a corriger
	for(i=0; i<10 ; i++){
		for(j=0 ; j<10 ; j++){
			printf("%i ", contextPerso.board[i][j].content);
		}
		printf("\n");
	}
	printf("\n");
	printf("\n");
	printf("\n");
	
	//Affichage de la position des pions restant selon positionPiece
	for(i=0; i<10 ; i++){
		for(j=0 ; j<10 ; j++){
			board[i][j].piece = EPnone;
			board[i][j].content = ECnone;
		}
	}
	
	printf("Couleur du joueur : %i \n\n", couleur);
	for(i=0; i<40 ; i++){
		if(positionPiece[i].line != -1){
			board[positionPiece[i].line][positionPiece[i].col].piece = EPbomb;
			board[positionPiece[i].line][positionPiece[i].col].content = couleur;
		}
	}
	
	
	for(i=0; i<10 ; i++){
		for(j=0 ; j<10 ; j++){
			printf("%i ", board[i][j].piece);
		}
		printf("\n");
	}
	printf("Couleur du joueur : %i\n", couleur);
	printf("Coup origine : \n%i - %i\n%i - %i\n\n", move.start.line, move.start.col,move.end.line,move.end.col);
	return move;
}

void AttackResult(SPos armyPos,EPiece armyPiece,SPos enemyPos,EPiece enemyPiece)
{
	//SI ATTAQUE = 1 ALORS ON EST ATTAQUANT
	
	//REMPLIR TABLEAU SELON LE RESULTAT
	printf("AttackResult IA\n");
	EPiece attaquant;
	EPiece attaquer;
	SBox newBox;
	int numPiece = 0, arret =1, i;
	SPos pos;
	
	while(arret)
	{
		if(positionPiece[numPiece].line == armyPos.line){
			if(positionPiece[numPiece].col == armyPos.col){
				arret = 0;
			}
		}
		if(arret == 1){
			numPiece = numPiece + 1;
		}
	}	

	if(attaque == 1)
	{
		attaquant = armyPiece;
		attaquer = enemyPiece;
	}
	else
	{
		attaquant = enemyPiece;
		attaquer = armyPiece;
	}
			
	//CAS OU LES 2 PIÈCES SONT DE FORCE ÉQUIVALENTE
	if(attaquant == attaquer){

		contextPerso.redOut[attaquant]++;
		contextPerso.blueOut[attaquer]++;	

		//ON MODIFIE LE CONTEXTE DE JEU
		newBox.content = ECnone;
		newBox.piece = EPnone;
		contextPerso.board[armyPos.line][armyPos.col] = newBox;
		contextPerso.board[enemyPos.line][enemyPos.col] = newBox;
		
		positionPiece[numPiece].line = -1;	
	}
	
	/*
	 * CAS OU LA PIECE ATTAQUANT EST PLUS FORTE QUE LA PIÈCE ATTAQUÉE
	 * OU CAS OU LA PIECE ATTAQUANT EST UN DÉMINENEUR ET LA PIECE ATTAQUÉE UNE BOMBE
	 * OU CAS OU LA PIECE ATTAQUANT EST UNE ESPIONNE ET LA PIECE ATTAQUÉE UN MARCHAL
	 */
	else if((attaquant > attaquer && attaquer!=EPbomb) || 
	(attaquant == EPminer && attaquer == EPbomb) ||
	(attaquant == EPspy && attaquer == EPmarshal)){

		if(couleur == ECred){
			contextPerso.blueOut[attaquer]++;
		} else {
			contextPerso.redOut[attaquer]++;
		}
		
		//ON MODIFIE LE CONTEXTE DE JEU
		newBox.content = ECnone;
		newBox.piece = EPnone;
	

		if(attaque == 1)
		{
			positionPiece[numPiece].line = enemyPos.line;
			positionPiece[numPiece].col = enemyPos.col;		

			contextPerso.board[enemyPos.line][enemyPos.col] = contextPerso.board[armyPos.line][armyPos.col];
			contextPerso.board[armyPos.line][armyPos.col] = newBox;	
		}
		
		
	}	
	else
	{
	
		//ON MODIFIE LE CONTEXTE DE JEU
		newBox.content = ECnone;
		newBox.piece = EPnone;

		if(attaque == 1)
		{
			positionPiece[numPiece].line = -1;
			contextPerso.board[armyPos.line][armyPos.col] = newBox;	
		}

	}	
	attaque = 0;
}

void Penalty()
{
	printf("Penalty\n");
	penalite++;
	printf("Nombre de pénalité : %i\n",penalite);
}

int vaEtViens(SMove move){
	
	static int nbr_mvt;
	int retour =0;
	static SPos pos[2];

	if(pos[0].line == move.start.line && pos[0].col == move.start.col && pos[1].line ==  move.end.line && pos[1].col == move.end.col && nbr_mvt%2 == 0){
		nbr_mvt ++;
		pos[0] = move.start;
		pos[1] = move.end;
	}
	else if(pos[0].line == move.end.line && pos[0].col == move.end.col && pos[1].line ==  move.start.line && pos[1].col ==  move.start.col && nbr_mvt%2 == 1){
		nbr_mvt ++;
		pos[0] = move.end;
		pos[1] = move.start;
	}
	else{
		nbr_mvt = 1;
		pos[0] = move.start;
		pos[1] = move.end;
	}
	
	if( nbr_mvt >= 4){
		retour =1;
	}
	
	return retour;
}

//Ensemble des fonctions liées a IA

/*
 * Fonction qui vérifie si le mouvement est possible ou pas.
 * Renvoie 0 si le mouvement est possible ou 1 si il est pas possible
 */
int verificationMouvement(SMove move, SGameState gameState,EColor color){
	
	
	SBox boxStart, boxEnd, newBox;
	
	if(move.start.line>=0 && move.start.line<=9 && move.start.col>=0 && move.start.col<=9){	
		
		if(vaEtViens(move) == 0){
		
			boxStart = gameState.board[move.start.line][move.start.col];
			int t;
			//VERIFICATION QUE LE PION SELECTIONNER CORRESPOND A UN PION DE LA BONNE COULEUR
			if(boxStart.content == color){
				
				//VERIFICATION QUE LE PION SELECTIONNER PEUT ETRE BOUGER
				if(boxStart.piece!=EPnone && boxStart.piece!=EPbomb && boxStart.piece!=EPflag){
					if(move.end.line>=0 && move.end.line<=9 && move.end.col>=0 && move.end.col<=9){
						
						boxEnd = gameState.board[move.end.line][move.end.col];
			
						//VERIFICATION QUE ARRIVE NE CORRESPOND PAS A UN LAC NI A UN DE CES PIONS	
						if(boxEnd.content!=color && boxEnd.content!=EClake){
							
							//ON VERIFIE QUE LA PIECE DEPLACER CORRESPOND OU PAS UN ECLAIREUR
							if(boxStart.piece == EPscout){
								
								//ON VERIFIE QU'IL NE BOUGE PAS EN DIAGONAL
								if((move.start.line == move.end.line && move.start.col != move.end.col) ||
								   (move.start.line != move.end.line && move.start.col == move.end.col)){
									   
									   int startLine = move.start.line;
									   int endLine = move.end.line;
									   int startCol = move.start.col;
									   int endCol= move.end.col;
									   
									   //ON VERIFIE DANS LE CAS D'UN ÉCLAIREUR SI IL NE SAUTE PAS PAR DESSUS UN LAC OU UN JOUEUR DURANT SONT DÉPLACEMENT
									   while((startLine == endLine && startCol != endCol) ||
												(startLine != endLine && startCol == endCol)){
										   
										   //CAS PION MONTANT
										   if(startLine < endLine || startCol < endCol){
										   
											   //Cas si le prochain coup arrive a la ligne arrivé (mouvement ligne)
											   if(startCol == endCol && startLine != endLine && startLine+1 == endLine && gameState.board[startLine+1][endCol].content == couleurAdverse) {
												   startLine++;
												}
											   
											   //Cas si le prochain mouvement n'est pas une case vide (mouvement ligne)
											   else if(startCol == endCol && startLine != endLine && gameState.board[startLine+1][endCol].content !=ECnone){
													return 1;
												}
												
												//Cas si le prochain mouvement est une case vide (mouvement ligne)
												else if(startCol == endCol && startLine != endLine && gameState.board[startLine+1][endCol].content == ECnone){
													startLine++;
												}
												 
												 //Cas si le prochain coup arrive a la ligne arrivé (mouvement col)
											   if(startLine == endLine && startCol != endCol && startCol+1 == endCol && gameState.board[startLine][startCol+1].content == couleurAdverse) {
												   startCol++;
												}
												 
												//Cas si le prochain mouvement n'est pas une case vide (mouvement colonne)
												else if (startLine == endLine && startCol != endCol && gameState.board[move.start.line][startCol+1].content != ECnone){
													 return 1;
												}
												
												//Cas si le prochain mouvement est une case vide (mouvement colonne)
												else if (startLine == endLine && startCol != endCol && gameState.board[move.start.line][startCol+1].content == ECnone){
													 startCol++;
												}
											}
											//CAS PION MONTANT
										   else if(startLine > endLine || startCol > endCol){
										   
											   //Cas si le prochain coup arrive a la ligne arrivé (mouvement ligne)
											   if(startCol == endCol && startLine != endLine && startLine-1 == endLine && gameState.board[startLine-1][endCol].content == couleurAdverse) {
												   startLine--;
												}
											   
											   //Cas si le prochain mouvement n'est pas une case vide (mouvement ligne)
											   else if(startCol == endCol && startLine != endLine && gameState.board[startLine-1][endCol].content !=ECnone){
													printf("Erreur 1 \n");
													return 1;
												}
												
												//Cas si le prochain mouvement est une case vide (mouvement ligne)
												else if(startCol == endCol && startLine != endLine && gameState.board[startLine-1][endCol].content == ECnone){
													startLine--;
												}
												 
												 //Cas si le prochain coup arrive a la ligne arrivé (mouvement col)
											   if(startLine == endLine && startCol != endCol && startCol-1 == endCol && gameState.board[startLine][startCol-1].content == couleurAdverse) {
												   startCol--;
												}
												 
												//Cas si le prochain mouvement n'est pas une case vide (mouvement colonne)
												else if (startLine == endLine && startCol != endCol && gameState.board[move.start.line][startCol-1].content != ECnone){
													 return 1;
												}
												
												//Cas si le prochain mouvement est une case vide (mouvement colonne)
												else if (startLine == endLine && startCol != endCol && gameState.board[move.start.line][startCol-1].content == ECnone){
													 startCol--;
												}
											}
										}
									return 0;
								}   
							}
							
							//CAS D'UNE PIÈCE QUI BOUGE ET QUI N'EST PAS UN ÉCLAIREUR
							else{
								//Cas ou la piece peut bouger
								if((move.start.line == move.end.line && move.start.col+1 == move.end.col) ||
								   (move.start.line == move.end.line && move.start.col-1 == move.end.col) ||
								   (move.start.line+1 == move.end.line && move.start.col == move.end.col) ||
								   (move.start.line-1 == move.end.line && move.start.col == move.end.col)){
									   
									return 0;
								}   
							}
						}
					}		
				}
			}
		}
	}
	return 1;
}

/*
 *Initialisation des pièces de l'ia
 */
void choixStrategieIA(int choix,EPiece boardInit[4][10])
{
	if(choix == 0){
		//4ème ligne
		boardInit[0][0] = EPsergeant;//4
		boardInit[0][1] = EPscout;//2
		boardInit[0][2] = EPminer;//3
		boardInit[0][3] = EPbomb;
		boardInit[0][4] = EPsergeant;//4
		boardInit[0][5] = EPminer;//3
		boardInit[0][6] = EPminer;//3
		boardInit[0][7] = EPbomb;
		boardInit[0][8] = EPflag;
		boardInit[0][9] = EPbomb;
		//3ème ligne
		boardInit[1][0] = EPbomb;
		boardInit[1][1] = EPcaptain;//6
		boardInit[1][2] = EPspy;
		boardInit[1][3] = EPmajor;//7
		boardInit[1][4] = EPlieutenant;//5
		boardInit[1][5] = EPscout;//2
		boardInit[1][6] = EPcaptain;//6
		boardInit[1][7] = EPlieutenant;//5
		boardInit[1][8] = EPbomb;
		boardInit[1][9] = EPsergeant;//4
		//2ème ligne
		boardInit[2][0] = EPminer;//3
		boardInit[2][1] = EPscout;//2
		boardInit[2][2] = EPcolonel;//8
		boardInit[2][3] = EPmajor;//7
		boardInit[2][4] = EPbomb;
		boardInit[2][5] = EPlieutenant;//5
		boardInit[2][6] = EPmarshal;//10
		boardInit[2][7] = EPmajor;//7
		boardInit[2][8] = EPlieutenant;//5
		boardInit[2][9] = EPcolonel;//8
		//1ère ligne
		boardInit[3][0] = EPgeneral;//9
		boardInit[3][1] = EPcaptain;//6
		boardInit[3][2] = EPscout;//2
		boardInit[3][3] = EPsergeant;//4
		boardInit[3][4] = EPscout;//2
		boardInit[3][5] = EPscout;//2
		boardInit[3][6] = EPscout;//2
		boardInit[3][7] = EPminer;//3
		boardInit[3][8] = EPcaptain;//6
		boardInit[3][9] = EPscout;//2
	}
}


//fonctions liées à nextMove

//mise à jour du contexte perso avec le gameState envoyé par l'arbitre
void majContextePerso(const SGameState * const gameState)
{
	int i,j;

	for(i=0;i<10;i++)
	{
		for(j=0;i<10;i++)
		{
			if(contextPerso.board[i][j].content != gameState->board[i][j].content && contextPerso.board[i][j].piece == 12)
				contextPerso.board[i][j] = gameState->board[i][j];
		}
	}
}
