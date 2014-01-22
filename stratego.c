
/* COMPTER LE NOMBRE DE PARTIE GAGNER ET PERDUE
 * EU UN MOUVEMENT NON VALIDE QUI EST PASSÉ DE IA
 * ERREUR ON NE COMPTE PAS LE COUPS
 * FAIRE TXT AFFICHE TOUS LES COUPS
 * règle dans IA
 * Méthode joueur a la main
 * 		Afficher ok au message erreur et grisé la croix
 *  	placePion // Problème lorsque erreur placement affiche pas message erreur et puis gamestate pas effacé
 * 		afficheErreur
 * 		Lors d'un combat affiché la piece attaqué avec un delay
 * */

#include <stdio.h>
#include <dlfcn.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <X11/Xlib.h>
#include <time.h>
#include "PolyLib.h"
#include "IGStratego.h"

void initialisationContexteJeu(SGameState *gameState);
int verificationNombrePiece(EPiece boardInit[4][10]);
int verifPionsBouger(SGameState gameState);
int vaEtViensJ1(SMove move);
int vaEtViensJ2(SMove move);
void enregistrePion(EPiece boardInit[4][10], SGameState *gameState, EColor color, int joueur);
SGameState duplicationDuContexteDeJeu(SGameState gameState, EColor color, int joueur);
SGameState duplicationJoueur(SGameState gameState, EColor color, int joueur);
int verificationMouvement(SMove move, SGameState *gameState,EColor color, int joueur, void(*AttackResult1)(SPos, EPiece, SPos, EPiece), void(*AttackResult2)(SPos, EPiece, SPos, EPiece), int nb_IA);
void placePion(EColor color, EPiece boardInit[4][10]);
int attaque(SMove move, SGameState *gameState,EColor color, int joueur, void(*AttackResultJ1)(SPos, EPiece, SPos, EPiece), void(*AttackResultJ2)(SPos, EPiece, SPos, EPiece), int nbr_IA);
int finPartie(int joueur, int flag);
static void * quitter(void * p_data);

void afficheConsole(SGameState gameState, EColor joueur1, EColor joueur2);

char * message;
void *j1Lib, *j2Lib;

