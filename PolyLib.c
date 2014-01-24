#include "PolyLib.h"
#include <stdio.h>
#include <string.h>

int verificationMouvement(SMove move, SGameState gameState,EColor color);
void choixStrategieIA(int choix,EPiece boardInit[4][10]);
void majContextePerso(const SGameState * const gameState);
int vaEtViens(SMove move);
int doitAttaquer(int lineArmy,int colArmy,int lineEnemy,int colEnemy);
int	mouvementAleatoire(int j,int * line,int * col, int startLine, int startCol);
int mouvementSpyMiner(EPiece army,int i,int j);

void afficheConsole(SGameState gameState, EColor joueur1, EColor joueur2);

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
	int i,j;
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
	int line, col, i, j, label;
	//SBox board[10][10];
	
	do
	{
		label = 0;

		//On sélectionne la pièce a bouger
		for(i=0;i<33;i++)
		{
			line = positionPiece[i].line;
			col = positionPiece[i].col;
			

			if ( (line >=0) && (contextPerso.board[line][col].piece != EPbomb) && (contextPerso.board[line][col].piece != EPflag) )
			{ 
				
				//si notre pièce est à côté d'un adversaire
				if( (line<9) && (contextPerso.board[line+1][col].content == couleurAdverse) )
				{
					if(doitAttaquer(line,col,line+1,col) == 1 || mouvementAleatoire(-1,&(move.end.line),&(move.end.col),line,col) == 0)
					{
						move.end.line = line+1;
						move.end.col = col;
				
						label=1;
						break;
					}
				}
				else if( (line>0) && (contextPerso.board[line-1][col].content == couleurAdverse) )
				{
					if(doitAttaquer(line,col,line+1,col) == 1 || mouvementAleatoire(-1,&(move.end.line),&(move.end.col),line,col) == 0)
					{
						move.end.line = line-1;
						move.end.col = col;

						label=1;
						break;
					}
				}
				else if( (col<9) && (contextPerso.board[line][col+1].content == couleurAdverse) )
				{
					if(doitAttaquer(line,col,line+1,col) == 1 || mouvementAleatoire(-1,&(move.end.line),&(move.end.col),line,col) == 0)
					{
						move.end.line = line;
						move.end.col = col+1;
					
						label=1;
						break;
					}
				}
				else if( (col>0) && (contextPerso.board[line][col-1].content == couleurAdverse) )
				{
					if(doitAttaquer(line,col,line+1,col) == 1 || mouvementAleatoire(-1,&(move.end.line),&(move.end.col),line,col) == 0)
					{
						move.end.line = line;
						move.end.col = col-1;
					
						label=1;
						break;
					}
				}
			}
		}

		//sinon mouvement aléatoire
		if(label == 0)
		{
			do
			{
				i = (int)rand()%33;
				j = (int)rand()%20;
				line = positionPiece[i].line;
				col = positionPiece[i].col;
			}while( (contextPerso.board[line][col].piece == EPbomb) || (contextPerso.board[line][col].piece == EPflag) || (line <0));
	
			//Position d'arrivée
			if (mouvementAleatoire(j,&(move.end.line),&(move.end.col),line,col)==0)
			{
				move.end.line = -1;
				move.end.col = 0;
			}

		}

		//Position de départ
		move.start.line = line;
		move.start.col = col;
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
	/*
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
	printf("Coup origine : \n%i - %i\n%i - %i\n\n", move.start.line, move.start.col,move.end.line,move.end.col);*/
	afficheConsole(contextPerso, couleur, couleurAdverse);
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

		//maj de la pièce éliminée pour le contexte perso
		if(couleur == ECred){
			contextPerso.blueOut[attaquer]++;
		} else {
			contextPerso.redOut[attaquer]++;
		}
		
		//ON MODIFIE LE CONTEXTE DE JEU
		newBox.content = ECnone;
		newBox.piece = EPnone;
	
		//l'ia attaque
		if(attaque == 1)
		{
			//déplacement de notre pièce
			positionPiece[numPiece].line = enemyPos.line;
			positionPiece[numPiece].col = enemyPos.col;		

			//maj de déplacement pour le contexte perso
			contextPerso.board[enemyPos.line][enemyPos.col] = contextPerso.board[armyPos.line][armyPos.col];
			contextPerso.board[armyPos.line][armyPos.col] = newBox;	
		}
		//l'adversaire nous attaque
		else
		{
			//perte de notre piece
			positionPiece[numPiece].line = -1;

			//la case de l'adversaire est libérée par son déplacment
			contextPerso.board[enemyPos.line][enemyPos.col] = newBox;

			//création d'une box correspondant à l'adversaire
			newBox.content = couleurAdverse;
			newBox.piece = attaquant;

			//l'adversaire prend la place de notre pièce, on a maintenant sa valeur
			contextPerso.board[armyPos.line][armyPos.col] = newBox;	

		}
		
		
	}	
	else
	{

		//maj de la pièce éliminée pour le contexte perso
		if(couleur == ECred){
			contextPerso.blueOut[attaquant]++;
		} else {
			contextPerso.redOut[attaquant]++;
		}

		//ON MODIFIE LE CONTEXTE DE JEU
		newBox.content = ECnone;
		newBox.piece = EPnone;

		//l'ia attaque
		if(attaque == 1)
		{
			positionPiece[numPiece].line = -1;

			contextPerso.board[armyPos.line][armyPos.col] = newBox;	

			//création d'une box correspondant à l'adversaire
			newBox.content = couleurAdverse;
			newBox.piece = attaquant;

			//on a maintenant la valeur de l'adversaire
			contextPerso.board[enemyPos.line][enemyPos.col] = newBox;
		}
		//l'adversaire nous attaque
		else
		{
			// l'emplacement de l'adversaire est maintenant vide
			contextPerso.board[enemyPos.line][enemyPos.col] = newBox;
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
		printf("Va et viens IA\n");
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

		// on enregistre les pièces dans l'ordre d'importance pour la fonction nextMove
		positionPiece[0].line = 1;
		positionPiece[0].col = 2;
		positionPiece[1].line = 3;
		positionPiece[1].col = 7;
		positionPiece[2].line = 2;
		positionPiece[2].col = 0;
		positionPiece[3].line = 0;
		positionPiece[3].col = 6;
		positionPiece[4].line = 0;
		positionPiece[4].col = 5;
		positionPiece[5].line = 0;
		positionPiece[5].col = 2;
		positionPiece[6].line = 2;
		positionPiece[6].col = 6;
		positionPiece[7].line = 2;
		positionPiece[7].col = 7;
		positionPiece[8].line = 2;
		positionPiece[8].col = 3;
		positionPiece[9].line = 1;
		positionPiece[9].col = 3;
		positionPiece[10].line = 3;
		positionPiece[10].col = 8;
		positionPiece[11].line = 3;
		positionPiece[11].col = 1;
		positionPiece[12].line = 1;
		positionPiece[12].col = 6;
		positionPiece[13].line = 1;
		positionPiece[13].col = 1;
		positionPiece[14].line = 2;
		positionPiece[14].col = 9;
		positionPiece[15].line = 2;
		positionPiece[15].col = 2;
		positionPiece[16].line = 3;
		positionPiece[16].col = 0;
		positionPiece[17].line = 3;
		positionPiece[17].col = 9;
		positionPiece[18].line = 3;
		positionPiece[18].col = 6;
		positionPiece[19].line = 3;
		positionPiece[19].col = 5;
		positionPiece[20].line = 3;
		positionPiece[20].col = 4;
		positionPiece[21].line = 3;
		positionPiece[21].col = 2;
		positionPiece[22].line = 2;
		positionPiece[22].col = 1;
		positionPiece[23].line = 1;
		positionPiece[23].col = 5;
		positionPiece[24].line = 0;
		positionPiece[24].col = 1;
		positionPiece[25].line = 3;
		positionPiece[25].col = 3;
		positionPiece[26].line = 1;
		positionPiece[26].col = 9;
		positionPiece[27].line = 0;
		positionPiece[27].col = 4;
		positionPiece[28].line = 0;
		positionPiece[28].col = 0;
		positionPiece[29].line = 2;
		positionPiece[29].col = 8;
		positionPiece[30].line = 2;
		positionPiece[30].col = 5;
		positionPiece[31].line = 1;
		positionPiece[31].col = 7;
		positionPiece[32].line = 1;
		positionPiece[32].col = 4;

		positionPiece[33].line = 2;
		positionPiece[33].col = 4;
		positionPiece[34].line = 1;
		positionPiece[34].col = 8;
		positionPiece[35].line = 1;
		positionPiece[35].col = 0;
		positionPiece[36].line = 0;
		positionPiece[36].col = 9;
		positionPiece[37].line = 0;
		positionPiece[37].col = 7;
		positionPiece[38].line = 0;
		positionPiece[38].col = 3;
		positionPiece[39].line = 0;
		positionPiece[39].col = 8;

			
	}
}


