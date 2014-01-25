/**
 * Projet Stratego, 3ème année de polytech
 * Réalisé par : -Guinard Arnaud
 * 				 -Mouden Benjamin
 * 				 -Evo Brieuc
 * 				 -Chotard François
 */

#include "PolyLib.h"
#include "Arbitre.h"
#include "IGStratego.h"

//Liste des variales globales
FILE *fichier;
void *j1Lib, *j2Lib;


int main(int argc, const char * argv[]){

int nbr_coup_permis = atoi(argv[1]);
int nbr_IA = atoi(argv[2]);

if(nbr_coup_permis != 0 && nbr_IA > -1 && nbr_IA < 3){


	//Ensemble des fonction communes a toutes les librairies
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
	SGameState gameState;//Plateau du jeu a dupliquer afin eviter qu'une libraire modifie original
	SGameState gameStateJ1,gameStateJ2;//Plateau du jeu pour le joueur 1 et 2
	SMove move;
	char nameJ1[50],  nameJ2[50], texte[200];
	int game, fin ,couleur, pion_erreur_j1, pion_erreur_j2, nbr_coup_j1, nbr_coup_j2, nbr_pion_rouge=0, nbr_pion_bleu=0, arret,i,j, partie=1;
	EColor couleurJ1, couleurJ2; //Différents couleurs des joueurs
	EPiece boardInitJ1[4][10], boardInitJ2[4][10];
	pthread_t thread_quitter; //Thread qui permet de quitter la fenetre lorsque 2 IA se combattent
	srand(time(NULL));//Pour la fonction aléatoire qui va définir les couleurs
	
	//Log qui contient
	fichier = fopen("log.txt", "w");

	//Initialisation des librarie
	if(nbr_IA >0){
		j1InitLibrary(nameJ1);
		j1StartMatch();
	}
	if(nbr_IA >1){
		j2InitLibrary(nameJ2);
		j2StartMatch();
	}

	//Ecriture de le log.txt
	if(nbr_IA == 2){
		fprintf (fichier, "Debut de match entre deux IA\n");
		fprintf (fichier, "%s VS %s\n", nameJ1,  nameJ2);
		fprintf (fichier, "joueur 1 : %s\n", nameJ1);
		fprintf (fichier, "joueur 2 : %s\n", nameJ2);
	}
	else if(nbr_IA == 1){
		fprintf (fichier, "Debut de match entre une IA et un humain\n");
		fprintf (fichier, "joueur 1 : Humain\n");
		fprintf (fichier, "joueur 2 : %s\n", nameJ1);
	}
	else if(nbr_IA == 0){
		fprintf (fichier, "Debut de match entre deux humain\n");
	}
	
	
	//Ouverture du thread lorsqu'on a 2 IA
	if(nbr_IA == 2){
		XInitThreads();
		pthread_create (&thread_quitter, NULL, quitter, NULL);
	}
		
	do {
		
		fprintf(fichier,"\n\nDebut de la partie N°%i\n", partie);
		
		fin =0;	//Lorsque fin = 1 alors la partie est finie
		//Nombre de coups joué
		nbr_coup_j1 = 1;
		nbr_coup_j2 = 1;
		
		//On initialise a 1 car aucune ereurs commise actuellement
		pion_erreur_j1 = 1;
		pion_erreur_j2 = 1;
		
		//On initialise le nombre de faute a 0
		finPartie(0,0);
		
//INITIALISATION DU CONTEXTE DE JEU
		initialisationContexteJeu(&gameState);
		//Initialise l'interface graphique
		initInterface();
		interfaceGraphique(gameState);
		
		//Diffinission de la couleurs des joueurs
		couleur = (int)rand()%2;

//ENREGISTREMENT ET VERIFICATION DES PIONS DES JOUEURS
		do{
			if(couleur == 0){ //Si c'est egal a 0 alors le joueur 1 sera rouge et joueur 2 sera bleu sinon inverse
				
				fprintf (fichier, "joueur 1 couleur rouge et joueur 2 couleur bleu\n");
				
				//Joueur 1 = rouge
				if(pion_erreur_j1 !=0){
					
					//Placement pion pour IA
					if(nbr_IA == 2){
						j1StartGame(ECred,boardInitJ1);
						couleurJ1 = ECred;
					}
					
					//Placement pion pour l'humain
					else{
						arret = placePion(ECred, boardInitJ1, 1);
						//Cas ou il quitte la fenetre
						if(arret == 1){
							if(nbr_IA == 1){
								j1EndMatch();
								dlclose(j1Lib);
							}
							fclose(fichier);
							quitter_sdl();
							exit(EXIT_SUCCESS);
						}
						couleurJ1 = ECred;
					}
					pion_erreur_j1 =  verificationNombrePiece(boardInitJ1);//Vérification du placement des différents pions
				}

				//Joueur 2 = Bleu
				if(pion_erreur_j2 !=0){
					
					//Placement pion pour IA
					if(nbr_IA == 1){
						j1StartGame(ECblue,boardInitJ2);
						couleurJ2 = ECblue;
					}
					
					//Placement pion pour IA
					else if(nbr_IA == 2){
						j2StartGame(ECblue,boardInitJ2);
						couleurJ2 = ECblue;
					}
					
					//Placement pion pour l'humain
					else{
						arret = placePion(ECblue, boardInitJ2, 2);
						//Cas ou il quitte la fenetre
						if(arret == 1){
							fclose(fichier);
							quitter_sdl();
							exit(EXIT_SUCCESS);
						}
						couleurJ2 = ECblue;		
					}
					pion_erreur_j2 =  verificationNombrePiece(boardInitJ2);//Vérification du placement des différents pions
				}
			} 
			//Joueur 1 = Bleu
			else{
				
				fprintf(fichier, "joueur 1 couleur bleu et joueur 2 couleur rouge\n");
				
				//Placement pion pour IA
				if(pion_erreur_j1 !=0){
					if(nbr_IA == 2){
						j1StartGame(ECblue,boardInitJ1);
						couleurJ1 = ECblue;
					}
					
					//Placement pion pour l'humain
					else{
						arret = placePion(ECblue, boardInitJ1, 1);
						//Cas ou il quitte la fenetre
						if(arret == 1){
							fclose(fichier);
							quitter_sdl();
							exit(EXIT_SUCCESS);
						}
						couleurJ1 = ECblue;		
					}
					pion_erreur_j1 =  verificationNombrePiece(boardInitJ1);//Vérification du placement des différents pions
				}
				//Joueur 2 = rouge
				if(pion_erreur_j2 == 1){
					
					//Placement pion pour IA
					if(nbr_IA == 1){
						j1StartGame(ECred,boardInitJ2);
						couleurJ2 = ECred;
					}
					//Placement pion pour IA
					else if(nbr_IA == 2){
						j2StartGame(ECred,boardInitJ2);
						couleurJ2 = ECred;
					}
					
					//Placement pion pour l'humain
					else{
						arret = placePion(ECred, boardInitJ2, 2);
						//Cas ou il quitte la fenetre
						if(arret == 1){
							if(nbr_IA == 1){
								j1EndMatch();
								dlclose(j1Lib);
							}
							fclose(fichier);
							quitter_sdl();
							exit(EXIT_SUCCESS);
						}
						couleurJ2 = ECred;
					}
					pion_erreur_j2 =  verificationNombrePiece(boardInitJ2);//Vérification du placement des différents pions
				}
			}
			
			//Cas ou le joueur 1 a réaliser une erreur de placement
			if(pion_erreur_j1 == 1){
				
				fprintf (fichier, "Erreur placement des pions de depart pour le joueur 1\n");
				
				if(nbr_IA == 2){
					fin = finPartie(1,0);
					j1Penalty();
				}
				
				//3 erreurs réaliser par le joueur 1
				if(fin == 1 ){
					afficherMessageEcran("Victoire joueur 2, joueur 1 a fait 3 erreures", 3000);
					if(nbr_IA == 1){
						j1EndGame();
					}
					if(nbr_IA == 2){
						j2EndGame();
					}
				}
			}
			
			//Cas ou le joueur 2 a réaliser une erreur de placement
			if(pion_erreur_j2 == 1){
				
				fprintf (fichier, "Erreur placement des pions de depart pour le joueur 2\n");
				
				if(nbr_IA == 1){
					fin = finPartie(2,0);
					j1Penalty();
				}
				else if(nbr_IA == 2){
					fin = finPartie(2,0);
					j2Penalty();
				}
				
				//3 erreurs réaliser par le joueur 2
				if(fin == 1 ){
					afficherMessageEcran("Victoire joueur 1, joueur 2 a fait 3 erreure", 3000);
					if(nbr_IA == 1){
						j1EndGame();
					}
					if(nbr_IA == 2){
						j2EndGame();
					}
				}
			}
			
		}while((pion_erreur_j1 == 1 || pion_erreur_j2 == 1) && fin == 0 );
		
		if(fin == 0){
			
	//ENREGISTRE LES PIONS DU JOUEUR 1 SUR LE CONTEXTE DE JEU
			enregistrePion(boardInitJ1, &gameState, couleurJ1, 1);
			
	//ENREGISTRE LES PIONS DU JOUEUR 2 SUR LE CONTEXTE DE JEU
			enregistrePion(boardInitJ2, &gameState, couleurJ2, 2);
			
			//Affichage des pions place par les joueurs dans le log.txt
			fprintf(fichier, "\n------------------------------------------------------------------------------\n\n");
			fprintf(fichier, "Placement des pions sur le jeu\n");
			ecrireFichier(gameState);
			do{
				
	//DEPLACEMENT D'UN PION
	
				//Si joueur 1 est rouge alors il commence
				if(couleurJ1 == ECred){
					do{
						
						fprintf(fichier, "\n------------------------------------------------------------------------------\n\n");
						fprintf(fichier, "Deplacement du joueur 1\n");
						
						//Deplacement par IA
						if(nbr_IA == 2){
							gameStateJ1 = duplicationDuContexteDeJeu(gameState, couleurJ2, 1);//Dupplication de l'etat de jeu pour IA
							move = j1NextMove(&gameStateJ1);//Mouvement de IA
							pion_erreur_j1 = verificationMouvement(move, &gameState, couleurJ1, 1, j1AttackResult, j2AttackResult, nbr_IA);//Verification du mouvement
							interfaceGraphique(gameState);
						}
						
						//Deplacement par l'humain
						else{
							gameStateJ1 = duplicationJoueur(gameState, couleurJ2, 1);
							interfaceGraphique(gameStateJ1);
							move = renvoieCoordonnees(gameState);
							
							//Cas ou il quitte la fenetre
							if(move.start.line == -2 && move.start.col == -2){

								if(nbr_IA == 1){
									j1EndMatch();
									dlclose(j1Lib);
								}
								fclose(fichier);
								quitter_sdl();
								exit(EXIT_SUCCESS);
							}
							
							//On modifie les position du mouvement pour correspondre a l'état de jeu général
							move.start.line = 9-move.start.line;
							move.end.line = 9-move.end.line;	
							
							pion_erreur_j1 = verificationMouvement(move, &gameState, couleurJ1, 1, j1AttackResult, j2AttackResult, nbr_IA);
							gameStateJ1 = duplicationJoueur(gameState, couleurJ2, 1);
							interfaceGraphique(gameStateJ1);
						}
						
						//On verifie qu'il y a encore un pion a bouger
						fin = verifPionsBouger(gameState);
						
						//Cas ou le joueur 1 a réaliser une erreur de mouvement
						if(pion_erreur_j1 == 1){
							if(nbr_IA == 2){
								fin = finPartie(1,0);
								j1Penalty();
								pion_erreur_j1 = 0;
							}
							
							afficherMessageEcran("Mouvement non valide", 1000);
							
							//3 erreurs réaliser par le joueur 1
							if(fin == 1 ){
								afficherMessageEcran("Victoire joueur 2, joueur 1 a fait 3 erreure", 3000);
								if(nbr_IA == 2){
									j1EndGame();
								}
								if(nbr_IA == 1){
									j2EndGame();
								}
							}
						}
						
						//Drapeau détruit par le joueur 1
						if(pion_erreur_j1 == 2){
							fin = 1;
							afficherMessageEcran("Victoire joueur 1, joueur 2 perdu son drapeau", 3000);
							if(nbr_IA == 2){
								j1EndGame();
							}
							if(nbr_IA == 1){
								j2EndGame();
							}
						}
						
					}while(pion_erreur_j1 == 1 && fin == 0);
					
					//Ecrire le gameState dans log.txt
					ecrireFichier(gameState);
					
					nbr_coup_j1 ++; //Un coup de realiser
					
					if(fin == 0 && nbr_coup_j1<=nbr_coup_permis && nbr_coup_j2<=nbr_coup_permis){
						do{
							
							fprintf(fichier, "\n------------------------------------------------------------------------------\n\n");
							fprintf(fichier, "Deplacement du joueur 2\n");
							
							//Deplacement par IA
							if(nbr_IA == 1){
								gameStateJ2 = duplicationDuContexteDeJeu(gameState, couleurJ1, 2);	
								move = j1NextMove(&gameStateJ2);
								pion_erreur_j2 = verificationMouvement(move, &gameState, couleurJ2, 2, j1AttackResult, j2AttackResult, nbr_IA);
							}
							
							//Deplacement par IA
							else if(nbr_IA == 2){
								interfaceGraphique(gameState);
								gameStateJ2 = duplicationDuContexteDeJeu(gameState, couleurJ1, 2);
								move = j2NextMove(&gameStateJ2);
								pion_erreur_j2 = verificationMouvement(move, &gameState, couleurJ2, 2, j1AttackResult, j2AttackResult, nbr_IA);
								interfaceGraphique(gameState);
							}
							
							//Deplacement par l'humain
							else{
								gameStateJ2 = duplicationJoueur(gameState, couleurJ1, 2);
								interfaceGraphique(gameStateJ2);
								move = renvoieCoordonnees(gameState);
								
								//Cas ou il quitte la fenetre
								if(move.start.line == -2 && move.start.col == -2){
									fclose(fichier);
									quitter_sdl();
									exit(EXIT_SUCCESS);
								}
								
								//On modifie les position du mouvement pour correspondre a l'état de jeu général
								move.start.line = 9-move.start.line;
								move.end.line = 9-move.end.line;	
													
								pion_erreur_j2 = verificationMouvement(move, &gameState, couleurJ2, 2, j1AttackResult, j2AttackResult, nbr_IA);
								gameStateJ2 = duplicationJoueur(gameState, couleurJ1, 2);
								
								interfaceGraphique(gameStateJ2);
							}
							
							//On verifie qu'il y a encore un pion a bouger
							fin = verifPionsBouger(gameState);
							
							//Cas ou le joueur 2 a réaliser une erreur de mouvement
							if(pion_erreur_j2 == 1){
								
								if(nbr_IA == 1){
									fin = finPartie(2,0);
									j1Penalty();
									pion_erreur_j2 = 0;
								}
								else if(nbr_IA == 2){
									fin = finPartie(2,0);
									j2Penalty();
									pion_erreur_j2 = 0;
								}
								
								afficherMessageEcran("Mouvement non valide", 1000);
								
								//3 erreurs réaliser par le joueur 2
								if(fin == 1 ){
									afficherMessageEcran("Victoire joueur 1, joueur 2 a fait 3 erreure", 3000);
									if(nbr_IA == 1){
										j1EndGame();
									}
									if(nbr_IA == 2){
										j2EndGame();
									}
								}
							}
							
							//Drapeau détruit par le joueur 2
							if(pion_erreur_j2 == 2){
								fin = 1;
								afficherMessageEcran("Victoire joueur 2, joueur 1 perdu son drapeau", 3000);
								if(nbr_IA == 1){
									j1EndGame();
								}
								if(nbr_IA == 2){
									j2EndGame();
								}
							}
						
							
						}while(pion_erreur_j2 == 1 && fin == 0);
						
							//Ecrire gameState dans le log.txt
							ecrireFichier(gameState);
						
							nbr_coup_j2 ++; //Un coup de realiser
					}
				}
				
				//Si joueur 2 est rouge alors il commence
				else{
							
					do{
						
						fprintf(fichier, "\n------------------------------------------------------------------------------\n\n");
						fprintf(fichier, "Deplacement du joueur 2\n");
						
						//Deplacement par IA
						if(nbr_IA == 1){
							gameStateJ2  = duplicationDuContexteDeJeu(gameState, couleurJ1, 2);
							move = j1NextMove(&gameStateJ2);
							pion_erreur_j2 = verificationMouvement(move, &gameState, couleurJ2, 2, j1AttackResult, j2AttackResult, nbr_IA);
						}
						
						//Deplacement par IA
						else if(nbr_IA == 2){
							interfaceGraphique(gameState);
							gameStateJ2 = duplicationDuContexteDeJeu(gameState, couleurJ1, 2);
							move = j2NextMove(&gameStateJ2);
							pion_erreur_j2 = verificationMouvement(move, &gameState, couleurJ2, 2, j1AttackResult, j2AttackResult, nbr_IA);
							interfaceGraphique(gameState);
						}
						
						//Deplacement par l'humain
						else{
								gameStateJ2 = duplicationJoueur(gameState, couleurJ1, 2);
								interfaceGraphique(gameStateJ2);
								move = renvoieCoordonnees(gameState);
								
								//Cas ou il quitte la fenetre
								if(move.start.line == -2 && move.start.col == -2){
									if(nbr_IA == 1){
										j1EndMatch();
										dlclose(j1Lib);
									}
									fclose(fichier);
									quitter_sdl();
									exit(EXIT_SUCCESS);
								}
								
								//On modifie les position du mouvement pour correspondre a l'état de jeu général
								move.start.line = 9-move.start.line;
								move.end.line = 9-move.end.line;	
														
								pion_erreur_j2 = verificationMouvement(move, &gameState, couleurJ2, 2, j1AttackResult, j2AttackResult, nbr_IA);
								gameStateJ2 = duplicationJoueur(gameState, couleurJ1, 2);
								interfaceGraphique(gameStateJ2);
							}
						
						
						//On verifie qu'il y a encore un pion a bouger
						fin = verifPionsBouger(gameState);
						
						//Cas ou le joueur 2 a réaliser une erreur de mouvement
						if(pion_erreur_j2 == 1){
							
							if(nbr_IA == 1){
								fin = finPartie(2,0);
								j1Penalty();
								pion_erreur_j2 = 0;
							}	
							
							else if(nbr_IA == 2){
								fin = finPartie(2,0);
								j2Penalty();
								pion_erreur_j2 = 0;
							}	
							
							afficherMessageEcran("Mouvement non valide", 1000);
							
							//3 erreurs réaliser par le joueur 2
							if(fin == 1 ){
								afficherMessageEcran("Victoire joueur 1, joueur 2 a fait 3 erreure", 3000);
								if(nbr_IA == 1){
									j1EndGame();
								}
								if(nbr_IA == 2){
									j2EndGame();
								}
							}
						}
						
						//Drapeau détruit par le joueur 2
						if(pion_erreur_j1 == 2){
							fin = 1;
							afficherMessageEcran("Victoire joueur 2, joueur 1 perdu son drapeau", 3000);
								if(nbr_IA == 1){
									j1EndGame();
								}
								if(nbr_IA == 2){
									j2EndGame();
								}
						}
					}while(pion_erreur_j2 == 1 && fin == 0);
					
					nbr_coup_j2 ++; //Un coup de realiser
					
					//Ecrire gameState dans le log.txt
					ecrireFichier(gameState);
					
					if(fin == 0){
						do{
							
							fprintf(fichier, "\n------------------------------------------------------------------------------\n\n");
							fprintf(fichier, "Deplacement du joueur 1\n");
							
							//Deplacement par IA
							if(nbr_IA == 2){
								interfaceGraphique(gameState);
								gameStateJ1 = duplicationDuContexteDeJeu(gameState, couleurJ2, 1);
								move = j1NextMove(&gameStateJ1);
								pion_erreur_j1 = verificationMouvement(move, &gameState, couleurJ1, 1, j1AttackResult, j2AttackResult, nbr_IA);
							}
							
							//Deplacement par l'humain
							else{
								gameStateJ1 = duplicationJoueur(gameState, couleurJ2, 1);
								interfaceGraphique(gameStateJ1);
								move = renvoieCoordonnees(gameState); 
								
								//Cas ou il quitte la fenetre
								if(move.start.line == -2 && move.start.col == -2){
									if(nbr_IA == 1){
										j1EndMatch();
										dlclose(j1Lib);
									}
									fclose(fichier);
									quitter_sdl();
									exit(EXIT_SUCCESS);
								}
								
								//On modifie les position du mouvement pour correspondre a l'état de jeu général
								move.start.line = 9-move.start.line;
								move.end.line = 9-move.end.line;	
												
								pion_erreur_j1 = verificationMouvement(move, &gameState, couleurJ1, 1, j1AttackResult, j2AttackResult, nbr_IA);
								gameStateJ1 = duplicationJoueur(gameState, couleurJ2, 1);
							}
							
							
							//On verifie qu'il y a encore un pion a bouger
							fin = verifPionsBouger(gameState);
							
							//Cas ou le joueur 1 a réaliser une erreur de mouvement
							if(pion_erreur_j1 == 1){
								
								if(nbr_IA == 2){
									fin = finPartie(1,0);
									j1Penalty();
									pion_erreur_j1 = 0;
								}
								
								afficherMessageEcran("Mouvement non valide", 1000);
								
								//3 erreurs réaliser par le joueur 1
								if(fin == 1 ){
									afficherMessageEcran("Victoire joueur 2, joueur 1 a fait 3 erreure", 3000);
									if(nbr_IA == 1){
										j1EndGame();
									}
									if(nbr_IA == 2){
										j2EndGame();
									}
								}
							}
							
							//Drapeau détruit par le joueur 1
							if(pion_erreur_j1 == 2){
								fin = 1;
								afficherMessageEcran("Victoire joueur 1, joueur 2 perdu son drapeau", 3000);
								if(nbr_IA == 1){
									j1EndGame();
								}
								if(nbr_IA == 2){
									j2EndGame();
								}
							}
							
							
						}while(pion_erreur_j1 == 1 && fin == 0);
						
						//Ecrire gameState dans le log.txt
						ecrireFichier(gameState);
						
						nbr_coup_j1 ++; //Un coup de realiser
					}
				}
			}while(fin == 0  && nbr_coup_j1<=nbr_coup_permis  && nbr_coup_j2<=nbr_coup_permis);//Tant qu'il y a encore des coups a jouer
		}
		
//FIN DE LA PARTIE
		afficherMessageEcran("FIN DE LA PARTIE", 3000);
		
		fprintf(fichier,"\n\nFIN PARTIE\n");
		
		//Cas ou on a atteint le nombre de coup limite
		if(nbr_coup_j1==nbr_coup_permis+1  || nbr_coup_j2==nbr_coup_permis+1){
			int i;
			
			for(i=0; i<11;i++){
				nbr_pion_rouge = nbr_pion_rouge + gameState.redOut[i];
			}
			for(i=0; i<11;i++){
				nbr_pion_bleu = nbr_pion_bleu + gameState.blueOut[i];
			}
			
			//Cas ou le joueur rouge a gagné car il a plus de pions
			if(nbr_pion_rouge > nbr_pion_bleu){
				fprintf(fichier,"Joueur rouge a gagne la partie avec un plus grand nombre de pions restant\n");
				afficherMessageEcran("Joueur rouge a gagne la partie avec un plus grand nombre de pions restant", 3000);
			}
			
			//Cas ou le joueur bleu a gagné car il a plus de pions
			else if(nbr_pion_bleu> nbr_pion_rouge){
				fprintf(fichier,"Joueur bleu a gagne la partie avec un plus grand nombre de pions restant\n");
				afficherMessageEcran("Joueur bleu a gagne la partie avec un plus grand nombre de pions restant", 3000);
			}
			
			//Cas ou il y a un match exaequo
			else{
				fprintf(fichier,"Match exaequo\n");
				afficherMessageEcran("Match exaequo", 3000);
			}
			
		}
		
		//Demande de nouvelle partie				
		game = afficheMessageDemande("Voulez vous refaire une partie ?");	
		
		partie ++;
		
	}while(game==1);
		
	//On ferme tous les fonctions nécissantant une fermeture
	fclose(fichier);
	quitter_sdl();
		
	j1EndMatch();
	j2EndMatch();

	dlclose(j1Lib);
	dlclose(j2Lib);

	return(1);
}
else{
	return(0);
}
}

