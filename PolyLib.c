#include "PolyLib.h"
#include <stdio.h>
#include <string.h>

int verificationMouvement(SMove move, SGameState gameState,EColor color);
void choixStrategieIA(int choix,EPiece boardInit[4][10]);
int peutBouger(int i, int j);
void majContextePerso(const SGameState * const gameState);

EColor couleur,couleurAdverse;
//attaque à 0 quand le mouvement réalisé précédement n'est pas une attaque, sinon 1
int penalite=0,attaque=0, nbrPionRestant = 40;
SPos positionPiece[40];
SGameState contextPerso;


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
		
	//FAIRE ATTENTION AVEC LA POSITION DE LA COULEUR	
	int i,j,k;
	SBox box;
	
	if(couleur = ECred){
		couleurAdverse = ECblue;
	}
	else{
		couleurAdverse = ECred;
	}
	
	printf("Lancement d'une partie\n");
	printf("Placement des pions sur le terrain\n");
	couleur = color;

	//Choix stratégie de départ
	srand(time(NULL));
	int choix = (int)rand()%1;
	choixStrategieIA(choix,boardInit);
	
	//Initialisation du contexte de jeu interne
	k = 0;
	for(i=0; i<4 ; i++){
		for(j=0 ; j<10 ; j++){
			box.content = couleur;
			box.piece = boardInit[i][j];
			contextPerso.board[i][j] = box;
			positionPiece[k].col = i;
			positionPiece[k].line = j;
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
		}
	}
	for(i=6; i<10 ; i++){
		for(j=0 ; j<10 ; j++){
			box.content = couleurAdverse;
			box.piece = EPnone;
			contextPerso.board[i][j] = box;
		}
	}
	
	for(i= 0; i<11 ; i++){
		contextPerso.redOut[i] = 0;
		contextPerso.blueOut[i] = 0;
	}
}

void EndGame()
{
	printf("EndGame\n");
	penalite = 0;
}

void EndMatch()
{
	printf("EndMatch\n");
}

SMove NextMove(const SGameState * const gameState)
{
	//Maj du contextePerso
	majContextePerso(gameState);

	//RÈGLE DE DÉPLACEMENT RECUPERER SUR LE STRATEGO.C
	//VÉRIFIER DANS LE TABLEAU QUE ENNEMIE EST BON (SELON STRATEGIE)
	printf("Deplacement d'un pion\n");
	
	SMove move; 
	int i;
	int j;
	
	do
	{
		srand(time(NULL));
		do
		{
			i = (int)rand()%nbrPionRestant;
			printf("%i \n", i);
			//j = (int)rand()%nbrPionRestant;
		}
		while(( (contextPerso.board[positionPiece[i].line][positionPiece[i].col].piece == EPbomb) || (contextPerso.board[positionPiece[i].line][positionPiece[i].col].piece == EPflag) )  );
		//while( (contextPerso.board[i][j].content != couleur) || (peutBouger(i,j) != 0) || ( (contextPerso.board[i][j].piece == EPbomb) || (contextPerso.board[i][j].piece == EPflag) )  );
		/*
			printf("\n\n%i %i\n\n",i,j);
			if(couleur==ECblue)
				printf("bleue");
			else
				printf("rouge");
		
		move.start.line = i;
		move.start.col = j;
		*/
		if(i<9)
		{
			move.end.line = positionPiece[i].line+1;
			move.end.col = positionPiece[i].col;
		}
		else if(j>0)
		{
			move.end.line = positionPiece[i].line;
			move.end.col = positionPiece[i].col-1;
		}
		else
		{
			move.end.line = positionPiece[i].line;
			move.end.col = positionPiece[i].col+1;
		}

		if(gameState->board[move.end.line][move.end.col].content == couleurAdverse)
			attaque = 1;
		else
			attaque = 0;
	}
	while(verificationMouvement(move, *gameState, couleur)!=0);
	
	return move;
}

