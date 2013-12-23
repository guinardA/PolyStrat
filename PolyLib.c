#include "PolyLib.h"
#include <stdio.h>
#include <string.h>

EColor couleur;
int penalite=0;

//Ensemble des fonctions communes a toutes les groupes
void InitLibrary(char name[50])
{
	printf("Initialisation des variables de la librairie\n");
	strcpy(name,"GUINARD Arnaud et MOUDEN Benjamin");
}

void StartMatch()
{
	printf("Début du match\n");
}

void StartGame(const EColor color,EPiece boardInit[4][10])
{
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
	printf("Deplacement d'un pion\n");
	SMove move; 
	move.start.line = 3;
	move.start.col = 8;
	move.end.line = 4;
	move.end.col = 8;
	return move;
}

void AttackResult(SPos armyPos,EPiece armyPiece,SPos enemyPos,EPiece enemyPiece)
{
	printf("AttackResult\n");
}

void Penalty()
{
	printf("Penalty\n");
	penalite++;
	printf("Nombre de pénalité : %i\n",penalite);
}

//Ensemble des fonctions liées a IA