/*
 * Fonction qui initialise le contexte general du jeu, en implantant tous les case vide possible ainsi que les lacs
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
 * Fonction qui vérifie si il reste des pions a bouger ou pas
 * Renvoie 1 quand on peut plus bouger de pions sinon 0
 */
int verifPionsBouger(SGameState gameState){
	
int retour = 0; 

if(gameState.redOut[1] == 1 && gameState.redOut[2] == 8 && gameState.redOut[3] == 5 && gameState.redOut[4] == 4 && gameState.redOut[5] == 4
&& gameState.redOut[6] == 4 && gameState.redOut[7] == 3 && gameState.redOut[8] == 2 && gameState.redOut[9] == 1 && gameState.redOut[10] == 1){
	fprintf(fichier, "\n\nFin de la partie pour le joueur rouge car plus de pion a deplacer\n");
	afficherMessageEcran("Fin de la partie pour le joueur rouge car plus de pion a deplacer", 3000);
	retour = 1;
}

if(gameState.blueOut[1] == 1 && gameState.blueOut[2] == 8 && gameState.blueOut[3] == 5 && gameState.blueOut[4] == 4 && gameState.blueOut[5] == 4
&& gameState.blueOut[6] == 4 && gameState.blueOut[7] == 3 && gameState.blueOut[8] == 2 && gameState.blueOut[9] == 1 && gameState.blueOut[10] == 1){
	fprintf(fichier, "\n\nFin de la partie pour le joueur bleu car plus de pion a deplacer\n");
	afficherMessageEcran("Fin de la partie pour le joueur bleu car plus de pion a deplacer", 3000);
	retour = 1;
}

return retour;
}