//fonctions liées à nextMove

//mise à jour du contexte perso avec le gameState envoyé par l'arbitre
void majContextePerso(const SGameState * const gameState)
{
	int i,j;

	for(i=0;i<10;i++)
	{
		for(j=0;j<10;j++)
		{
			if(contextPerso.board[i][j].content != gameState->board[i][j].content)
			{
				 if(contextPerso.board[i][j].piece!=EPnone && contextPerso.board[i][j].piece!=EPscout && contextPerso.board[i][j].content==couleurAdverse)
				 {
				 	if (i<9 && contextPerso.board[i+1][j].content != gameState->board[i+1][j].content)
				 	{
				 		contextPerso.board[i+1][j] = contextPerso.board[i][j];
				 		contextPerso.board[i][j] = gameState->board[i][j];
				 	}
				 	if (j<9 && contextPerso.board[i][j+1].content != gameState->board[i][j+1].content)
				 	{
				 		contextPerso.board[i][j+1] = contextPerso.board[i][j];
				 		contextPerso.board[i][j] = gameState->board[i][j];
				 	}
				 }
				 else
				 {
				 	contextPerso.board[i][j] = gameState->board[i][j];
				 }
			}
		}
	}
}



int doitAttaquer(int lineArmy,int colArmy,int lineEnemy,int colEnemy)
{
	EPiece enemy = contextPerso.board[lineEnemy][colEnemy].piece;
	EPiece army = contextPerso.board[lineArmy][colArmy].piece;

	if (enemy==EPnone && army!=EPminer && army!=EPspy && army!=EPmarshal)
		return 1;
	else if (enemy<army && enemy!=EPbomb)
		return 1;
	else if (enemy==EPbomb && army==EPminer)
		return 1;
	else if (army==EPspy && enemy==EPmarshal)
		return 1;


	return 0;

}

