/**
 * Projet Stratego, 3ème année de polytech
 * Réalisé par : -Guinard Arnaud
 * 				 -Mouden Benjamin
 * 				 -Evo Brieuc
 * 				 -Chotard François
 */

#include "PolyLib.h"
#include <stdio.h>

/*
 * FONCTION QUI AFFICHE DANS EN CONSOLE LE CONTEXTE DE JEU DANS LE TERMINAL
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