/*
 * Fonction qui enregistre les pions place par le joueur dans le contexte general du jeu
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
 * Fonction qui va demander aux joueur humain de placer ces pions sur le plateau de jeu
 */
int placePion(EColor color, EPiece boardInit[4][10], int joueur){
	
	int i, j;
	SPos position;
	SBox box;
	SGameState SGame;
	
	initialisationContexteJeu(&SGame);
	
	for(i =0; i<4 ;i++){
		for(j=0; j<10;j++){
			 boardInit[i][j] = EPnone;
			 if(joueur == 2){
				 if(color == ECblue) box.content = ECred;
				 else box.content = ECblue;
				 box.piece = EPnone;
				 SGame.board[i][j] = box;
			}
		}
	}
	
	interfaceGraphique(SGame);
	 afficherMessageEcran("Pose ton drapeau", 10);
	
	do{
		position = getPos();
		if(position.line == -2 && position.col == -2) {
			return 1;
		}
	}while((position.line == -1 && position.col == -1) || 9-position.line > 6 || boardInit[9-position.line][9-position.col] != EPnone);
	boardInit[9-position.line][9-position.col] = EPflag;
	box.piece = EPflag;
	box.content = color;
	SGame.board[position.line][position.col] = box;
	interfaceGraphique(SGame);
	afficherMessageEcran("Pose tes 6 bombes", 10);

	for(i=0;i<6;i++){
		do{
			position = getPos();
			if(position.line == -2 && position.col == -2) {
				return 1;
			}
		}while((position.line == -1 && position.col == -1) || 9-position.line > 6 || boardInit[9-position.line][9-position.col] != EPnone);
		boardInit[9-position.line][9-position.col] = EPbomb;
		box.piece = EPbomb;
		box.content = color;
		SGame.board[position.line][position.col] = box;
		interfaceGraphique(SGame);
	}

	afficherMessageEcran("Pose ton espion", 10);
	do{
		position = getPos();
		if(position.line == -2 && position.col == -2) {
				return 1;
		}
	}while((position.line == -1 && position.col == -1) || 9-position.line > 6 || boardInit[9-position.line][9-position.col] != EPnone);
	boardInit[9-position.line][9-position.col] = EPspy;
	box.piece = EPspy;
	box.content = color;
	SGame.board[position.line][position.col] = box;
	interfaceGraphique(SGame);
	
	afficherMessageEcran("Pose tes 8 scoutes", 10);
	for(i=0;i<8;i++){
		do{
			position = getPos();
			if(position.line == -2 && position.col == -2) {
				return 1;
			}
		}while((position.line == -1 && position.col == -1) || 9-position.line > 6 || boardInit[9-position.line][9-position.col] != EPnone);
		boardInit[9-position.line][9-position.col] = EPscout;
		box.piece = EPscout;
		box.content = color;
		SGame.board[position.line][position.col] = box;
		interfaceGraphique(SGame);
	}
	
	afficherMessageEcran("Pose tes 5 demineurs", 10);
	for(i=0;i<5;i++){
		do{
			position = getPos();
			if(position.line == -2 && position.col == -2) {
				return 1;
			}
		}while((position.line == -1 && position.col == -1) || 9-position.line > 6 || boardInit[9-position.line][9-position.col] != EPnone);
		boardInit[9-position.line][9-position.col] = EPminer;
		box.piece = EPminer;
		box.content = color;
		SGame.board[position.line][position.col] = box;
		interfaceGraphique(SGame);
	}
	
	afficherMessageEcran("Pose tes 4 sergeants", 10);
	for(i=0;i<4;i++){
		do{
			position = getPos();
			if(position.line == -2 && position.col == -2) {
				return 1;
			}
		}while((position.line == -1 && position.col == -1) || 9-position.line > 6 || boardInit[9-position.line][9-position.col] != EPnone);
		boardInit[9-position.line][9-position.col] = EPsergeant;
		box.piece = EPsergeant;
		box.content = color;
		SGame.board[position.line][position.col] = box;
		interfaceGraphique(SGame);
	}
	
	afficherMessageEcran("Pose tes 4 lieutenants", 10);
	for(i=0;i<4;i++){
		do{
			position = getPos();
			if(position.line == -2 && position.col == -2) {
				return 1;
			}
		}while((position.line == -1 && position.col == -1) || 9-position.line > 6 || boardInit[9-position.line][9-position.col] != EPnone);
		boardInit[9-position.line][9-position.col] = EPlieutenant;
		box.piece = EPlieutenant;
		box.content = color;
		SGame.board[position.line][position.col] = box;
		interfaceGraphique(SGame);
	}
	
	afficherMessageEcran("Pose tes 4 capitaines", 10);
	for(i=0;i<4;i++){
		do{
			position = getPos();
			if(position.line == -2 && position.col == -2) {
				return 1;
			}
		}while((position.line == -1 && position.col == -1) || 9-position.line > 6 || boardInit[9-position.line][9-position.col] != EPnone);
		boardInit[9-position.line][9-position.col] = EPcaptain;
		box.piece = EPcaptain;
		box.content = color;
		SGame.board[position.line][position.col] = box;
		interfaceGraphique(SGame);
	}
	
	afficherMessageEcran("Pose tes 3 majors", 10);
	for(i=0;i<3;i++){
		do{
			position = getPos();
			if(position.line == -2 && position.col == -2) {
				return 1;
			}
		}while((position.line == -1 && position.col == -1) || 9-position.line > 6 || boardInit[9-position.line][9-position.col] != EPnone);
		boardInit[9-position.line][9-position.col] = EPmajor;
		box.piece = EPmajor;
		box.content = color;
		SGame.board[position.line][position.col] = box;
		interfaceGraphique(SGame);
	}
	
	afficherMessageEcran("Pose tes 2 colonnels", 10);
	for(i=0;i<2;i++){
		do{
			position = getPos();
			if(position.line == -2 && position.col == -2) {
				return 1;
			}
		}while((position.line == -1 && position.col == -1) || 9-position.line > 6 || boardInit[9-position.line][9-position.col] != EPnone);
		boardInit[9-position.line][9-position.col] = EPcolonel;
		box.piece = EPcolonel;
		box.content = color;
		SGame.board[position.line][position.col] = box;
		interfaceGraphique(SGame);
	}
	
	afficherMessageEcran("Pose ton general", 10);
	do{
		position = getPos();
		if(position.line == -2 && position.col == -2) {
				return 1;
			}
	}while((position.line == -1 && position.col == -1) || 9-position.line > 6 || boardInit[9-position.line][9-position.col] != EPnone);
	boardInit[9-position.line][9-position.col] = EPgeneral;
	box.piece = EPgeneral;
	box.content = color;
	SGame.board[position.line][position.col] = box;
	interfaceGraphique(SGame);
		
	afficherMessageEcran("Pose ton marshal", 10);
	do{
		position = getPos();
		if(position.line == -2 && position.col == -2) {
				return 1;
			}
	}while((position.line == -1 && position.col == -1) || 9-position.line > 6 || boardInit[9-position.line][9-position.col] != EPnone);
	boardInit[9-position.line][9-position.col] = EPmarshal;
	box.piece = EPmarshal;
	box.content = color;
	SGame.board[position.line][position.col] = box;
	interfaceGraphique(SGame);		
		
	EPiece rendu[4][10];
	for(i = 0 ; i<4 ; i++){
		for(j=0 ; j<10 ; j++){
			rendu[i][9-j] = boardInit[i][j];
		}
	}
	for(i = 0 ; i<4 ; i++){
		for(j=0 ; j<10 ; j++){
			boardInit[i][j] = rendu[i][j];
		}
	}
}

