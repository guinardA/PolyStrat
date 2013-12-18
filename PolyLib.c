#include "PolyLib.h"
#include <stdio.h>
#include <string.h>

//Ensemble des fonctions communes a toutes les groupes
void InitLibrary(char name[50])
{
	printf("InitLibrary\n");
	strcpy(name,"Fabien Picarougne");
}

void StartMatch(const EColor color)
{
	printf("StartMatch\n");
}

void StartGame(EPiece boardInit[4][10])
{
	printf("StartGame\n");
}

void EndGame()
{
	printf("EndGame\n");
}

void EndMatch()
{
	printf("EndMatch\n");
}

void NextMove(const SGameState * const gameState, SMove *move)
{
	printf("NextMove\n");
}

void AttackResult(SPos armyPos,EPiece armyPiece,SPos enemyPos,EPiece enemyPiece)
{
	printf("AttackResult\n");
}

void Penalty()
{
	printf("Penalty\n");
}

//Ensemble des fonctions liées a IA
