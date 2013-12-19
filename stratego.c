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
int game, erreur=0;
EColor couleurJ1;

srand(time(NULL));
int couleur;

//==========ATTENTION : CODE 1 JOUEUR, MODIF EN 2 JOUEUR

j1InitLibrary(name);
j1StartMatch(); //DEBUT DU MATCH, on peut avoir plusieurs parties dans un match choix du joueur

do {

	//DIFINISSION DE LA COULEUR DU JOUEUR ET PLACEMENT DES PIONS PAR LE JOUEURS
	couleur = (int)rand()%2;
	EPiece boardInit[4][10];
	
	if(couleur == 0){ //Si c'est égal a 0 alors la librairie 1 sera rouge et la librari 2 sera bleu sinon inverse
		j1StartGame(ECred,boardInit);
		couleurJ1 = ECred;
		//j2StartGame(ECblue,boardInit); //Lorsqu'on aura 2 joueur

	} else{
		j1StartGame(ECblue,boardInit);
		couleurJ1 = ECblue;
		//j2StartGame(ECred,boardInit); //Lorsqu'on aura 2 joueur
	}

	//VERIFICATION DU NOMBRE DE JETONS
	//AJOUTER UNE BOUCLE QUI REDEMANDE AU JOUEUR DE ENTRER C'EST PION
	int Bomb=0, Spy=0, Scout=0, Miner=0, Sergeant=0, Lieutenant=0, Captain=0, Major=0, Colonel=0, General=0, Marshal=0, Flag=0;
	int i,j;
	for(i=0;i<4;i++){
		for(j=0;j<10;j++){
			if(boardInit[i][j] == EPbomb) Bomb++;
			if(boardInit[i][j] == EPspy) Spy++;
			if(boardInit[i][j] == EPscout) Scout++;
			if(boardInit[i][j] == EPminer) Miner++;
			if(boardInit[i][j] == EPsergeant) Sergeant++;
			if(boardInit[i][j] == EPlieutenant) Lieutenant++;
			if(boardInit[i][j] == EPcaptain) Captain++;
			if(boardInit[i][j] == EPmajor) Major++;
			if(boardInit[i][j] == EPcolonel) Colonel++;
			if(boardInit[i][j] == EPgeneral) General++;
			if(boardInit[i][j] == EPmarshal) Marshal++;
			if(boardInit[i][j] == EPflag) Flag++;
		}
	}

	if(Bomb != 6){
		printf("ERREUR Bomb\n");
		erreur++;
		j1Penalty();
	} else if(Spy != 1){
		printf("ERREUR Spy\n");
		erreur++;
		j1Penalty();
	} else if(Scout != 8){
		printf("ERREUR Scout\n");
		erreur++;
		j1Penalty();
	} else if(Miner != 5){
		printf("ERREUR Miner\n");
		erreur++;
		j1Penalty();
	} else if(Sergeant != 4){
		printf("ERREUR Sergeant\n");
		erreur++;
		j1Penalty();
	} else if(Lieutenant != 4){
		printf("ERREUR Lieutenant\n");
		erreur++;
		j1Penalty();
	} else if(Captain != 4){
		printf("ERREUR Captain\n");
		erreur++;
		j1Penalty();
	} else if(Major != 3){
		printf("ERREUR Major\n");
		erreur++;
		j1Penalty();
	} else if(Colonel != 2){
		printf("ERREUR Colonel\n");
		erreur++;
		j1Penalty();
	} else if(General != 1){
		printf("ERREUR General\n");
		erreur++;
		j1Penalty();
	} else if(Marshal != 1){
		printf("ERREUR Marshal\n");
		erreur++;
		j1Penalty();
	}else if(Flag != 1){
		printf("ERREUR Flag\n");
		erreur++;
		j1Penalty();
	}

	//ENREGISTRE LES PIONS DU JOUEUR1 SUR L'ETAT DE JEU COMPLET (A NE JAMAIS TRANSMETTRE A UNE IA)
	for(i=0;i<4;i++){
		for(j=0;j<10;j++){
			SBox box;
			box.content = couleurJ1;
			box.piece = boardInit[i][j];
			gameState.board[i][j] = box;
		}
	}
	
	printf("Voulez vous refaire une partie ? 1:oui, 0:non = ");
	scanf ("%i",&game);
	
}while(game==1);
	

dlclose(j1Lib);
return(1);
}