/*
 * Fonction qui verifie que le mouvement du joueru est possible
 * Si oui on vérifie ensuite si il y a un conflits ou pas
 * Si non on envoie une erreur au joueur
 */
int verificationMouvement(SMove move, SGameState *gameState,EColor color, int joueur, void(*AttackResult1)(SPos, EPiece, SPos, EPiece), void(*AttackResult2)(SPos, EPiece, SPos, EPiece), int nb_IA){
	
	SBox boxStart, boxEnd, newBox;
	int colorAdverse;
	
	//Selectionne la couleur adverse
	if(color == ECred){
		colorAdverse = ECblue;
	}
	else colorAdverse = ECred;
	
		//VERIFICATION QUE LE PION SELECTIONNER EST DANS LE PLATEAU DE JEU
		if(move.start.line>=0 && move.start.line<=9 && move.start.col>=0 && move.start.col<=9){	
			
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
			
			//VERIFICATION QU'ON A PAS DE VA ET VIENS
			if((joueur == 1 && vaEtViensJ1(move, boxStart) == 0) || (joueur == 2 && vaEtViensJ2(move, boxStart) == 0)){
				
				//VERIFICATION QUE LE PION SELECTIONNER CORRESPOND A UN PION DE LA BONNE COULEUR
				if(boxStart.content == color){
		
					//VERIFICATION QUE LE PION SELECTIONNER PEUT ETRE BOUGER
					if(boxStart.piece!=EPnone && boxStart.piece!=EPbomb && boxStart.piece!=EPflag){
					
						//ON VeRIFIE QUE ARRIVE EST BIEN DANS LE JEU
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
										
										   //ON VERIFIE DANS LE CAS D'UN ECLAIREUR SI IL NE SAUTE PAS PAR DESSUS UN LAC OU UN JOUEUR DURANT SONT DEPLACEMENT
										   while((startLine == endLine && startCol != endCol) ||
													(startLine != endLine && startCol == endCol)){
														
											   //CAS PION MONTANT
											   if(startLine < endLine || startCol < endCol){
												   
												   //Cas si le prochain coup arrive a la ligne arrive (mouvement ligne)
												   if(startCol == endCol && startLine != endLine && startLine+1 == endLine && gameState->board[startLine+1][endCol].content == colorAdverse) {
														startLine++;
													}
												   
												   //Cas si le prochain mouvement n'est pas une case vide (mouvement ligne)
													else if(startCol == endCol && startLine != endLine && gameState->board[startLine+1][endCol].content !=ECnone){
														fprintf(fichier,"\n\nERREUR DEPLACEMENT\nJoueur couleur : %i\nPosition origine : ligne = %i et colonne = %i\nPosition arrivee : ligne = %i et colonne = %i\n\n", color, startLine,startCol, endLine, endCol);
														return 1;
													}
													
													//Cas si le prochain mouvement est une case vide (mouvement ligne)
													else if(startCol == endCol && startLine != endLine && gameState->board[startLine+1][endCol].content == ECnone){
														startLine++;
													}
													 
													 //Cas si le prochain coup arrive a la ligne arrive (mouvement colonne)
													if (startLine == endLine && startCol != endCol && startCol+1 == endCol && gameState->board[startLine][startCol+1].content == colorAdverse) {
														startCol++;
													}
													 
													 //Cas si le prochain mouvement n'est pas une case vide (mouvement colonne)
													else if (startLine == endLine && startCol != endCol && gameState->board[move.start.line][startCol+1].content !=ECnone){
														 fprintf(fichier,"\n\nERREUR DEPLACEMENT\nJoueur couleur : %i\nPosition origine : ligne = %i et colonne = %i\nPosition arrivee : ligne = %i et colonne = %i\n\n", color, startLine,startCol, endLine, endCol);
														 return 1;
													}
													
													//Cas si le prochain mouvement est une case vide (mouvement colonne)
													else if (startLine == endLine && startCol != endCol && gameState->board[move.start.line][startCol+1].content ==ECnone){
														 startCol++;
													}
												}
												//CAS PION DESCENDANT
												else if(startLine > endLine || startCol > endCol){
													
													  //Cas si le prochain coup arrive a la ligne arrive (mouvement ligne)
													if(startCol == endCol && startLine != endLine && startLine-1 == endLine && gameState->board[startLine-1][endCol].content == colorAdverse) {
														startLine--;
													}
													
													 //Cas si le prochain mouvement n'est pas une case vide (mouvement ligne)
													else if(startCol == endCol && startLine != endLine && gameState->board[startLine-1][endCol].content !=ECnone){
														fprintf(fichier,"\n\nERREUR DEPLACEMENT\nJoueur couleur : %i\nPosition origine : ligne = %i et colonne = %i\nPosition arrivee : ligne = %i et colonne = %i\n\n", color, startLine,startCol, endLine, endCol);
														return 1;
													}
													
													//Cas si le prochain mouvement est une case vide (mouvement ligne)
													else if(startCol == endCol && startLine != endLine && gameState->board[startLine-1][endCol].content == ECnone){
														startLine--;
													}
													 
													  //Cas si le prochain coup arrive a la ligne arrive (mouvement colonne)
													if (startLine == endLine && startCol != endCol && startCol-1 == endCol && gameState->board[startLine][startCol-1].content == colorAdverse) {
														startCol--;
													}
													  //Cas si le prochain mouvement n'est pas une case vide (mouvement colonne)
													else if (startLine == endLine && startCol != endCol && gameState->board[move.start.line][startCol-1].content !=ECnone){
														 fprintf(fichier,"\n\nERREUR DEPLACEMENT\nJoueur couleur : %i\nPosition origine : ligne = %i et colonne = %i\nPosition arrivee : ligne = %i et colonne = %i\n\n", color, startLine,startCol, endLine, endCol);
														 return 1;
													}
													
													//Cas si le prochain mouvement est une case vide (mouvement colonne)
													else if (startLine == endLine && startCol != endCol && gameState->board[move.start.line][startCol-1].content ==ECnone){
														 startCol--;
													}
												}
											}
										printf("Deplacement realise\n");
										
										//ON VeRIFIE SI LE DePLACEMENT ENTRAINE UNE ATTAQUE OU PAS
										if(boxEnd.content==colorAdverse){
											fprintf(fichier,"\n\nPosition origine : ligne = %i et colonne = %i\nPosition arrivee : ligne = %i et colonne = %i",startLine,startCol, endLine, endCol);
											return attaque(move, gameState,color, joueur, AttackResult1, AttackResult2, nb_IA);
										}
										else{
											//ON MODIFIE LE CONTEXTE DE JEU AVEC LE DEPLACEMENT
											gameState->board[move.end.line][move.end.col] = gameState->board[move.start.line][move.start.col];
											newBox.content = ECnone;
											newBox.piece = EPnone;
											gameState->board[move.start.line][move.start.col] = newBox;
										}
										fprintf(fichier,"\n\nPosition origine : ligne = %i et colonne = %i\nPosition arrivee : ligne = %i et colonne = %i\n\n",startLine,startCol, endLine, endCol);
										return 0;
									}   
								}
								//CAS D'UNE PIeCE QUI BOUGE ET QUI N'EST PAS UN ECLAIREUR
								else{
								
									if((move.start.line == move.end.line && move.start.col == move.end.col+1) ||
									   (move.start.line == move.end.line && move.start.col == move.end.col-1) ||
									   (move.start.line == move.end.line+1 && move.start.col == move.end.col) ||
									   (move.start.line == move.end.line-1 && move.start.col == move.end.col)){
										   
										printf("Deplacement realise\n");
										//ON VERIFIE SI LE DEPLACEMENT ENTRAIN UNE ATTAQUE OU PAS
										if(boxEnd.content==colorAdverse){
											fprintf(fichier,"\n\nPosition origine : ligne = %i et colonne = %i\nPosition arrivee : ligne = %i et colonne = %i",move.start.line,move.start.col, move.end.line, move.end.col);
											return attaque(move, gameState,color, joueur, AttackResult1, AttackResult2, nb_IA);
										}
										else{
											gameState->board[move.end.line][move.end.col] = gameState->board[move.start.line][move.start.col];
											newBox.content = ECnone;
											newBox.piece = EPnone;
											gameState->board[move.start.line][move.start.col] = newBox;
										}
										fprintf(fichier,"\n\nPosition origine : ligne = %i et colonne = %i\nPosition arrivee : ligne = %i et colonne = %i\n\n",move.start.line,move.start.col, move.end.line, move.end.col);
										return 0;
									}
								}
							
							}
							fprintf(fichier,"\n\nERREUR DEPLACEMENT\nJoueur couleur : %i\nMouvement arrive lac ou même couleur\nPosition origine : ligne = %i et colonne = %i\nPosition arrivee : ligne = %i et colonne = %i\n\n", color, move.start.line,move.start.col, move.end.line, move.end.col);
						}
						fprintf(fichier,"\n\nERREUR DEPLACEMENT\nJoueur couleur : %i\nMouvement arrive hors plateau\nPosition origine : ligne = %i et colonne = %i\nPosition arrivee : ligne = %i et colonne = %i\n\n", color, move.start.line,move.start.col, move.end.line, move.end.col);			
					}
					fprintf(fichier,"\n\nERREUR DEPLACEMENT\nJoueur couleur : %i\nPion qui ne bouge pas\nPosition origine : ligne = %i et colonne = %i\nPosition arrivee : ligne = %i et colonne = %i\n\n", color, move.start.line,move.start.col, move.end.line, move.end.col);
				}
				fprintf(fichier,"\n\nERREUR DEPLACEMENT\nJoueur couleur : %i\nCase depart mauvaise couleur\nPosition origine : ligne = %i et colonne = %i\nPosition arrivee : ligne = %i et colonne = %i\n\n", color, move.start.line,move.start.col, move.end.line, move.end.col);
			}
			fprintf(fichier,"\n\nERREUR DEPLACEMENT\nJoueur couleur : %i\nMouvement va et viens\nPosition origine : ligne = %i et colonne = %i\nPosition arrivee : ligne = %i et colonne = %i\n\n", color, move.start.line,move.start.col, move.end.line, move.end.col);
		}
		fprintf(fichier,"\n\nERREUR DEPLACEMENT\nJoueur couleur : %i\nMouvement depart hors plateau\nPosition origine : ligne = %i et colonne = %i\nPosition arrivee : ligne = %i et colonne = %i\n\n", color, move.start.line,move.start.col, move.end.line, move.end.col);
	return 1;
}