int	mouvementAleatoire(int j, int * line, int * col, int startLine, int startCol)
{
	EPiece army = contextPerso.board[startLine][startCol].piece;
	

	if( (j>=0) && (j<15) )
	{
		*line = startLine+1;
		*col = startCol;

		if (mouvementSpyMiner(army,*line,*col)==1)
			return 0;

		return 1;
	}		
	else if(j < 17)
	{
		*line = startLine-1;
		*col = startCol;

		if (mouvementSpyMiner(army,*line,*col)==1)
			return 0;

		return 1;
	}		
	else if(j < 19)
	{
		*line = startLine;
		*col = startCol+1;

		if (mouvementSpyMiner(army,*line,*col)==1)
			return 0;

		return 1;
	}		
	else if(j == 20)
	{
		*line = startLine;
		*col = startCol-1;

		if (mouvementSpyMiner(army,*line,*col)==1)
			return 0;

		return 1;
	}
	else
	{
		if ( (startLine<9) && (contextPerso.board[startLine+1][startCol].content != couleurAdverse || contextPerso.board[startLine+1][startCol].piece == EPnone) )
		{
			*line = startLine+1;
			*col = startCol;
			return 1;
		}
		else if ( (startCol>0) && (contextPerso.board[startLine][startCol-1].content != couleurAdverse || contextPerso.board[startLine][startCol-1].piece == EPnone) )
		{
			*line = startLine;
			*col = startCol-1;
			return 1;
		}
		else if ( (startCol<9) && (contextPerso.board[startLine][startCol+1].content != couleurAdverse || contextPerso.board[startLine][startCol+1].piece == EPnone) )
		{
			*line = startLine;
			*col = startCol+1;
			return 1;
		}
		else if ( (startLine>0) && (contextPerso.board[startLine-1][startCol].content != couleurAdverse || contextPerso.board[startLine-1][startCol].piece == EPnone) )
		{
			*line = startLine-1;
			*col = startCol;
			return 1;
		}
	}

	return 0;
}

int mouvementSpyMiner(EPiece army,int i,int j)
{
	if (army == EPspy)
	{
		if ( i<9 && contextPerso.board[i+1][j].content == couleurAdverse)
			if (contextPerso.board[i+1][j].piece != EPmarshal)
				return 1;
		

		if ( i>0 && contextPerso.board[i-1][j].content == couleurAdverse)
			if (contextPerso.board[i-1][j].piece != EPmarshal)
				return 1;

		if ( j<9 && contextPerso.board[i][j+1].content == couleurAdverse)
			if (contextPerso.board[i][j+1].piece != EPmarshal)
				return 1;

		if ( j>0 && contextPerso.board[i][j-1].content == couleurAdverse)
			if (contextPerso.board[i][j-1].piece != EPmarshal)
				return 1;
	}
	else if (army == EPminer)
	{
		if ( i<9 && contextPerso.board[i+1][j].content == couleurAdverse)
			if (contextPerso.board[i+1][j].piece != EPbomb)
				return 1;
		

		if ( i>0 && contextPerso.board[i-1][j].content == couleurAdverse)
			if (contextPerso.board[i-1][j].piece != EPbomb)
				return 1;

		if ( j<9 && contextPerso.board[i][j+1].content == couleurAdverse)
			if (contextPerso.board[i][j+1].piece != EPbomb)
				return 1;

		if ( j>0 && contextPerso.board[i][j-1].content == couleurAdverse)
			if (contextPerso.board[i][j-1].piece != EPbomb)
				return 1;
	}

	return 0;
}

