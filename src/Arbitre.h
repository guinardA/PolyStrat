#include <stdio.h>
#include <dlfcn.h>
#include <stdlib.h>
/**
 * Projet Stratego, 3ème année de polytech
 * Réalisé par : -Guinard Arnaud
 * 				 -Mouden Benjamin
 * 				 -Evo Brieuc
 * 				 -Chotard François
 */
 
#include <time.h>
#include <pthread.h>
#include <X11/Xlib.h>
#include <time.h>

//Stratego.c
void initialisationContexteJeu(SGameState *gameState);
int verifPionsBouger(SGameState gameState);
void enregistrePion(EPiece boardInit[4][10], SGameState *gameState, EColor color, int joueur);
int verificationMouvement(SMove move, SGameState *gameState,EColor color, int joueur, void(*AttackResult1)(SPos, EPiece, SPos, EPiece), void(*AttackResult2)(SPos, EPiece, SPos, EPiece), int nb_IA);
int placePion(EColor color, EPiece boardInit[4][10], int joueur);
void afficheAttaque(int color, SGameState gameState, SMove move, int nbr_IA);
int attaque(SMove move, SGameState *gameState,EColor color, int joueur, void(*AttackResultJ1)(SPos, EPiece, SPos, EPiece), void(*AttackResultJ2)(SPos, EPiece, SPos, EPiece), int nbr_IA);
int finPartie(int joueur, int flag);
void ecrireFichier(SGameState gameState);
static void * quitter(void * p_data);

//verificationArbitre.c
int verificationNombrePiece(EPiece boardInit[4][10]);
int CompareMoves(SMove move1,SMove move2);
int vaEtViensJ1(SMove move, SBox box);
int vaEtViensJ2(SMove move, SBox box);

//affichageArbitre.c
void afficheConsole(SGameState gameState, EColor joueur1, EColor joueur2);

//dupplicationArbitre.c
SGameState duplicationDuContexteDeJeu(SGameState gameState, EColor color, int joueur);
SGameState duplicationJoueur(SGameState gameState, EColor color, int joueur);
