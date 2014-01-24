/**
 * Projet Stratego, 3ème année de polytech
 * Réalisé par : -Guinard Arnaud
 * 				 -Mouden Benjamin
 * 				 -Evo Brieuc
 * 				 -Chotard François
 */
 
#include <stdio.h>
#include <string.h>


int verificationMouvement(SMove move, SGameState gameState,EColor color);
void choixStrategieIA(int choix,EPiece boardInit[4][10]);
void majContextePerso(const SGameState * const gameState);
int CompareMoves(SMove move1,SMove move2);
int vaEtViens(SMove move, SBox box);
int doitAttaquer(int lineArmy,int colArmy,int lineEnemy,int colEnemy);
int	mouvementAleatoire(int j,int * line,int * col, int startLine, int startCol);
int mouvementSpyMiner(EPiece army,int i,int j);
void afficheConsole(SGameState gameState, EColor joueur1, EColor joueur2);