void AttackResult(SPos armyPos,EPiece armyPiece,SPos enemyPos,EPiece enemyPiece)
{
	//REMPLIR TABLEAU SELON LE RESULTAT
	printf("AttackResult\n");
	EPiece attaquant;
	EPiece attaquer;
	SBox newBox;
	
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
		nbrPionRestant --;
	}
	
	
	/*
	 * CAS OU LA PIECE ATTAQUANT EST PLUS FORTE QUE LA PIÈCE ATTAQUÉE
	 * OU CAS OU LA PIECE ATTAQUANT EST UN DÉMINENEUR ET LA PIECE ATTAQUÉE UNE BOMBE
	 * OU CAS OU LA PIECE ATTAQUANT EST UNE ESPIONNE ET LA PIECE ATTAQUÉE UN MARCHAL
	 */
	if((attaquant > attaquer && attaquer!=EPbomb) || 
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
	
		contextPerso.board[enemyPos.line][enemyPos.col] = contextPerso.board[armyPos.line][armyPos.col];
		contextPerso.board[armyPos.line][armyPos.col] = newBox;		
	}	
	else
	{
		if(couleur == ECred){
			contextPerso.redOut[attaquer]++;
		} else {
			contextPerso.blueOut[attaquer]++;
		}
	
		//ON MODIFIE LE CONTEXTE DE JEU
		newBox.content = ECnone;
		newBox.piece = EPnone;

		contextPerso.board[armyPos.line][armyPos.col] = contextPerso.board[enemyPos.line][enemyPos.col];
		contextPerso.board[enemyPos.line][enemyPos.col] = newBox;
	}	

}

void Penalty()
{
	printf("Penalty\n");
	penalite++;
	printf("Nombre de pénalité : %i\n",penalite);
}

//Ensemble des fonctions liées a IA

/*
 * Fonction qui vérifie si le mouvement est possible ou pas.
 * Renvoie 0 si le mouvement est possible ou 1 si il est pas possible
 */
int verificationMouvement(SMove move, SGameState gameState,EColor color){
	
	SBox boxStart, boxEnd, newBox;
	
	if(move.start.line>=0 && move.start.line<=9 && move.start.col>=0 && move.start.col<=9){	
		
			boxStart = gameState.board[move.start.line][move.start.col];
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
								if((move.start.line == move.end.line && move.start.col != move.end.col) ||
								   (move.start.line != move.end.line && move.start.col == move.end.col)){
									   
									   int startLine = move.start.line;
									   int endLine = move.end.line;
									   int startCol = move.start.col;
									   int endCol= move.end.col;
									   
									   //ON VERIFIE DANS LE CAS D'UN ÉCLAIREUR SI IL NE SAUTE PAS PAR DESSUS UN LAC OU UN JOUEUR DURANT SONT DÉPLACEMENT
									   while((startLine == endLine && startCol != endCol) ||
												(startLine != endLine && startCol == endCol)){
										   
										   if(startCol == endCol && startLine != endLine && gameState.board[startLine+1][endCol].content !=ECnone){
												return 1;
											}
											else if(startCol == endCol && startLine != endLine && gameState.board[startLine+1][endCol].content == ECnone){
												startLine++;
											}
											 
											if (startLine == endLine && startCol != endCol && gameState.board[move.start.line][move.end.col+1].content !=ECnone){
												 return 1;
											}
											else if (startLine == endLine && startCol != endCol && gameState.board[move.start.line][move.end.col+1].content ==ECnone){
												 startCol++;
											}
										}
									return 0;
								}   
							}
							//CAS D'UNE PIÈCE QUI BOUGE ET QUI N'EST PAS UN ÉCLAIREUR
							else{
								if((move.start.line == move.end.line && move.start.col == move.end.col+1) ||
								   (move.start.line == move.end.line && move.start.col == move.end.col+1) ||
								   (move.start.line == move.end.line+1 && move.start.col == move.end.col) ||
								   (move.start.line == move.end.line-1 && move.start.col == move.end.col)){
									   
									return 0;
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

//Renvoie 0 si les coordonnées passées en paramètres correspondent à un pion de l'ia qui peut bouger
int peutBouger(int i, int j)
{
	if( (i!=9) && (contextPerso.board[i+1][j].content != EClake) && (contextPerso.board[i+1][j].content != couleur) )
		return 0;
	
	return 1;
}


//mise à jour du contexte perso avec le gameState envoyé par l'arbitre
void majContextePerso(const SGameState * const gameState)
{
	int i,j;

	for(i=0;i<10;i++)
	{
		for(j=0;i<10;i++)
		{
			if(contextPerso.board[i][j].content != gameState->board[i][j].content)
				contextPerso.board[i][j] = gameState->board[i][j];
		}
	}
}
