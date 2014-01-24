/**
 * Projet Stratego, 3ème année de polytech
 * Réalisé par : -Guinard Arnaud
 * 				 -Mouden Benjamin
 * 				 -Evo Brieuc
 * 				 -Chotard François
 */

#include "PolyLib.h"

/*
 * Fonction qui recopie l'état du jeu général en affichant que c'est propre pions en cachant ceux de l'adversaire (pour IA)
 */
SGameState duplicationDuContexteDeJeu(SGameState gameState, EColor color, int joueur){
	
	SGameState gameStatePerso;
	SBox box;
	int i,j, m = 9;
	
	//Recopie pour le joueur 1
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
	
	//Recopie pour le joueur 2
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
	
	//On copies les pions sortie
	for(i=0;i<11;i++){
		gameStatePerso.redOut[i] = gameState.redOut[i];
		gameStatePerso.blueOut[i] = gameState.blueOut[i];
	}
	
	return gameStatePerso;
}

/*
 * Fonction qui recopie l'état du jeu général en affichant que c'est propre pions en cachant ceux de l'adversaire (pour humain)
 */
SGameState duplicationJoueur(SGameState gameState, EColor color, int joueur){
	SGameState gameStatePerso;
	SBox box;
	int i,j, m = 9;
	
	//Recopie pour le joueur 1
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
	
	//Recopie pour le joueur 2
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