/*
 * Fonction qui envoie au joueur qu'il se passe un combat
 * Verifie le resultatt de l'attaque et modifie le contexte selon le resultat obtenue
 * Renvoie 2 lorsque le drapeau ennemie a ete detruit
 */
int attaque(SMove move, SGameState *gameState,EColor color, int joueur, void(*AttackResultJ1)(SPos, EPiece, SPos, EPiece), void(*AttackResultJ2)(SPos, EPiece, SPos, EPiece), int nbr_IA){
	SBox attaquant, attaquer, newBox;
	SMove moveJ2, moveJ1;
	
	//ON DEFINIE QUI ATTAQUE ET QUI EST ATTAQUE
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
	
	fprintf(fichier,"\n\nATTAQUE DU JOUEUR %i\n", joueur);
	
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
	
	//On affiche dans le cas de joueur humain les pions qui s'attaque
	if(nbr_IA == 1 && joueur == 1 || nbr_IA == 0){
		afficheAttaque(color, *gameState, move, nbr_IA);
	}
						
	//CAS OU LES 2 PIECES SONT DE FORCE EQUIVALENTE
	if(attaquant.piece == attaquer.piece){
		
		fprintf(fichier,"Attaque de 2 forces equivalente\n");
		afficherMessageEcran("Attaque de 2 forces equivalente", 1000);
		
		//On incremente les tableaux de perte
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
	 * CAS OU LA PIECE ATTAQUANT EST PLUS FORTE QUE LA PIeCE ATTAQUeE
	 * OU CAS OU LA PIECE ATTAQUANT EST UN DeMINENEUR ET LA PIECE ATTAQUeE UNE BOMBE
	 * OU CAS OU LA PIECE ATTAQUANT EST UNE ESPIONNE ET LA PIECE ATTAQUeE UN MARCHAL
	 */
	else if((attaquant.piece > attaquer.piece && attaquer.piece !=EPbomb) || 
	(attaquant.piece == EPminer && attaquer.piece == EPbomb) ||
	(attaquant.piece == EPspy && attaquer.piece == EPmarshal)){
		
		fprintf(fichier,"Attaquant plus fort que la piece attaque\n");
		afficherMessageEcran("Attaquant plus fort que la piece attaque", 1000);
		
		//Cas ou c'est le joueur bleu qui perd une piece
		if(attaquant.content == ECred){
			gameState->blueOut[attaquer.piece]++;
		} 
		
		//Cas ou c'est le joueur rouge qui perd une piece
		else {
			gameState->redOut[attaquer.piece]++;
		}
		
		//On met a jour l'état de jeu
		newBox.content = ECnone;
		newBox.piece = EPnone;
		
		gameState->board[move.end.line][move.end.col] = gameState->board[move.start.line][move.start.col];
		gameState->board[move.start.line][move.start.col] = newBox;
		
		return 0;
		
	}
	
	//CAS OU LE DRAPEAU EST ATTAQUe PAR L'ENNEMIE
	if(attaquer.piece == EPflag){
		fprintf(fichier,"Attaque du drapeau\n");
		afficherMessageEcran("Attaque du drapeau", 1000);
		return 2;
	}
	
	//CAS OU L'ATTAQUANT A PERDU
	else{
		fprintf(fichier,"Attaquant moins fort que la piece attaque\n");
		afficherMessageEcran("Attaquant moins fort que la piece attaque", 1000);
		
		//Cas ou c'est le joueur rouge qui perd une piece
		if(attaquant.content == ECred){
			gameState->redOut[attaquant.piece]++;
		} 
		
		//Cas ou c'est le joueur bleu qui perd une piece
		else {
			gameState->blueOut[attaquant.piece]++;
		}
		
		//On met a jour l'état de jeu
		newBox.content = ECnone;
		newBox.piece = EPnone;
		gameState->board[move.start.line][move.start.col] = newBox;
		
		return 0;
	}
}
/*
 * Fonction qui va afficher un gameState durant une duré x
 * Sur ceux gameState on ne voit que c'est propre pion et le pion attaqué ou attaquant
 */
void afficheAttaque(int color, SGameState gameState, SMove move, int nbr_IA){
	
	int i,j;
	SBox box;
	SGameState gamesAttaque;
	
	//Cas ou on affiche quand on est l'attaquant
	for(i =0; i<10;i++){
		for(j=0;j<10;j++){
			if(gameState.board[i][j].content == color){
				gamesAttaque.board[i][j] = gameState.board[i][j];
			}
			else if(i == move.end.line && j == move.end.col){
				gamesAttaque.board[i][j] = gameState.board[i][j];
			}
			else{
				box.content = gameState.board[i][j].content;
				box.piece = EPnone;
				gamesAttaque.board[i][j] = box;
			}
		}
	}
	for(i = 0; i<11;i++){
		gamesAttaque.blueOut[i] = gameState.blueOut[i];
		gamesAttaque.redOut[i] = gameState.redOut[i];
	}
	
	//Attente d'une periode de temps x
	clock_t start,end;
	start=clock();
	interfaceGraphique(gamesAttaque);
	while(((end=clock())-start)<=3*CLOCKS_PER_SEC);
	
	//Cas ou on affiche quand on est l'attaqué
	if(nbr_IA == 0){

		for(i =0; i<10;i++){
			for(j=0;j<10;j++){
				if(gameState.board[i][j].content != color){
					gamesAttaque.board[i][j] = gameState.board[i][j];
				}
				else if(i == move.start.line && j == move.start.col){
					gamesAttaque.board[i][j] = gameState.board[i][j];
				}
				else{
					box.content = gameState.board[i][j].content;
					box.piece = EPnone;
					gamesAttaque.board[i][j] = box;
				}
			}
		}
		
		for(i = 0; i<11;i++){
			gamesAttaque.blueOut[i] = gameState.blueOut[i];
			gamesAttaque.redOut[i] = gameState.redOut[i];
		}
	
		//Attente d'une periode de temps x
		clock_t start,end;
		start=clock();
		interfaceGraphique(gamesAttaque);
		while(((end=clock())-start)<=3*CLOCKS_PER_SEC);
		
	}
}

/*
 * Fonction qui enregistre les erreurs realise par les différent
 * Renvoie 1 lorsque la partie est finie
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
		if(joueur == 1){
			fprintf(fichier,"\n\nFIN PARTIE\nJoueur 1 a perdu son drapeau\nVictoire du joueur 2");
		}
		else{
			fprintf(fichier,"\n\nFIN PARTIE\nJoueur 2 a perdu son drapeau\nVictoire du joueur 1");
		}
		return 1;
	}
	
	//Cas ou le joueur a fait 3 fautes fin partie
	if(erreur_j1 == 3 || erreur_j2 == 3){
		if(erreur_j1 == 3){
			fprintf(fichier,"\n\nFIN PARTIE\nJoueur 1 a fait 3 erreurs\nVictoire du joueur 2");
		}
		else{
			fprintf(fichier,"\n\nFIN PARTIE\nJoueur 2 a fait 3 erreurs\nVictoire du joueur 1");
		}
		return 1;
	}
	return 0;
}


/*
 * Fonction qui va écrire l'état de jeu dans le log.txt
 */
void ecrireFichier(SGameState gameState){
	
	int i,j;
	
	fprintf(fichier,"     |  0  |  1  |  2  |  3  |  4  |  5  |  6  |  7  |  8  |  9  |\n");
	
	for(i=0;i<10;i++){
		fprintf(fichier,"  %i  |",i);
		for(j=0;j<10;j++){
			if(gameState.board[i][j].content == ECred){
				if(gameState.board[i][j].piece <10){
					fprintf(fichier,"[ %iR]|", gameState.board[i][j].piece);
				}
				else fprintf(fichier,"[%iR]|", gameState.board[i][j].piece);
				
			}
			else if(gameState.board[i][j].content == ECblue){
				if(gameState.board[i][j].piece <10){
					fprintf(fichier,"[ %iB]|", gameState.board[i][j].piece);
				}
				else fprintf(fichier,"[%iB]|", gameState.board[i][j].piece);
			}
			else if(gameState.board[i][j].content == EClake){
				fprintf(fichier," [O] |");
			}
			else{
				fprintf(fichier," [X] |");
			}
		}
		fprintf(fichier,"\n",i);
	}
	
	//Nombre de pions restants
	fprintf (fichier, "\nListe des pions rouge dehors :\n");
	for(i = 0; i<11;i++){
		fprintf (fichier, "| %i |",i);
	}
	fprintf (fichier, "\n");
	for(i = 0; i<11;i++){
		fprintf (fichier, "| %i |",gameState.redOut[i]);
	}
	fprintf (fichier, "\n\nListe des pions bleu dehors :\n");
	for(i = 0; i<11;i++){
		fprintf (fichier, "| %i |",i);
	}
	fprintf (fichier, "\n");
	for(i = 0; i<11;i++){
		fprintf (fichier, "| %i |",gameState.blueOut[i]);
	}
}

/*
 * Thread qui tourne lorsque 2 IA joues, permet de fermer la fenetre
 */
static void * quitter(void * p_data){
	
	int continuer = 1;
	SDL_Event event;
	
	while (continuer){
		SDL_Delay(10);
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

	dlclose(j1Lib);
	dlclose(j2Lib);
	fclose(fichier);
	quitter_sdl();
	exit(EXIT_SUCCESS);
	
}
