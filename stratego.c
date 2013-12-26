/* GÉRER LA REGLE DES VA ET VIENS */

#include <stdio.h>
#include <dlfcn.h>
#include <stdlib.h>
#include <time.h>
#include "PolyLib.h"

void initialisationContexteJeu(SGameState *gameState);
int verificationNombrePiece(EPiece boardInit[4][10]);
void enregistrePion(EPiece boardInit[4][10], SGameState *gameState, EColor color, int joueur);
SGameState duplicationDuContexteDeJeu(SGameState gameState, EColor color, int joueur);
int verificationMouvement(SMove move, SGameState *gameState,EColor color, int joueur, void(*AttackResult1)(SPos, EPiece, SPos, EPiece), void(*AttackResult2)(SPos, EPiece, SPos, EPiece));
int attaque(SMove move, SGameState *gameState,EColor color, int joueur, void(*AttackResult1)(SPos, EPiece, SPos, EPiece), void(*AttackResult2)(SPos, EPiece, SPos, EPiece));
int finPartie(int joueur, int flag);

void afficheConsole(SGameState gameState, EColor joueur1, EColor joueur2);

int main(int argc, const char * argv[]){

//=========================Gestion ouverture de la librarie et des méthodes liés

void *j1Lib, *j2Lib;

typedef void (*pfInitLibrary)(char[50]);
typedef void (*pfStartMatch)();
typedef void (*pfStartGame)(const EColor,EPiece[4][10]);
typedef void (*pfEndGame)();
typedef void (*pfEndMatch)();
typedef SMove (*pfNextMove)(const SGameState * const);
typedef void (*pfAttackResult)(SPos,EPiece,SPos,EPiece);
typedef void (*pfPenalty)();

//Librairie 1
pfInitLibrary j1InitLibrary;
pfStartMatch j1StartMatch;
pfStartGame j1StartGame;
pfEndGame j1EndGame;
pfEndMatch j1EndMatch;
pfNextMove j1NextMove;
pfAttackResult j1AttackResult;
pfPenalty j1Penalty;

//Librairie 2
pfInitLibrary j2InitLibrary;
pfStartMatch j2StartMatch;
pfStartGame j2StartGame;
pfEndGame j2EndGame;
pfEndMatch j2EndMatch;
pfNextMove j2NextMove;
pfAttackResult j2AttackResult;
pfPenalty j2Penalty;

//Ouverture fonction librairie 1
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

//Ouverture fonction librairie 2
if((j2Lib=dlopen("polylib.so",RTLD_LAZY))==NULL){
	printf("Erreur de chargement de la libraire");
	return(0);
}
if((j2InitLibrary=(pfInitLibrary)dlsym(j2Lib,"InitLibrary"))==NULL){
	printf("Erreur de chargement de 'InitLibrary'");
	return(0);
}
if((j2StartMatch=(pfStartMatch)dlsym(j2Lib,"StartMatch"))==NULL){
	printf("Erreur de chargement de 'StartMatch'");
	return(0);
}
if((j2StartGame=(pfStartGame)dlsym(j2Lib,"StartGame"))==NULL){
	printf("Erreur de chargement de 'StartGame'");
	return(0);
}
if((j2EndGame=(pfEndGame)dlsym(j2Lib,"EndGame"))==NULL){
	printf("Erreur de chargement de 'EndGame'");
	return(0);
}
if((j2EndMatch=(pfEndMatch)dlsym(j2Lib,"EndMatch"))==NULL){
	printf("Erreur de chargement de 'EndMatch'");
	return(0);
}

if((j2NextMove=(pfNextMove)dlsym(j2Lib,"NextMove"))==NULL){
	printf("Erreur de chargement de 'NextMove'");
	return(0);
}
if((j2AttackResult=(pfAttackResult)dlsym(j2Lib,"AttackResult"))==NULL){
	printf("Erreur de chargement de 'AttackResult'");
	return(0);
}
if((j2Penalty=(pfPenalty)dlsym(j2Lib,"Penalty"))==NULL){
	printf("Erreur de chargement de 'Penalty'");
	return(0);
}

//=========================Initialisation d'une partie
SGameState gameState;//Plateau du jeu a dupliquer afin éviter qu'une libraire modifie original
SGameState gameStateJ1,gameStateJ2;//Plateau du jeu pour le joueur 1 et 2
SMove move;
char nameJ1[50],  nameJ2[50];
int game, fin = 0;
EColor couleurJ1, couleurJ2;
EPiece boardInitJ1[4][10], boardInitJ2[4][10];

srand(time(NULL));
int couleur, pion_erreur_j1, pion_erreur_j2;

//==========ATTENTION : CODE 1 JOUEUR, MODIF EN 2 JOUEUR

j1InitLibrary(nameJ1);
j2InitLibrary(nameJ2);
j1StartMatch(); //DEBUT DU MATCH, on peut avoir plusieurs parties dans un match choix du joueur
j2StartMatch();

do {
	
	pion_erreur_j1 =1;
	pion_erreur_j2 =0;
	
	//INITIALISATION DU CONTEXTE DE JEU
	initialisationContexteJeu(&gameState);

	//DIFINISSION DE LA COULEUR DU JOUEUR ET PLACEMENT DES PIONS PAR LE JOUEURS
	couleur = (int)rand()%2;

	//ENREGISTREMENT ET VERIFICATION DES PIONS DES JOUEURS
	do{
		if(couleur == 0){ //Si c'est égal a 0 alors la librairie 1 sera rouge et la librarie 2 sera bleu sinon inverse
			if(pion_erreur_j1 !=0){
				j1StartGame(ECred,boardInitJ1);
				couleurJ1 = ECred;
				pion_erreur_j1 =  verificationNombrePiece(boardInitJ1);
			}
			if(pion_erreur_j2 !=0){
				j2StartGame(ECblue,boardInitJ2);
				couleurJ2 = ECblue;
				pion_erreur_j2 =  verificationNombrePiece(boardInitJ2);
			}
		} else{
			if(pion_erreur_j1 !=0){
				j1StartGame(ECblue,boardInitJ1);
				couleurJ1 = ECblue;
				pion_erreur_j1 =  verificationNombrePiece(boardInitJ1);
			}
			if(pion_erreur_j2 !=0){
				j2StartGame(ECred,boardInitJ2);
				couleurJ2 = ECred;
				pion_erreur_j2 =  verificationNombrePiece(boardInitJ2);
			}
		}
		
		if(pion_erreur_j1 == 1){
			fin = finPartie(1,0);
			j1Penalty();
			if(fin == 1 ){
				printf("3 erreurs pour %s donc %s gagne le match\n",nameJ1, nameJ2);
				j1EndGame();
				j2EndGame();
			}
		}
		if(pion_erreur_j2 == 1){
			fin = finPartie(2,0);
			j2Penalty();
			if(fin == 1 ){
				printf("3 erreurs pour %s donc %s gagne le match",nameJ2, nameJ1);
				j1EndGame();
				j2EndGame();
			}
		}
	}while((pion_erreur_j1 == 1 || pion_erreur_j2 == 1) && fin == 0 );
	
	if(fin == 0){
		//do{ //A METTRE LORSQU'ON AURA UNE VRAI PARTIE
			//ENREGISTRE LES PIONS DU JOUEUR 1 SUR LE CONTEXTE DE JEU
			enregistrePion(boardInitJ1, &gameState, couleurJ1, 1);
			//ENREGISTRE LES PIONS DU JOUEUR 2 SUR LE CONTEXTE DE JEU
			enregistrePion(boardInitJ2, &gameState, couleurJ2, 2);
				
			//DEMANDE DE DEPLACEMENT D'UN PION
			if(couleurJ1 == ECred){
				do{
					//COPIE DU CONTEXTE DE JEU QU'AVEC LES PIONS DU JOUEUR1
					gameStateJ1 = duplicationDuContexteDeJeu(gameState, couleurJ2, 1);
					move = j1NextMove(&gameStateJ1);
					//VERIFIE QUE LE MOUVEMENT EST VALIDE
					pion_erreur_j1 = verificationMouvement(move, &gameState, couleurJ1, 1, j1AttackResult, j2AttackResult);
					
					if(pion_erreur_j1 == 1){
						fin = finPartie(1,0);
						j1Penalty();
						printf("Mouvement non valide\n");
						if(fin == 1 ){
							printf("3 erreurs pour %s donc %s gagne le match\n",nameJ1, nameJ2);
							j1EndGame();
							j2EndGame();
						}
					}
					if(pion_erreur_j1 == 2){
						fin = 1;
						printf("%s a détruit le drapeau de %s, victoire de %s\n",nameJ1, nameJ2, nameJ1);
							j1EndGame();
							j2EndGame();
					}
					
				}while(pion_erreur_j1 == 1 && fin == 0);
				
				if(fin == 0){
					do{
						//COPIE DU CONTEXTE DE JEU QU'AVEC LES PIONS DU JOUEUR2
						gameStateJ2 = duplicationDuContexteDeJeu(gameState, couleurJ1, 2);
						move = j2NextMove(&gameStateJ2);
						
						//VERIFIE QUE LE MOUVEMENT EST VALIDE
						pion_erreur_j2 = verificationMouvement(move, &gameState, couleurJ2, 2, j1AttackResult, j2AttackResult);
						
						if(pion_erreur_j2 == 1){
							fin = finPartie(2,0);
							j2Penalty();
							printf("Mouvement non valide\n");
							if(fin == 1 ){
								printf("3 erreurs pour %s donc %s gagne le match\n",nameJ2, nameJ1);
								j1EndGame();
								j2EndGame();
							}
						}
						if(pion_erreur_j1 == 2){
							fin = 1;
							printf("%s a détruit le drapeau de %s, victoire de %s\n",nameJ2, nameJ1, nameJ2);
								j1EndGame();
								j2EndGame();
						}
					}while(pion_erreur_j2 == 1 && fin == 0);
				}
			}
			else{
				do{
					//COPIE DU CONTEXTE DE JEU QU'AVEC LES PIONS DU JOUEUR2
					gameStateJ2 = duplicationDuContexteDeJeu(gameState, couleurJ1, 2);
					move = j2NextMove(&gameStateJ2);
					
					//VERIFIE QUE LE MOUVEMENT EST VALIDE
					pion_erreur_j2 = verificationMouvement(move, &gameState, couleurJ2, 2, j1AttackResult, j2AttackResult);
					
					if(pion_erreur_j2 == 1){
						fin = finPartie(2,0);
						j2Penalty();
						if(fin == 1 ){
							printf("3 erreurs pour %s donc %s gagne le match\n",nameJ2, nameJ1);
							j1EndGame();
							j2EndGame();
						}
					}
					if(pion_erreur_j1 == 2){
						fin = 1;
						printf("%s a détruit le drapeau de %s, victoire de %s\n",nameJ2, nameJ1, nameJ2);
							j1EndGame();
							j2EndGame();
					}
						
				}while(pion_erreur_j2 == 1 && fin == 0);
				
				if(fin == 0){
					do{
						//COPIE DU CONTEXTE DE JEU QU'AVEC LES PIONS DU JOUEUR1
						gameStateJ1 = duplicationDuContexteDeJeu(gameState, couleurJ2, 1);
						move = j1NextMove(&gameStateJ1);
						
						//VERIFIE QUE LE MOUVEMENT EST VALIDE
						pion_erreur_j1 = verificationMouvement(move, &gameState, couleurJ1, 1, j1AttackResult, j2AttackResult);
						
						if(pion_erreur_j1 == 1){
							fin = finPartie(1,0);
							j1Penalty();
							printf("Mouvement non valide\n");
							if(fin == 1 ){
								printf("3 erreurs pour %s donc %s gagne le match\n",nameJ1, nameJ2);
								j1EndGame();
								j2EndGame();
							}
						}
						if(pion_erreur_j1 == 2){
							fin = 1;
							printf("%s a détruit le drapeau de %s, victoire de %s\n",nameJ1, nameJ2, nameJ1);
							j1EndGame();
							j2EndGame();
						}
					}while(pion_erreur_j1 == 1 && fin == 0);
				}
			}
		//}while(fin == 0);
	}
							
	printf("Voulez vous refaire une partie ? 1:oui, 0:non = ");
	scanf ("%i",&game);
	
	/*
	printf("\n=========================================================\nPLATEAU DE JEU\n");
	afficheConsole(gameState, couleurJ1, couleurJ2);
	printf("\n=========================================================\n");
	*/
	
}while(game==1);
	
j1EndMatch();
j2EndMatch();

dlclose(j1Lib);
dlclose(j2Lib);
return(1);
}

