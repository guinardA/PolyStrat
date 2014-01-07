#include "PolyLib.h"
#include <stdio.h>
#include <string.h>

int verificationMouvement(SMove move, SGameState gameState,EColor color);

EColor couleur;
int penalite=0;
SGameState contextPerso;

//Ensemble des fonctions communes a toutes les groupes
void InitLibrary(char name[50])
{
	printf("Initialisation des variables de la librairie\n");
	strcpy(name,"Polylib");
}

void StartMatch()
{	
	printf("Début du match\n");
}

void StartGame(const EColor color,EPiece boardInit[4][10])
{
		
	//FAIRE ATTENTION AVEC LA POSITION DE LA COULEUR	
	int i,j, couleurAdverse;
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

	//A modifier avec la strategie de IA
	//BOMBE
	boardInit[0][0] = EPbomb;
	boardInit[0][1] = EPbomb;
	boardInit[0][2] = EPbomb;
	boardInit[0][3] = EPbomb;
	boardInit[0][4] = EPbomb;
	boardInit[0][5] = EPbomb;
	//SPY
	boardInit[0][6] = EPspy;
	//SCOUT
	boardInit[0][7] = EPscout;
	boardInit[0][8] = EPscout;
	boardInit[0][9] = EPscout;
	boardInit[1][0] = EPscout;
	boardInit[1][1] = EPscout;
	boardInit[1][2] = EPscout;
	boardInit[1][3] = EPscout;
	boardInit[1][4] = EPscout;
	//MINER
	boardInit[1][5] = EPminer;
	boardInit[1][6] = EPminer;
	boardInit[1][7] = EPminer;
	boardInit[1][8] = EPminer;
	boardInit[1][9] = EPminer;
	//SERGENT
	boardInit[2][0] = EPsergeant;
	boardInit[2][1] = EPsergeant;
	boardInit[2][2] = EPsergeant;
	boardInit[2][3] = EPsergeant;
	//LIEUTENANT
	boardInit[2][4] = EPlieutenant;
	boardInit[2][5] = EPlieutenant;
	boardInit[2][6] = EPlieutenant;
	boardInit[2][7] = EPlieutenant;
	//CAPITAINE
	boardInit[2][8] = EPcaptain;
	boardInit[2][9] = EPcaptain;
	boardInit[3][0] = EPcaptain;
	boardInit[3][1] = EPcaptain;
	//MAJOR
	boardInit[3][2] = EPmajor;
	boardInit[3][3] = EPmajor;
	boardInit[3][4] = EPmajor;
	//COLONEL
	boardInit[3][5] = EPcolonel;
	boardInit[3][6] = EPcolonel;
	//GENERAL
	boardInit[3][7] = EPgeneral;
	//MARSHAL
	boardInit[3][8] = EPmarshal;
	//FLAG
	boardInit[3][9] = EPflag;
	
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
}

void EndMatch()
{
	printf("EndMatch\n");
}

SMove NextMove(const SGameState * const gameState)
{
	
	//RÈGLE DE DÉPLACEMENT RECUPERER SUR LE STRATEGO.C
	//VÉRIFIER DANS LE TABLEAU QUE ENNEMIE EST BON (SELON STRATEGIE)
	printf("Deplacement d'un pion\n");
	
	SMove move; 
	move.start.line = 3;
	move.start.col = 8;
	move.end.line = 4;
	move.end.col = 8;
	
	//Vérifie le mouvement renvoie 0 mouvement ok et 1 mouvement pas ok
	verificationMouvement(move, contextPerso, couleur);
	
	return move;
}

void AttackResult(SPos armyPos,EPiece armyPiece,SPos enemyPos,EPiece enemyPiece)
{
	//REMPLIR TABLEAU SELON LE RESULTAT
	
	printf("AttackResult\n");
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
									   //ON VERIFIE DANS LE CAS D'UN ÉCLAIREUR SI IL NE SAUTE PAS PAR DESSUS UN LAC OU UN JOUEUR DURANT SONT DÉPLACEMENT
									   while(move.start.line != move.end.line || move.start.col != move.end.col){
										   if(move.start.line != move.end.line && gameState.board[move.start.line+1][move.end.col].content !=ECnone){
											  return 1;
											 }
											 else move.start.line++;
											 if (move.start.col != move.end.col && gameState.board[move.start.line][move.end.col+1].content !=ECnone){
												 return 1;
											}
											else move.start.col++;
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
