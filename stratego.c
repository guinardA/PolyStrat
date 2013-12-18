#include <stdio.h>
#include <dlfcn.h>
#include <stdlib.h>
#include <time.h>
#include "PolyLib.h"

int main(int argc, const char * argv[]){

//=========================Gestion ouverture de la librarie et des méthodes liés
void *j1Lib;

typedef void (*pfInitLibrary)(char[50]);
typedef void (*pfStartMatch)();
typedef void (*pfStartGame)(const EColor,EPiece[4][10]);
typedef void (*pfEndGame)();
typedef void (*pfEndMatch)();
typedef SMove (*pfNextMove)(const SGameState * const);
typedef void (*pfAttackResult)(SPos,EPiece,SPos,EPiece);
typedef void (*pfPenalty)();

pfInitLibrary j1InitLibrary;
pfStartMatch j1StartMatch;
pfStartGame j1StartGame;
pfEndGame j1EndGame;
pfEndMatch j1EndMatch;
pfNextMove j1NextMove;
pfAttackResult j1AttackResult;
pfPenalty j1Penalty;

if((j1Lib=dlopen("polylib.so",RTLD_LAZY))==NULL){
	printf("Erreur de chargement de la libraire");
	return(0);
}
if((j1InitLibrary=(pfInitLibrary)dlsym(j1Lib,"InitLibrary"))==NULL){
	printf("Erreur de chargement de 'InitLibrary'");
	return(0);
}
if((j1StartMatch=(pfStartMatch)dlsym(j1Lib,"StartMatch"))==NULL){
	printf("Erreur de chargement de 'StartMatch'");
	return(0);
}
if((j1StartGame=(pfStartGame)dlsym(j1Lib,"StartGame"))==NULL){
	printf("Erreur de chargement de 'StartGame'");
	return(0);
}
if((j1EndGame=(pfEndGame)dlsym(j1Lib,"EndGame"))==NULL){
	printf("Erreur de chargement de 'EndGame'");
	return(0);
}
if((j1EndMatch=(pfEndMatch)dlsym(j1Lib,"EndMatch"))==NULL){
	printf("Erreur de chargement de 'EndMatch'");
	return(0);
}

if((j1NextMove=(pfNextMove)dlsym(j1Lib,"NextMove"))==NULL){
	printf("Erreur de chargement de 'NextMove'");
	return(0);
}
if((j1AttackResult=(pfAttackResult)dlsym(j1Lib,"AttackResult"))==NULL){
	printf("Erreur de chargement de 'AttackResult'");
	return(0);
}
if((j1Penalty=(pfPenalty)dlsym(j1Lib,"Penalty"))==NULL){
	printf("Erreur de chargement de 'Penalty'");
	return(0);
}

//=========================Initialisation d'une partie
SGameState gameState;//Plateau du jeu a dupliquer afin éviter qu'une libraire modifie original

SMove move;
char name[50];

srand(time(NULL));
int couleur;

j1InitLibrary(name);
j1StartMatch(); //DEBUT DU MATCH, on peut avoir plusieurs parties dans un match choix du joueur

couleur = (int)rand()%2;
if(couleur == 0){ //Si c'est égal a 0 alors la librairie 1 sera rouge et la librari 2 sera bleu sinon inverse
	EPiece boardInit[4][10];
	j1StartGame(ECred,boardInit);
	//j2StartGame(ECblue,boardInit); //Lorsqu'on aura 2 joueur
} else{
	EPiece boardInit[4][10];
	j1StartGame(ECblue,boardInit);
	//j2StartGame(ECred,boardInit); //Lorsqu'on aura 2 joueur
}

dlclose(j1Lib);
return(1);
}