/*
 * Fonction qui initialise le contexte général du jeu, en implantant tous les case vide possible ainsi que les lacs
 */
void initialisationContexteJeu(SGameState *gameState){
	
	SBox box;
	int i,j;
	
	for(i=0;i<10;i++){
		for(j=0;j<10;j++){
			
			//PLACEMENT DES LACS
			if((i==4 && j==2) || (i==4 && j==3) || (i==4 && j==6) || (i==4 && j==7) ||(i==5 && j==2) || (i==5 && j==3) || (i==5 && j==6) || (i==5 && j==7)){
				box.content = EClake;
				box.piece = EPnone;
				gameState->board[i][j] = box;
			} else{
				box.content = ECnone;
				box.piece = EPnone;
				gameState->board[i][j] = box;
			}
		}
	}
	for(i=0;i<11;i++){
		gameState->redOut[i] = 0;
		gameState->blueOut[i] = 0;
	}
}

/*
 * Fonction qui vérifie que les pions placé par le joueur est correcte,
 * pour cela on vérifie que tous les types de pions sont présents et qu'il y a le bon nombre de chaque type
 */
int verificationNombrePiece(EPiece boardInit[4][10]){
	
	int Bomb=0, Spy=0, Scout=0, Miner=0, Sergeant=0, Lieutenant=0, Captain=0, Major=0, Colonel=0, General=0, Marshal=0, Flag=0, i, j;
		
	//VERIFICATION DU NOMBRE DE JETONS
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
		return 1;
	} else if(Spy != 1){
		printf("ERREUR Spy\n");
		return 1;
	} else if(Scout != 8){
		printf("ERREUR Scout\n");
		return 1;
	} else if(Miner != 5){
		printf("ERREUR Miner\n");
		return 1;
	} else if(Sergeant != 4){
		printf("ERREUR Sergeant\n");
		return 1;
	} else if(Lieutenant != 4){
		printf("ERREUR Lieutenant\n");
		return 1;
	} else if(Captain != 4){
		printf("ERREUR Captain\n");
		return 1;
	} else if(Major != 3){
		printf("ERREUR Major\n");
		return 1;
	} else if(Colonel != 2){
		printf("ERREUR Colonel\n");
		return 1;
	} else if(General != 1){
		printf("ERREUR General\n");
		return 1;
	} else if(Marshal != 1){
		printf("ERREUR Marshal\n");
		return 1;
	}else if(Flag != 1){
		printf("ERREUR Flag\n");
		return 1;
	}
	return 0;
}