int main(int argc, const char * argv[]){

int nbr_coup_permis = atoi(argv[1]);
int nbr_IA = atoi(argv[2]);

if(nbr_coup_permis != 0 && nbr_IA > -1 && nbr_IA < 3){
	//=========================Gestion ouverture de la librarie et des méthodes liés

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
	if(nbr_IA >0){
		if((j1Lib=dlopen(argv[3],RTLD_LAZY))==NULL){
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
	}

	if(nbr_IA >1){
		//Ouverture fonction librairie 2
		if((j2Lib=dlopen(argv[4],RTLD_LAZY))==NULL){
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
	}

	//=========================Initialisation d'une partie
	SGameState gameState;//Plateau du jeu a dupliquer afin éviter qu'une libraire modifie original
	SGameState gameStateJ1,gameStateJ2;//Plateau du jeu pour le joueur 1 et 2
	SMove move;
	char nameJ1[50],  nameJ2[50];
	int game, fin ,couleur, pion_erreur_j1, pion_erreur_j2, nbr_coup_j1, nbr_coup_j2, nbr_pion_rouge=0, nbr_pion_bleu=0;
	EColor couleurJ1, couleurJ2;
	EPiece boardInitJ1[4][10], boardInitJ2[4][10];
	pthread_t thread_quitter;
	srand(time(NULL));

	//==========ATTENTION : CODE 1 JOUEUR, MODIF EN 2 JOUEUR

	//Initialisation des librarie
	//Début du match avec possibilité de plusieurs parties
	if(nbr_IA >0){
		j1InitLibrary(nameJ1);
		j1StartMatch();
	}
	if(nbr_IA >1){
		j2InitLibrary(nameJ2);
		j2StartMatch();
	}
	
	XInitThreads();
	pthread_create (&thread_quitter, NULL, quitter, NULL);

	do {
		
		fin =0;
		nbr_coup_j1 = 1;
		nbr_coup_j2 = 1;
		
		//On initialise a 1 car aucune ereurs commise actuellement
		pion_erreur_j1 = 1;
		pion_erreur_j2 = 1;
		
		//On initialise le nombre de faute a 0
		finPartie(0,0);
		
		//INITIALISATION DU CONTEXTE DE JEU
		initialisationContexteJeu(&gameState);

		//DIFINISSION DE LA COULEUR DU JOUEUR ET PLACEMENT DES PIONS PAR LE JOUEURS
		couleur = (int)rand()%2;
		
//ENREGISTREMENT ET VERIFICATION DES PIONS DES JOUEURS
		do{
			if(couleur == 0){ //Si c'est égal a 0 alors la librairie 1 sera rouge et la librarie 2 sera bleu sinon inverse
				//Joueur 1 = rouge
				if(pion_erreur_j1 !=0){
					//if(nbr_IA == 2){
						j1StartGame(ECred,boardInitJ1);
						couleurJ1 = ECred;
					/*}
					else{
						placePion(ECred, boardInitJ1);
						couleurJ1 = ECred;		
					}*/
					pion_erreur_j1 =  verificationNombrePiece(boardInitJ1);
				}
				
				//Joueur 2 = Bleu
				if(pion_erreur_j2 !=0){
					if(nbr_IA == 1){
						j1StartGame(ECblue,boardInitJ2);
						couleurJ2 = ECblue;
					}
					else if(nbr_IA == 2){
						j2StartGame(ECblue,boardInitJ2);
						couleurJ2 = ECblue;
					}
					else{
						placePion(ECblue, boardInitJ2);
						couleurJ2 = ECblue;		
					}
					pion_erreur_j2 =  verificationNombrePiece(boardInitJ2);
				}
			} 
			
			else{
				//Joueur 1 = Bleu
				if(pion_erreur_j1 !=0){
					if(nbr_IA == 2){
						j1StartGame(ECblue,boardInitJ1);
						couleurJ1 = ECblue;
					}
					else{
						placePion(ECblue, boardInitJ1);
						printf("\ntoto 1");
						couleurJ1 = ECblue;		
					}
					pion_erreur_j1 =  verificationNombrePiece(boardInitJ1);
				}
				//Joueur 2 = rouge
				if(pion_erreur_j2 == 1){
					if(nbr_IA == 1){
						j1StartGame(ECred,boardInitJ2);
						couleurJ2 = ECred;
					}
					else if(nbr_IA == 2){
						j2StartGame(ECred,boardInitJ2);
						couleurJ2 = ECred;
					}
					else{
						placePion(ECred, boardInitJ2);
						couleurJ2 = ECred;		
					}
					pion_erreur_j2 =  verificationNombrePiece(boardInitJ2);
				}
			}
			
			if(pion_erreur_j1 == 1){
				if(nbr_IA == 2){
					fin = finPartie(1,0);
					j1Penalty();
				}
				if(fin == 1 ){
					printf("3 erreurs pour %s donc %s gagne le match\n",nameJ1, nameJ2);
					if(nbr_IA == 1){
						j1EndGame();
					}
					if(nbr_IA == 2){
						j2EndGame();
					}
				}
			}
			if(pion_erreur_j2 == 1){
				if(nbr_IA == 1){
					fin = finPartie(2,0);
					j1Penalty();
				}
				else if(nbr_IA == 2){
					fin = finPartie(2,0);
					j2Penalty();
				}
				if(fin == 1 ){
					printf("3 erreurs pour %s donc %s gagne le match",nameJ2, nameJ1);
					if(nbr_IA == 1){
						j1EndGame();
					}
					if(nbr_IA == 2){
						j2EndGame();
					}
				}
			}
			
		}while((pion_erreur_j1 == 1 || pion_erreur_j2 == 1) && fin == 0 );
		//On continue la partie si il n'y a pas eu 3 erreurs lors du placement des pions
		if(fin == 0){
			//ENREGISTRE LES PIONS DU JOUEUR 1 SUR LE CONTEXTE DE JEU
			enregistrePion(boardInitJ1, &gameState, couleurJ1, 1);
			
			//ENREGISTRE LES PIONS DU JOUEUR 2 SUR LE CONTEXTE DE JEU
			enregistrePion(boardInitJ2, &gameState, couleurJ2, 2);
			
			do{
				
//DEPLACEMENT D'UN PION
				//Si joueur 1 est rouge alors il commence
				if(couleurJ1 == ECred){
					do{
						
						if(nbr_IA == 2){
							//COPIE DU CONTEXTE DE JEU QU'AVEC LES PIONS DU JOUEUR1
							gameStateJ1 = duplicationDuContexteDeJeu(gameState, couleurJ2, 1);
							move = j1NextMove(&gameStateJ1);
							pion_erreur_j1 = verificationMouvement(move, &gameState, couleurJ1, 1, j1AttackResult, j2AttackResult, 2);
							interfaceGraphique(gameState); //On affiche le plateau de jeu après initialisation
						}
						else{
							gameStateJ1 = duplicationJoueur(gameState, couleurJ2, 1);
							interfaceGraphique(gameStateJ1);
							move = renvoieCoordonnees(); //A METTRE QUE LORSQUE C'EST UN JOUEUR HUMAIN 
							move.start.line = 9-move.start.line;
							move.end.line = 9-move.end.line;	
							
							pion_erreur_j1 = verificationMouvement(move, &gameState, couleurJ1, 1, j1AttackResult, j2AttackResult, 0);
							gameStateJ1 = duplicationJoueur(gameState, couleurJ2, 1);
							interfaceGraphique(gameStateJ1);
						}
						
						/*
						clock_t start,end;
						printf("titi\n");
						start=clock();
						printf("toto\n");
						while(((end=clock())-start)<=2*CLOCKS_PER_SEC);
						printf("tata\n");*/
						
						//On vérifie qu'il y a encore un pion a bouger
						fin = verifPionsBouger(gameState);
							
						if(pion_erreur_j1 == 1){
							if(nbr_IA == 2){
								fin = finPartie(1,0);
								j1Penalty();
							}
							
							afficheMessage("Mouvement non valide");
							
							if(fin == 1 ){
								
								printf("3 erreurs pour %s donc %s gagne le match\n",nameJ1, nameJ2);
								if(nbr_IA == 2){
									j1EndGame();
								}
								if(nbr_IA == 1){
									j2EndGame();
								}
							}
						}
						//Cas ou le drapeau a été détruie
						if(pion_erreur_j1 == 2){
							fin = 1;
							printf("%s a détruit le drapeau de %s, victoire de %s\n",nameJ1, nameJ2, nameJ1);
							if(nbr_IA == 2){
								j1EndGame();
							}
							if(nbr_IA == 1){
								j2EndGame();
							}
						}
						
					}while(pion_erreur_j1 == 1 && fin == 0);
					
					nbr_coup_j1 ++; //Un coup de réaliser
					
					if(fin == 0 && nbr_coup_j1<=nbr_coup_permis && nbr_coup_j2<=nbr_coup_permis){
						do{
							if(nbr_IA == 1){
								//COPIE DU CONTEXTE DE JEU QU'AVEC LES PIONS DU JOUEUR2
								gameStateJ2 = duplicationDuContexteDeJeu(gameState, couleurJ1, 2);	
								move = j1NextMove(&gameStateJ2);
								pion_erreur_j2 = verificationMouvement(move, &gameState, couleurJ2, 2, j1AttackResult, j2AttackResult, 1);
							}
							else if(nbr_IA == 2){
								interfaceGraphique(gameState);
								gameStateJ2 = duplicationDuContexteDeJeu(gameState, couleurJ1, 2);
								move = j2NextMove(&gameStateJ2);
								pion_erreur_j2 = verificationMouvement(move, &gameState, couleurJ2, 2, j1AttackResult, j2AttackResult, 2);
								interfaceGraphique(gameState);
							}
							
							else{
								gameStateJ2 = duplicationJoueur(gameState, couleurJ1, 2);
								interfaceGraphique(gameStateJ2);
								move = renvoieCoordonnees();
								move.start.line = 9-move.start.line;
								move.end.line = 9-move.end.line;						
								pion_erreur_j2 = verificationMouvement(move, &gameState, couleurJ2, 2, j1AttackResult, j2AttackResult, 0);
								gameStateJ2 = duplicationJoueur(gameState, couleurJ1, 2);
								interfaceGraphique(gameStateJ2);
							}
							
							//On vérifie qu'il y a encore un pion a bouger
							fin = verifPionsBouger(gameState);
							
							//Cas ou on a réalisé une erreure
							if(pion_erreur_j2 == 1){
								
								if(nbr_IA == 1){
									fin = finPartie(2,0);
									j1Penalty();
								}
								else if(nbr_IA == 2){
									fin = finPartie(2,0);
									j2Penalty();
								}
								
								afficheMessage("Mouvement non valide");
								if(fin == 1 ){
									printf("3 erreurs pour %s donc %s gagne le match\n",nameJ2, nameJ1);
									if(nbr_IA == 1){
										j1EndGame();
									}
									if(nbr_IA == 2){
										j2EndGame();
									}
								}
							}
							//Cas ou le drapeau a été détruie
							if(pion_erreur_j1 == 2){
								fin = 1;
								printf("%s a détruit le drapeau de %s, victoire de %s\n",nameJ2, nameJ1, nameJ2);
								if(nbr_IA == 1){
									j1EndGame();
								}
								if(nbr_IA == 2){
									j2EndGame();
								}
							}
						
							
						}while(pion_erreur_j2 == 1 && fin == 0);
						
							nbr_coup_j2 ++; //Un coup de réaliser
					}
				}
				
				//Si joueur 2 est rouge alors il commence
				else{
							
					do{
						if(nbr_IA == 1){
							printf("\ntoto 3");
							
							interfaceGraphique(gameState); 
							//COPIE DU CONTEXTE DE JEU QU'AVEC LES PIONS DU JOUEUR2
							gameStateJ2  = duplicationDuContexteDeJeu(gameState, couleurJ1, 2);
							
							move = j1NextMove(&gameStateJ2);
							pion_erreur_j2 = verificationMouvement(move, &gameState, couleurJ2, 2, j1AttackResult, j2AttackResult, 1);
						}
						
						else if(nbr_IA == 2){
							interfaceGraphique(gameState);
							//COPIE DU CONTEXTE DE JEU QU'AVEC LES PIONS DU JOUEUR2
							gameStateJ2 = duplicationDuContexteDeJeu(gameState, couleurJ1, 2);
							
							move = j2NextMove(&gameStateJ2);
							pion_erreur_j2 = verificationMouvement(move, &gameState, couleurJ2, 2, j1AttackResult, j2AttackResult, 2);
							interfaceGraphique(gameState);
						}
						else{
								gameStateJ2 = duplicationJoueur(gameState, couleurJ1, 2);
								interfaceGraphique(gameStateJ2);
								move = renvoieCoordonnees(); //A METTRE QUE LORSQUE C'EST UN JOUEUR HUMAIN 
								move.start.line = 9-move.start.line;
								move.end.line = 9-move.end.line;							
								pion_erreur_j2 = verificationMouvement(move, &gameState, couleurJ2, 2, j1AttackResult, j2AttackResult, 0);
								gameStateJ2 = duplicationJoueur(gameState, couleurJ1, 2);
								interfaceGraphique(gameStateJ2);
							}
						
						
						//On vérifie qu'il y a encore un pion a bouger
						fin = verifPionsBouger(gameState);
						
						//Cas ou on a réalisé une erreure
						if(pion_erreur_j2 == 1){
							if(nbr_IA == 1){
								fin = finPartie(2,0);
								j1Penalty();
							}	
							
							else if(nbr_IA == 2){
								fin = finPartie(2,0);
								j2Penalty();
							}	
							afficheMessage("Mouvement non valide");
							if(fin == 1 ){
								printf("3 erreurs pour %s donc %s gagne le match\n",nameJ2, nameJ1);
								if(nbr_IA == 1){
									j1EndGame();
								}
								if(nbr_IA == 2){
									j2EndGame();
								}
							}
						}
						//Cas ou le drapeau a été détruie
						if(pion_erreur_j1 == 2){
							fin = 1;
							printf("%s a détruit le drapeau de %s, victoire de %s\n",nameJ2, nameJ1, nameJ2);
								if(nbr_IA == 1){
									j1EndGame();
								}
								if(nbr_IA == 2){
									j2EndGame();
								}
						}
					}while(pion_erreur_j2 == 1 && fin == 0);
					
					nbr_coup_j2 ++; //Un coup de réaliser
					
					if(fin == 0){
						do{
							//COPIE DU CONTEXTE DE JEU QU'AVEC LES PIONS DU JOUEUR1
							if(nbr_IA == 2){
								interfaceGraphique(gameState);
								gameStateJ1 = duplicationDuContexteDeJeu(gameState, couleurJ2, 1);
							
								move = j1NextMove(&gameStateJ1);
								pion_erreur_j1 = verificationMouvement(move, &gameState, couleurJ1, 1, j1AttackResult, j2AttackResult, 2);
							}
							else{
								gameStateJ1 = duplicationJoueur(gameState, couleurJ2, 1);
								interfaceGraphique(gameStateJ1);
								move = renvoieCoordonnees(); //A METTRE QUE LORSQUE C'EST UN JOUEUR HUMAIN 
								move.start.line = 9-move.start.line;
								move.end.line = 9-move.end.line;					
								pion_erreur_j1 = verificationMouvement(move, &gameState, couleurJ1, 1, j1AttackResult, j2AttackResult, 0);
								gameStateJ1 = duplicationJoueur(gameState, couleurJ2, 1);
							}
							
							
							//On vérifie qu'il y a encore un pion a bouger
							fin = verifPionsBouger(gameState);
							
							//Cas ou on a réalisé une erreure
							if(pion_erreur_j1 == 1){
								if(nbr_IA == 2){
									fin = finPartie(1,0);
									j1Penalty();
								}
								afficheMessage("Mouvement non valide");
								if(fin == 1 ){
									printf("3 erreurs pour %s donc %s gagne le match\n",nameJ1, nameJ2);
									if(nbr_IA == 1){
										j1EndGame();
									}
									if(nbr_IA == 2){
										j2EndGame();
									}
								}
							}
							//Cas ou le drapeau a été détruie
							if(pion_erreur_j1 == 2){
								fin = 1;
								printf("%s a détruit le drapeau de %s, victoire de %s\n",nameJ1, nameJ2, nameJ1);
								if(nbr_IA == 1){
									j1EndGame();
								}
								if(nbr_IA == 2){
									j2EndGame();
								}
							}
							
							interfaceGraphique(gameState); //On affiche le plateau de jeu après le tour du joueur 1
							
						}while(pion_erreur_j1 == 1 && fin == 0);
						
						nbr_coup_j1 ++; //Un coup de réaliser
					}
				}
			}while(fin == 0  && nbr_coup_j1<=nbr_coup_permis  && nbr_coup_j2<=nbr_coup_permis);
		}

		
		if(nbr_coup_j1==nbr_coup_permis+1  || nbr_coup_j2==nbr_coup_permis+1){
			int i;
			
			for(i=0; i<11;i++){
				nbr_pion_rouge = nbr_pion_rouge + gameState.redOut[i];
			}
			for(i=0; i<11;i++){
				nbr_pion_bleu = nbr_pion_bleu + gameState.blueOut[i];
			}
			
			if(nbr_pion_rouge > nbr_pion_bleu){
				printf("Joueur rouge a gagné la partie avec un plus grand nombre de pions restant\n");
			}
			else if(nbr_pion_bleu> nbr_pion_rouge){
				printf("Joueur bleu a gagné la partie avec un plus grand nombre de pions restant\n");
			}
			else{
				printf("Match exaequo\n");
			}
			
		}
		
		//Demande de nouvelle partie				
		game = afficheMessageDemande("Voulez vous refaire une partie ?");	
		
		
		/*printf("\n=========================================================\nPLATEAU DE JEU\n");
		afficheConsole(gameState, couleurJ1, couleurJ2);
		printf("\n=========================================================\n");
		*/
		
	}while(game==1);
		
	//AJOUTER LES FREE ET LES CLOSES
	quitter_sdl();
		
	j1EndMatch();
	j2EndMatch();

	dlclose(j1Lib);
	dlclose(j2Lib);

	exit(EXIT_SUCCESS);	

	return(1);
}
else{
	return(0);
}
}

/*
 * Fonction qui initialise le contexte général du jeu, en implantant tous les case vide possible ainsi que les lacs
 */
void initialisationContexteJeu(SGameState *gameState){
	
	SBox box;
	int i,j;
	
	//Placement des box vides pour le moment
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
	//Nombre de pions morts
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

int vaEtViensJ1(SMove move){
	
	static int nbr_mvt_J1;
	int retour =0;
	static SPos pos1[2];

	if(pos1[0].line == move.start.line && pos1[0].col == move.start.col && pos1[1].line ==  move.end.line && pos1[1].col == move.end.col && nbr_mvt_J1%2 == 0){
		nbr_mvt_J1 ++;
		pos1[0] = move.start;
		pos1[1] = move.end;
	}
	else if(pos1[0].line == move.end.line && pos1[0].col == move.end.col && pos1[1].line ==  move.start.line && pos1[1].col ==  move.start.col && nbr_mvt_J1%2 == 1){
		nbr_mvt_J1 ++;
		pos1[0] = move.end;
		pos1[1] = move.start;
	}
	else{
		nbr_mvt_J1 = 1;
		pos1[0] = move.start;
		pos1[1] = move.end;
	}
	
	if( nbr_mvt_J1 >= 4){
		retour =1;
	}
	
	return retour;
}

int verifPionsBouger(SGameState gameState){
	
int retour = 0; 

if(gameState.redOut[1] == 1 && gameState.redOut[2] == 8 && gameState.redOut[3] == 5 && gameState.redOut[4] == 4 && gameState.redOut[5] == 4
&& gameState.redOut[6] == 4 && gameState.redOut[7] == 3 && gameState.redOut[8] == 2 && gameState.redOut[9] == 1 && gameState.redOut[10] == 1){
	printf("Fin de la partie pour le joueur rouge car plus de pion a déplacer");
	retour = 1;
}

if(gameState.blueOut[1] == 1 && gameState.blueOut[2] == 8 && gameState.blueOut[3] == 5 && gameState.blueOut[4] == 4 && gameState.blueOut[5] == 4
&& gameState.blueOut[6] == 4 && gameState.blueOut[7] == 3 && gameState.blueOut[8] == 2 && gameState.blueOut[9] == 1 && gameState.blueOut[10] == 1){
	printf("Fin de la partie pour le joueur bleu car plus de pion a déplacer");
	retour = 1;
}

return retour;
}

int vaEtViensJ2(SMove move){
	
	static int nbr_mvt_J2;
	int retour =0;
	static SPos pos2[2];

	if(pos2[0].line == move.start.line && pos2[0].col == move.start.col && pos2[1].line ==  move.end.line && pos2[1].col == move.end.col && nbr_mvt_J2%2 == 0){
		nbr_mvt_J2 ++;
		pos2[0] = move.start;
		pos2[1] = move.end;
	}
	else if(pos2[0].line == move.end.line && pos2[0].col == move.end.col && pos2[1].line ==  move.start.line && pos2[1].col ==  move.start.col && nbr_mvt_J2%2 == 1){
		nbr_mvt_J2 ++;
		pos2[0] = move.end;
		pos2[1] = move.start;
	}
	else{
		nbr_mvt_J2 = 1;
		pos2[0] = move.start;
		pos2[1] = move.end;
	}
	
	if( nbr_mvt_J2 >= 4){
		retour =1;
	}
	
	return retour;
}

/*
 * Fonction qui enregistre les pions placé par le joueur dans le contexte général du jeu
 * Joueur 1 : pion en bas du contexte
 * Joueur 2 : pion en haut du contexte
 */
void enregistrePion(EPiece boardInit[4][10], SGameState *gameState, EColor color, int joueur){
	
	SBox box;
	int i,j,lineJ, colJ;
	
	if(joueur == 1){
		for(i=0;i<4;i++){
			for(j=0;j<10;j++){	
				box.content = color;
				box.piece = boardInit[i][j];
				lineJ = 9 - i;
				gameState->board[lineJ][j] = box;
			}
		}
	}
	
	if(joueur == 2){
		for(i=0;i<4;i++){
			for(j=0;j<10;j++){	
				box.content = color;
				box.piece = boardInit[i][j];
				colJ = 9 - j;
				gameState->board[i][colJ] = box;
			}
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
	
	if(joueur == 1){
		for(i=0;i<10;i++){
			for(j=0;j<10;j++){
				if(gameState.board[i][j].content == color){
					box.content = color;
					box.piece = EPnone;
					gameStatePerso.board[9-i][j] = box;
				
				}
				else{
					gameStatePerso.board[9-i][j] = gameState.board[i][j];
				}
			}
		}
	}
	
	if(joueur == 2){
		for(i=0;i<10;i++){
			for(j=0;j<10;j++){
				if(gameState.board[i][j].content == color){
					box.content = color;
					box.piece = EPnone;
					gameStatePerso.board[i][9-j] = box;
				
				}
				else{
					gameStatePerso.board[i][9-j] = gameState.board[i][j];
				}
			}
		}
	}
	/*
	//Placement des pions adverse avec juste la couleur et ces pions avec toute la couleur
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
	}*/
	
	//On copies les pions sortie
	for(i=0;i<11;i++){
		gameStatePerso.redOut[i] = gameState.redOut[i];
		gameStatePerso.blueOut[i] = gameState.blueOut[i];
	}
	
	return gameStatePerso;
}

SGameState duplicationJoueur(SGameState gameState, EColor color, int joueur){
	SGameState gameStatePerso;
	SBox box;
	int i,j, m = 9;
	
	if(joueur == 1){
		for(i=0;i<10;i++){
			for(j=0;j<10;j++){
				if(gameState.board[i][j].content == color){
					box.content = color;
					box.piece = EPnone;
					gameStatePerso.board[i][j] = box;
				}
				else{
					gameStatePerso.board[i][j] = gameState.board[i][j];
				}
			}
		}
	}
	
	if(joueur == 2){
		for(i=0;i<10;i++){
			for(j=0;j<10;j++){
				if(gameState.board[i][j].content == color){
					box.content = color;
					box.piece = EPnone;
					gameStatePerso.board[9-i][9-j] = box;
				
				}
				else{
					gameStatePerso.board[9-i][9-j] = gameState.board[i][j];
				}
			}
		}
	}
	
	//On copies les pions sortie
	for(i=0;i<11;i++){
		gameStatePerso.redOut[i] = gameState.redOut[i];
		gameStatePerso.blueOut[i] = gameState.blueOut[i];
	}
	
	return gameStatePerso;
}

void placePion(EColor color, EPiece boardInit[4][10]){
	
	int i, j;
	SPos position;
	SBox box;
	SGameState SGame;
	
	for(i =0; i<4 ;i++){
		for(j=0; j<10;j++){
			 boardInit[i][j] = EPnone;
		}
	}
	interfaceGraphique(SGame);
	printf("Pose ton drapeau\n");
	do{
		position = getPos();
		if((position.line == -1 && position.col == -1) || 9-position.line < 6 || boardInit[9-position.line][9-position.col] != EPnone){
			printf("Erreur de placement, veuillez recommencez 1");
		}
	}while((position.line == -1 || position.col == -1) && 9-position.line < 6 && boardInit[9-position.line][9-position.col] != EPnone);
	boardInit[9-position.line][9-position.col] = EPflag;
	box.piece = EPflag;
	box.content = color;
	SGame.board[position.line][position.col] = box;
	interfaceGraphique(SGame);
	printf("Pose tes 6 bombes\n");

	for(i=0;i<6;i++){
		do{
			position = getPos();
			if((position.line == -1 && position.col == -1) || 9-position.line < 6 || boardInit[9-position.line][9-position.col] != EPnone){
				printf("Erreur de placement, veuillez recommencez 2");
			}
		}while((position.line == -1 || position.col == -1) && 9-position.line < 6 && boardInit[9-position.line][9-position.col] != EPnone);
		boardInit[9-position.line][9-position.col] = EPbomb;
		box.piece = EPbomb;
		box.content = color;
		SGame.board[position.line][position.col] = box;
		interfaceGraphique(SGame);
	}

	printf("Pose ton espion\n");
	do{
		position = getPos();
		if((position.line == -1 && position.col == -1) || 9-position.line < 6 || boardInit[9-position.line][9-position.col] != EPnone){
			printf("Erreur de placement, veuillez recommencez");
		}
	}while((position.line == -1 || position.col == -1) && 9-position.line < 6 && boardInit[9-position.line][9-position.col] != EPnone);
	boardInit[9-position.line][9-position.col] = EPspy;
	box.piece = EPspy;
	box.content = color;
	SGame.board[position.line][position.col] = box;
	interfaceGraphique(SGame);
	
	printf("Pose tes 8 scoutes\n");
	for(i=0;i<8;i++){
		do{
			position = getPos();
			if((position.line == -1 && position.col == -1) || 9-position.line < 6 || boardInit[9-position.line][9-position.col] != EPnone){
				printf("Erreur de placement, veuillez recommencez");
			}
		}while((position.line == -1 || position.col == -1) && 9-position.line < 6 && boardInit[9-position.line][9-position.col] != EPnone);
		boardInit[9-position.line][9-position.col] = EPscout;
		box.piece = EPscout;
		box.content = color;
		SGame.board[position.line][position.col] = box;
		interfaceGraphique(SGame);
	}
	
	printf("Pose tes 5 démineurs\n");
	for(i=0;i<5;i++){
		do{
			position = getPos();
			if((position.line == -1 && position.col == -1) || 9-position.line < 6 || boardInit[9-position.line][9-position.col] != EPnone){
				printf("Erreur de placement, veuillez recommencez");
			}
		}while((position.line == -1 || position.col == -1) && 9-position.line < 6 && boardInit[9-position.line][9-position.col] != EPnone);
		boardInit[9-position.line][9-position.col] = EPminer;
		box.piece = EPminer;
		box.content = color;
		SGame.board[position.line][position.col] = box;
		interfaceGraphique(SGame);
	}
	
	printf("Pose tes 4 sergeants\n");
	for(i=0;i<4;i++){
		do{
			position = getPos();
			if((position.line == -1 && position.col == -1) || 9-position.line < 6 || boardInit[9-position.line][9-position.col] != EPnone){
				printf("Erreur de placement, veuillez recommencez");
			}
		}while((position.line == -1 || position.col == -1) && 9-position.line < 6 && boardInit[9-position.line][9-position.col] != EPnone);
		boardInit[9-position.line][9-position.col] = EPsergeant;
		box.piece = EPsergeant;
		box.content = color;
		SGame.board[position.line][position.col] = box;
		interfaceGraphique(SGame);
	}
	
	printf("Pose tes 4 lieutenants\n");
	for(i=0;i<4;i++){
		do{
			position = getPos();
			if((position.line == -1 && position.col == -1) || 9-position.line < 6 || boardInit[9-position.line][9-position.col] != EPnone){
				printf("Erreur de placement, veuillez recommencez");
			}
		}while((position.line == -1 || position.col == -1) && 9-position.line < 6 && boardInit[9-position.line][9-position.col] != EPnone);
		boardInit[9-position.line][9-position.col] = EPlieutenant;
		box.piece = EPlieutenant;
		box.content = color;
		SGame.board[position.line][position.col] = box;
		interfaceGraphique(SGame);
	}
	
	printf("Pose tes 4 capitaines\n");
	for(i=0;i<4;i++){
		do{
			position = getPos();
			if((position.line == -1 && position.col == -1) || 9-position.line < 6 || boardInit[9-position.line][9-position.col] != EPnone){
				printf("Erreur de placement, veuillez recommencez");
			}
		}while((position.line == -1 || position.col == -1) && 9-position.line < 6 && boardInit[9-position.line][9-position.col] != EPnone);
		boardInit[9-position.line][9-position.col] = EPcaptain;
		box.piece = EPcaptain;
		box.content = color;
		SGame.board[position.line][position.col] = box;
		interfaceGraphique(SGame);
	}
	
	printf("Pose tes 3 majors\n");
	for(i=0;i<3;i++){
		do{
			position = getPos();
			if((position.line == -1 && position.col == -1) || 9-position.line < 6 || boardInit[9-position.line][9-position.col] != EPnone){
				printf("Erreur de placement, veuillez recommencez");
			}
		}while((position.line == -1 || position.col == -1) && 9-position.line < 6 && boardInit[9-position.line][9-position.col] != EPnone);
		boardInit[9-position.line][9-position.col] = EPmajor;
		box.piece = EPmajor;
		box.content = color;
		SGame.board[position.line][position.col] = box;
		interfaceGraphique(SGame);
	}
	
	printf("Pose tes 2 colonnel\n");
	for(i=0;i<2;i++){
		do{
			position = getPos();
			if((position.line == -1 && position.col == -1) || 9-position.line < 6 || boardInit[9-position.line][9-position.col] != EPnone){
				printf("Erreur de placement, veuillez recommencez");
			}
		}while((position.line == -1 || position.col == -1) && 9-position.line < 6 && boardInit[9-position.line][9-position.col] != EPnone);
		boardInit[9-position.line][9-position.col] = EPcolonel;
		box.piece = EPcolonel;
		box.content = color;
		SGame.board[position.line][position.col] = box;
		interfaceGraphique(SGame);
	}
	
	printf("Pose ton général\n");
	do{
		position = getPos();
		if((position.line == -1 && position.col == -1) || 9-position.line < 6 || boardInit[9-position.line][9-position.col] != EPnone){
			printf("Erreur de placement, veuillez recommencez");
		}
	}while((position.line == -1 || position.col == -1) && 9-position.line < 6 && boardInit[9-position.line][9-position.col] != EPnone);
	boardInit[9-position.line][9-position.col] = EPgeneral;
	box.piece = EPgeneral;
	box.content = color;
	SGame.board[position.line][position.col] = box;
	interfaceGraphique(SGame);
		
	printf("Pose ton marshal\n");
	do{
		position = getPos();
		if((position.line == -1 && position.col == -1) || 9-position.line < 6 || boardInit[9-position.line][9-position.col] != EPnone){
			printf("Erreur de placement, veuillez recommencez");
		}
	}while((position.line == -1 || position.col == -1) && 9-position.line < 6 && boardInit[9-position.line][9-position.col] != EPnone);
	boardInit[9-position.line][9-position.col] = EPmarshal;
	box.piece = EPmarshal;
	box.content = color;
	SGame.board[position.line][position.col] = box;
	interfaceGraphique(SGame);			
}

/*
 * FONCTION QUI VERIFIE QUE LE MOUVEMENT DU JOUEUR EST RÉALISABLE
 * SI OUI ON VÉRIFIE ENSUITE SI IL Y A UN CONFLITS OU PAS
 * SI NON ON ENVOIE UNE ERREUR AU JOUEUR
 */
int verificationMouvement(SMove move, SGameState *gameState,EColor color, int joueur, void(*AttackResult1)(SPos, EPiece, SPos, EPiece), void(*AttackResult2)(SPos, EPiece, SPos, EPiece), int nb_IA){
	
	SBox boxStart, boxEnd, newBox;
	int colorAdverse;
	
	//Sélectionne la couleur adverse
	if(color == ECred){
		colorAdverse = ECblue;
	}
	else colorAdverse = ECred;
	
		if(move.start.line>=0 && move.start.line<=9 && move.start.col>=0 && move.start.col<=9){	
			
			if((joueur == 1 && vaEtViensJ1(move) == 0) || (joueur == 2 && vaEtViensJ2(move) == 0)){
			
				//On inverse le mouvement pour le joueur 2
				if(joueur == 1){
					move.start.line = 9-move.start.line;
					move.end.line = 9-move.end.line;
				}
				if(joueur == 2){
					move.start.col = 9-move.start.col;
					move.end.col = 9-move.end.col;
				}

				boxStart = gameState->board[move.start.line][move.start.col];
				
				//VERIFICATION QUE LE PION SELECTIONNER CORRESPOND A UN PION DE LA BONNE COULEUR
				if(boxStart.content == color){
		
					//VERIFICATION QUE LE PION SELECTIONNER PEUT ETRE BOUGER
					if(boxStart.piece!=EPnone && boxStart.piece!=EPbomb && boxStart.piece!=EPflag){
					
						//ON VÉRIFIE QUE ARRIVÉ EST BIEN DANS LE JEU
						if(move.end.line>=0 && move.end.line<=9 && move.end.col>=0 && move.end.col<=9){
				
							boxEnd = gameState->board[move.end.line][move.end.col];
							
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
												   if(startCol == endCol && startLine != endLine && startLine+1 == endLine && gameState->board[startLine+1][endCol].content == colorAdverse) {
														startLine++;
													}
												   
												   //Cas si le prochain mouvement n'est pas une case vide (mouvement ligne)
													else if(startCol == endCol && startLine != endLine && gameState->board[startLine+1][endCol].content !=ECnone){
														return 1;
													}
													
													//Cas si le prochain mouvement est une case vide (mouvement ligne)
													else if(startCol == endCol && startLine != endLine && gameState->board[startLine+1][endCol].content == ECnone){
														startLine++;
													}
													 
													 //Cas si le prochain coup arrive a la ligne arrivé (mouvement colonne)
													if (startLine == endLine && startCol != endCol && startCol+1 == endCol && gameState->board[startLine][startCol+1].content == colorAdverse) {
														startCol++;
													}
													 
													 //Cas si le prochain mouvement n'est pas une case vide (mouvement colonne)
													else if (startLine == endLine && startCol != endCol && gameState->board[move.start.line][startCol+1].content !=ECnone){
														 return 1;
													}
													
													//Cas si le prochain mouvement est une case vide (mouvement colonne)
													else if (startLine == endLine && startCol != endCol && gameState->board[move.start.line][startCol+1].content ==ECnone){
														 startCol++;
													}
												}
												//CAS PION DESCENDANT
												else if(startLine > endLine || startCol > endCol){
													
													  //Cas si le prochain coup arrive a la ligne arrivé (mouvement ligne)
													if(startCol == endCol && startLine != endLine && startLine-1 == endLine && gameState->board[startLine-1][endCol].content == colorAdverse) {
														startLine--;
													}
													
													 //Cas si le prochain mouvement n'est pas une case vide (mouvement ligne)
													else if(startCol == endCol && startLine != endLine && gameState->board[startLine-1][endCol].content !=ECnone){
														return 1;
													}
													
													//Cas si le prochain mouvement est une case vide (mouvement ligne)
													else if(startCol == endCol && startLine != endLine && gameState->board[startLine-1][endCol].content == ECnone){
														startLine--;
													}
													 
													  //Cas si le prochain coup arrive a la ligne arrivé (mouvement colonne)
													if (startLine == endLine && startCol != endCol && startCol-1 == endCol && gameState->board[startLine][startCol-1].content == colorAdverse) {
														startCol--;
													}
													  //Cas si le prochain mouvement n'est pas une case vide (mouvement colonne)
													else if (startLine == endLine && startCol != endCol && gameState->board[move.start.line][startCol-1].content !=ECnone){
														 return 1;
													}
													
													//Cas si le prochain mouvement est une case vide (mouvement colonne)
													else if (startLine == endLine && startCol != endCol && gameState->board[move.start.line][startCol-1].content ==ECnone){
														 startCol--;
													}
												}
											}
										printf("Déplacement réalisé\n");
										
										//ON VÉRIFIE SI LE DÉPLACEMENT ENTRAINE UNE ATTAQUE OU PAS
										if(boxEnd.content==colorAdverse){
											return attaque(move, gameState,color, joueur, AttackResult1, AttackResult2, nb_IA);
										}
										else{
											//ON MODIFIE LE CONTEXTE DE JEU AVEC LE DÉPLACEMENT
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
									   (move.start.line == move.end.line && move.start.col == move.end.col-1) ||
									   (move.start.line == move.end.line+1 && move.start.col == move.end.col) ||
									   (move.start.line == move.end.line-1 && move.start.col == move.end.col)){
										   
										printf("Déplacement réalisé\n");
										//ON VÉRIFIE SI LE DÉPLACEMENT ENTRAIN UNE ATTAQUE OU PAS
										if(boxEnd.content==colorAdverse){
											return attaque(move, gameState,color, joueur, AttackResult1, AttackResult2, nb_IA);
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
		}
	return 1;
}

/*
 * FONCTION QUI ENVOIE AU JOUEUR QU'IL Y A UN COMBAT
 * VÉRIFIE QU'ELLE EST LE RÉSULTAT DE L'ATTAQUE ET MODIFIE LE CONTEXTE DE JEU SELON LE TYPE DE COMBAT
 * RENVOIE 2 LORSQUE LE DRAPEAU ÉNNEMIE A ÉTÉ CAPTURÉ LORS DU ATTAQUE
 */
int attaque(SMove move, SGameState *gameState,EColor color, int joueur, void(*AttackResultJ1)(SPos, EPiece, SPos, EPiece), void(*AttackResultJ2)(SPos, EPiece, SPos, EPiece), int nbr_IA){
	SBox attaquant, attaquer, newBox;
	SMove moveJ2, moveJ1;
	
	//ON DEFINIE QUI ATTAQUE ET QUI EST ATTAQUÉ
	attaquant = gameState->board[move.start.line][move.start.col];
	attaquer = gameState->board[move.end.line][move.end.col];
	
	moveJ1.start.line = 9 - move.start.line;
	moveJ1.start.col = move.start.col;
	moveJ1.end.line = 9 - move.end.line;
	moveJ1.end.col = move.end.col;
	
	moveJ2.end.line = move.end.line;
	moveJ2.end.col = 9 - move.end.col;
	moveJ2.start.line = move.start.line;
	moveJ2.start.col = 9 - move.start.col;
	
	//JOUEUR 1 QUI ATTAQUE
	if(joueur == 1){
		
		if(nbr_IA == 2){
			(*AttackResultJ1)(moveJ1.start, attaquant.piece, moveJ1.end, attaquer.piece);
			(*AttackResultJ2)(moveJ2.end, attaquer.piece, moveJ2.start, attaquant.piece);
		}
		if(nbr_IA == 1){
			(*AttackResultJ1)(moveJ2.end, attaquer.piece, moveJ2.start, attaquant.piece);
		}
		
	} 
	//JOUEUR 2 QUI ATTAQUE
	else
	{
		if(nbr_IA == 2){
			(*AttackResultJ2)(moveJ2.start, attaquant.piece, moveJ2.end, attaquer.piece);
			(*AttackResultJ1)(moveJ1.end, attaquer.piece, moveJ1.start, attaquant.piece);
		}
		if(nbr_IA == 1){
			(*AttackResultJ1)(moveJ2.start, attaquant.piece, moveJ2.end, attaquer.piece);
		}
	}
	
	
	//CAS OU LES 2 PIÈCES SONT DE FORCE ÉQUIVALENTE
	if(attaquant.piece == attaquer.piece){
		
		//On incrémente les tableaux de perte
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
	else if((attaquant.piece > attaquer.piece && attaquer.piece !=EPbomb) || 
	(attaquant.piece == EPminer && attaquer.piece == EPbomb) ||
	(attaquant.piece == EPspy && attaquer.piece == EPmarshal)){
		
		//Cas ou c'est le joueur bleu qui perd une pièce
		if(attaquant.content == ECred){
			gameState->blueOut[attaquer.piece]++;
		} 
		//Cas ou c'est le joueur rouge qui perd une pièce
		else {
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
		//Cas ou c'est le joueur rouge qui perd une pièce
		if(attaquant.content == ECred){
			gameState->redOut[attaquant.piece]++;
		} 
		//Cas ou c'est le joueur bleu qui perd une pièce
		else {
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
	
	//Nombre erreurs
	static int erreur_j1, erreur_j2;
	
	//Nouvelle partie on initialise les erreurs a 0
	if(joueur == 0){
		erreur_j1 = 0;
		erreur_j2 = 0;
	}
	
	//Cas ou le joueur 1 fait une faute et qui a toujours sont drapeau
	if(joueur == 1 && flag != 1){
		erreur_j1++;
	}
	
	//Cas ou le joueur 2 fait une faute et qui a toujours sont drapeau
	if(joueur == 2 && flag != 1){
		erreur_j2++;
	}
	
	//Cas ou le joueur vient de perdre son drapeau fin partie
	if(flag == 1){
		return 1;
	}
	
	//Cas ou le joueur a fait 3 fautes fin partie
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

static void * quitter(void * p_data){
	
	int continuer = 1;
	SDL_Event event;
	
	while (continuer){
		SDL_Delay(100);
		SDL_WaitEvent(&event);
		
		switch(event.type){
			case SDL_QUIT:
				continuer = 0;
				break;
				
			case SDL_KEYDOWN:
				switch(event.key.keysym.sym){
					
					case SDLK_ESCAPE:
						continuer = 0;
						break;
					
					default:
						break;
				}
				break;
		}
	}
	//AJOUTER TOUS LES FREE ET CLOSE A RÉALISER
	/*j1EndMatch();
	j2EndMatch();*/

	/*dlclose(j1Lib);
	dlclose(j2Lib);*/

	quitter_sdl();
	exit(EXIT_SUCCESS);
	
}

