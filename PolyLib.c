#include "PolyLib.h"
#include <stdio.h>
#include <string.h>

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