/*
 * Fonction qui enregistre les pions placé par le joueur dans le contexte général du jeu
 * Joueur 1 : pion en bas du contexte
 * Joueur 2 : pion en haut du contexte
 */
void enregistrePion(EPiece boardInit[4][10], SGameState *gameState, EColor color, int joueur){
	
	SBox box;
	int i,j,k=9;
	
	for(i=0;i<4;i++){
		for(j=0;j<10;j++){
			box.content = color;
			box.piece = boardInit[i][j];
			if(joueur == 1){
				gameState->board[i][j] = box;
			}
			else{
				gameState->board[k][j] = box;
			}
		}
		if(joueur == 2){
				k--;
		}
	}
}

/*
 * FONCTION QUI RECOPIE LE CONTEXTE DU JEU COMPLET EN MONTRANT LES POSITIONS DE TOUS LES PIONS MAIS NE MONTRE PAS LES NIVEAUX DE PIONS ADVERSE
 */
SGameState duplicationDuContexteDeJeu(SGameState gameState, EColor color, int joueur){
	
	SGameState gameStatePerso;
	SBox box;
	int i,j, m = 9;
	
	for(i=0;i<10;i++){
		for(j=0;j<10;j++){
			if(gameState.board[i][j].content == color){
				box.content = color;
				box.piece = EPnone;
				if(joueur == 1){
					gameStatePerso.board[i][j] = box;
				}
				else{
					gameStatePerso.board[m][j] = box;
				}
			}
			else{
				if(joueur == 1){
					gameStatePerso.board[i][j] = gameState.board[i][j];
				}
				else {
					gameStatePerso.board[m][j] = gameState.board[i][j];
				}
			}
		}
		if(joueur == 2){
			m --;
		}
	}
	
	for(i=0;i<11;i++){
		gameStatePerso.redOut[i] = gameState.redOut[i];
		gameStatePerso.blueOut[i] = gameState.blueOut[i];
	}
	
	return gameStatePerso;
}