/*
 * FONCTION QUI AFFICHE DANS EN CONSOLE LE CONTEXTE DE JEU
 */
void afficheConsole(SGameState gameState, EColor joueur1, EColor joueur2){
	int i, j;
	
	printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-\nCouleur\n");
	
	//AFFICHAGE QUE DES COULEURS
	
	if(joueur1 == ECred){
		printf("Joueur 1 : rouge\nJoueur 2 : bleu\n\n");
	}
	else{
		printf("Joueur 1 : bleu\nJoueur 2 : rouge\n\n");
	}
	
	printf("R : Joueur rouge\n");
	printf("B : Joueur bleu\n");
	printf("L : Lac\n");
	printf("0 : Vide\n\n");

	printf("  |");
	for(i=0;i<10;i++){
		printf(" %i |",i);
	}
	printf("\n");
	
	for(i=0;i<10;i++){
		printf("%i |",i);
		for(j=0;j<10;j++){
			if(gameState.board[i][j].content == ECred){
				printf(" R |");
			}
			else if(gameState.board[i][j].content == ECblue){
				printf(" B |");
			}
			else if(gameState.board[i][j].content == EClake){
				printf(" L |");
			}
			else{
				printf(" O |");
			}
		}
		printf("\n");
	}
	
	//AFFICHAGE QUE DES PERSONNAGE
	printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-\nPersonnages\n");
	
	if(joueur1 == ECred){
		printf("Joueur 1 : rouge\nJoueur 2 : bleu\n\n");
	}
	else{
		printf("Joueur 1 : bleu\nJoueur 2 : rouge\n\n");
	}
	
	printf("Lettre en majuscule : Joueur rouge\n");
	printf("Lettre en miniscule : Joueur bleu\n\n");
	
	printf("bo : Bomb\n");
	printf("sp : Spy\n");
	printf("sc : Scout\n");
	printf("mi : Miner\n");
	printf("se : Sergeant\n");
	printf("li : Lieutenant\n");
	printf("ca : Captain\n");
	printf("ma : Major\n");
	printf("co : Colonel\n");
	printf("ge : General\n");
	printf("ms : Marshal\n");
	printf("fl : Flag\n");
	printf("no : None\n\n");
	
	printf("L : Lac\n\n");
	printf("0 : Vide\n\n");

	printf("  |");
	for(i=0;i<10;i++){
		printf(" %i |",i);
	}
	printf("\n");
	
	for(i=0;i<10;i++){
		printf("%i |",i);
		for(j=0;j<10;j++){
			
			if(gameState.board[i][j].content == ECblue){
				if(gameState.board[i][j].piece == EPbomb) printf(" bo|");
				if(gameState.board[i][j].piece == EPspy) printf(" sp|");
				if(gameState.board[i][j].piece == EPscout) printf(" sc|");
				if(gameState.board[i][j].piece == EPminer) printf(" mi|");
				if(gameState.board[i][j].piece == EPsergeant) printf(" se|");
				if(gameState.board[i][j].piece == EPlieutenant) printf(" li|");;
				if(gameState.board[i][j].piece == EPcaptain) printf(" ca|");
				if(gameState.board[i][j].piece == EPmajor) printf(" ma|");
				if(gameState.board[i][j].piece == EPcolonel) printf(" co|");
				if(gameState.board[i][j].piece == EPgeneral) printf(" ge|");
				if(gameState.board[i][j].piece == EPmarshal) printf(" ms|");
				if(gameState.board[i][j].piece == EPflag) printf(" fl|");
				if(gameState.board[i][j].piece == EPnone) printf(" no|");
			}
			
			else if(gameState.board[i][j].content == ECred){
				if(gameState.board[i][j].piece == EPbomb) printf(" BO|");
				if(gameState.board[i][j].piece == EPspy) printf(" SP|");
				if(gameState.board[i][j].piece == EPscout) printf(" SC|");
				if(gameState.board[i][j].piece == EPminer) printf(" MI|");
				if(gameState.board[i][j].piece == EPsergeant) printf(" SE|");
				if(gameState.board[i][j].piece == EPlieutenant) printf(" LI|");;
				if(gameState.board[i][j].piece == EPcaptain) printf(" CA|");
				if(gameState.board[i][j].piece == EPmajor) printf(" MA|");
				if(gameState.board[i][j].piece == EPcolonel) printf(" CO|");
				if(gameState.board[i][j].piece == EPgeneral) printf(" GE|");
				if(gameState.board[i][j].piece == EPmarshal) printf(" MS|");
				if(gameState.board[i][j].piece == EPflag) printf(" FL|");
				if(gameState.board[i][j].piece == EPnone) printf(" NO|");
			}
			
			else if(gameState.board[i][j].content == EClake){
				printf(" L |");
			}
			else{
				printf(" O |");
			}
		}
		printf("\n");
	}


}