/*
 * FONCTION QUI VERIFIE QUE LE MOUVEMENT DU JOUEUR EST RÉALISABLE
 * SI OUI ON VÉRIFIE ENSUITE SI IL Y A UN CONFLITS OU PAS
 * SI NON ON ENVOIE UNE ERREUR AU JOUEUR
 */
int verificationMouvement(SMove move, SGameState *gameState,EColor color, int joueur, void(*AttackResult1)(SPos, EPiece, SPos, EPiece), void(*AttackResult2)(SPos, EPiece, SPos, EPiece)){
	
	SBox boxStart, boxEnd, newBox;
	
	if(move.start.line>=0 && move.start.line<=9 && move.start.col>=0 && move.start.col<=9){	
		
			if(joueur == 2){
				move.start.line = 9-move.start.line;
				move.end.line = 9-move.end.line;
			}
			boxStart = gameState->board[move.start.line][move.start.col];
			
			//VERIFICATION QUE LE PION SELECTIONNER CORRESPOND A UN PION DE LA BONNE COULEUR
			if(boxStart.content == color){
				//VERIFICATION QUE LE PION SELECTIONNER PEUT ETRE BOUGER
				if(boxStart.piece!=EPnone && boxStart.piece!=EPbomb && boxStart.piece!=EPflag){
					if(move.end.line>=0 && move.end.line<=9 && move.end.col>=0 && move.end.col<=9){
						
						boxEnd = gameState->board[move.end.line][move.end.col];

						//VERIFICATION QUE ARRIVE NE CORRESPOND PAS A UN LAC NI A UN DE CES PIONS
						if(boxEnd.content!=color && boxEnd.content!=EClake){
							//ON VERIFIE QUE LA PIECE DEPLACER CORRESPOND OU PAS UN ECLAIREUR
							if(boxStart.piece == EPscout){
								if((move.start.line == move.end.line && move.start.col != move.end.col) ||
								   (move.start.line != move.end.line && move.start.col == move.end.col)){
									   
									   //ON VERIFIE DANS LE CAS D'UN ÉCLAIREUR SI IL NE SAUTE PAS PAR DESSUS UN LAC OU UN JOUEUR DURANT SONT DÉPLACEMENT
									   while(move.start.line != move.end.line || move.start.col != move.end.col){
										   if(move.start.line != move.end.line && gameState->board[move.start.line+1][move.end.col].content !=ECnone){
											  return 1;
											 }
											 else move.start.line++;
											 if (move.start.col != move.end.col && gameState->board[move.start.line][move.end.col+1].content !=ECnone){
												 return 1;
											}
											else move.start.col++;
										}
									   
									printf("Déplacement réalisé\n");
									
									//ON VÉRIFIE SI LE DÉPLACEMENT ENTRAINE UNE ATTAQUE OU PAS
									if(boxEnd.content!=ECnone){
										return attaque(move, gameState,color, joueur, AttackResult1, AttackResult2);
									}
									else{
										//ON MODIFIE LE CONTEXTE DE JEU AVEC LE DÉPLACEMENT (CAS JOUEUR 1)
										gameState->board[move.end.line][move.end.col] = gameState->board[move.start.line][move.start.col];
										newBox.content = ECnone;
										newBox.piece = EPnone;
										gameState->board[move.start.line][move.start.col] = newBox;
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
									   
									printf("Déplacement réalisé\n");
									//ON VÉRIFIE SI LE DÉPLACEMENT ENTRAIN UNE ATTAQUE OU PAS
									if(boxEnd.content!=ECnone){
										return attaque(move, gameState,color, joueur, AttackResult1, AttackResult2);
									}
									else{
										gameState->board[move.end.line][move.end.col] = gameState->board[move.start.line][move.start.col];
										newBox.content = ECnone;
										newBox.piece = EPnone;
										gameState->board[move.start.line][move.start.col] = newBox;
									}
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
 * FONCTION QUI ENVOIE AU JOUEUR QU'IL Y A UN COMBAT
 * VÉRIFIE QU'ELLE EST LE RÉSULTAT DE L'ATTAQUE ET MODIFIE LE CONTEXTE DE JEU SELON LE TYPE DE COMBAT
 * RENVOIE 2 LORSQUE LE DRAPEAU ÉNNEMIE A ÉTÉ CAPTURÉ LORS DU ATTAQUE
 */
int attaque(SMove move, SGameState *gameState,EColor color, int joueur, void(*AttackResultJ1)(SPos, EPiece, SPos, EPiece), void(*AttackResultJ2)(SPos, EPiece, SPos, EPiece)){
	SBox attaquant, attaquer, newBox;
	
	attaquant = gameState->board[move.start.line][move.start.col];
	attaquer = gameState->board[move.end.line][move.end.col];
	
	//ON ENVOIE UN MESSAGE A TRAVERS LA FONCTION AttackResult aux joueurs
	if(joueur == 1){
		(*AttackResultJ1)(move.start, attaquant.piece, move.end, attaquer.piece);
		(*AttackResultJ2)(move.end, attaquer.piece, move.end, attaquer.piece);
	} else
	{
		(*AttackResultJ2)(move.start, attaquant.piece, move.end, attaquer.piece);
		(*AttackResultJ1)(move.end, attaquer.piece, move.end, attaquer.piece);
	}
	
	//CAS OU LES 2 PIÈCES SONT DE FORCE ÉQUIVALENTE
	if(attaquant.piece == attaquer.piece){
		
		if(attaquant.content == ECred){
			gameState->redOut[attaquant.piece]++;
			gameState->blueOut[attaquer.piece]++;
		} else {
			gameState->redOut[attaquer.piece]++;
			gameState->blueOut[attaquant.piece]++;
		}
		
		//ON MODIFIE LE CONTEXTE DE JEU
		newBox.content = ECnone;
		newBox.piece = EPnone;
		gameState->board[move.start.line][move.start.col] = newBox;
		gameState->board[move.end.line][move.end.col] = newBox;
		return 0;
	}
	
	/*
	 * CAS OU LA PIECE ATTAQUANT EST PLUS FORTE QUE LA PIÈCE ATTAQUÉE
	 * OU CAS OU LA PIECE ATTAQUANT EST UN DÉMINENEUR ET LA PIECE ATTAQUÉE UNE BOMBE
	 * OU CAS OU LA PIECE ATTAQUANT EST UNE ESPIONNE ET LA PIECE ATTAQUÉE UN MARCHAL
	 */
	if((attaquant.piece > attaquer.piece && attaquer.piece !=EPbomb) || 
	(attaquant.piece == EPminer && attaquer.piece == EPbomb) ||
	(attaquant.piece == EPspy && attaquer.piece == EPmarshal)){
		
		if(attaquant.content == ECred){
			gameState->blueOut[attaquer.piece]++;
		} else {
			gameState->redOut[attaquer.piece]++;
		}
		
		//ON MODIFIE LE CONTEXTE DE JEU
		newBox.content = ECnone;
		newBox.piece = EPnone;
		gameState->board[move.end.line][move.end.col] = gameState->board[move.start.line][move.start.col];
		gameState->board[move.start.line][move.start.col] = newBox;
		
		return 0;
		
	}
	
	//CAS OU LE DRAPEAU EST ATTAQUÉ PAR L'ENNEMIE
	if(attaquer.piece == EPflag){
		return 2;
	}
	
	//CAS OU L'ATTAQUANT A PERDU
	else{
		
		if(attaquant.content == ECred){
			gameState->redOut[attaquant.piece]++;
		} else {
			gameState->blueOut[attaquant.piece]++;
		}
		
		//ON MODIFIE LE CONTEXTE DE JEU
		newBox.content = ECnone;
		newBox.piece = EPnone;
		gameState->board[move.start.line][move.start.col] = newBox;
		return 0;
	}
}

/*
 * FONCTION QUI ENREGISTRE LES ERREURS RÉALISÉES PAR LES JOUEURS ET VÉRIFIE QUE LA PARTIE EST FINIE OU PAS
 * RENVOIE 1 LORSQUE LA PARTIE EST FINIE
 */
int finPartie(int joueur, int flag){
	static int erreur_j1=0, erreur_j2 =0;
	if(joueur == 1 && flag != 1){
		erreur_j1++;
	}
	if(joueur == 2 && flag != 1){
		erreur_j2++;
	}
	
	if(flag == 1){
		return 1;
	}
	if(erreur_j1 == 3 || erreur_j2 == 3){
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
