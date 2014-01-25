/**
 * Projet Stratego, 3ème année de polytech
 * Réalisé par : -Guinard Arnaud
 * 				 -Mouden Benjamin
 * 				 -Evo Brieuc
 * 				 -Chotard François
 */

#include "PolyLib.h"
#include "IA.h"



EColor couleur,couleurAdverse;

//utilisés pour le "va et viens"
SMove listMvt[3];
SBox listBox[3];
int repetitionMvt;

//attaque à 0 quand le mouvement réalisé précédement n'est pas une attaque, sinon 1
int penalite=0,attaque=0;

//tableau des positions des pieces de l'ia
SPos positionPiece[40];

// Etat du jeu propre à l'ia
SGameState contextPerso;

/*Ensemble des fonctions communes a toutes les groupes
---------------------------------------------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------------------------------------------
*/

//Renvoie le nom "Polylib" de l'ia à l'arbitre
void InitLibrary(char name[50])
{
	printf("Initialisation des variables de la librairie 1\n");
	strcpy(name,"Polylib");
}

// réponse de début de jeu
void StartMatch()
{	
	printf("Début du match\n");
}

//initialisation d'une partie
void StartGame(const EColor color,EPiece boardInit[4][10])
{
	
	printf("Lancement d'une partie\n");
	printf("Placement des pions sur le terrain\n");

	//Définition des couleurs des joueurs
	couleur = color;
	if(couleur == ECred){
		couleurAdverse = ECblue;	
	}
	else{
		couleurAdverse = ECred;
	}

	//Initialisation du srand pour la stratégie de départ de l'IA	
	srand(time(NULL));	

	//Choix stratégie de placement de pion
	//Actuellement une seule strategie de départ
	int choix = (int)rand()%1;
	choixStrategieIA(choix,boardInit);

	//Initialisation du contexte de jeu interne
	SBox box;
	int i,j;
	for(i=0; i<4 ; i++){
		for(j=0 ; j<10 ; j++){
			box.content = couleur;
			contextPerso.board[i][j] = box;
		}
	}
	
	for(i=4; i<6 ; i++){
		for(j=0 ; j<10 ; j++){
			if((i==4 && j==2) || (i==4 && j==3) || (i==4 && j==6) || (i==4 && j==7) ||(i==5 && j==2) || (i==5 && j==3) || (i==5 && j==6) || (i==5 && j==7)){
				box.content = EClake;
				box.piece = EPnone;
				contextPerso.board[i][j] = box;
			} 
			else{
				box.content = ECnone;
				box.piece = EPnone;
				contextPerso.board[i][j] = box;
			}
		}
	}
	
	//Au démarrage on sait qu'il y a 4 lignes de pions adverses mais on ne connait pas leurs forces
	for(i=6; i<10 ; i++){
		for(j=0 ; j<10 ; j++){
			box.content = couleurAdverse;
			box.piece = EPnone;
			contextPerso.board[i][j] = box;
		}
	}
	
	//Nombre de pions détruit
	for(i= 0; i<11 ; i++){
		contextPerso.redOut[i] = 0;
		contextPerso.blueOut[i] = 0;
	}
}

// réponse de fin d'une partie
void EndGame()
{
	printf("EndGame\n");
	//On remet les pénalités a 0
	penalite = 0;
}

// réponse de fin de jeu
void EndMatch()
{
	printf("EndMatch\n");
}

/*
Fonction centrale de l'ia: Détermine le prochain coup à jouer 
paramètre: contexte du jeu selon l'arbitre, ne peut être modifié
retourne un SMove : position de départ + position d'arrivée
*/
SMove NextMove(const SGameState * const gameState)
{
	
	printf("Deplacement d'un pion\n");

	//Maj du contextePerso
	majContextePerso(gameState);

	//initialisation de la variable de retour
	SMove move; 


	//on utilise numPiece pour ne pas séléctionner toujours la même pièce quand un mouvement n'est pas valide
	int line, col, i, j,numPiece=0;
	
	do
	{

		move.end.line = -1;//on initialise à une fausse valeur pour une vérification

		//On sélectionne la pièce à bouger (classées par ordre d'importance)
		//Cette boucle ne s'intéresse qu'au cas où une piece de l'ia est à côté d'une pièce adverse
		for(i=numPiece;i<33;i++)
		{ 
			//on récupère l'emplacement de la pièce
			line = positionPiece[i].line;
			col = positionPiece[i].col;
			
			//si line == -1 : piece tuée
			if ( (line >=0))
			{ 
				
				//si notre pièce est à côté d'un adversaire
				if( (line<9) && (contextPerso.board[line+1][col].content == couleurAdverse) )
				{	
					//vérifie si l'attaque doit se faire
					if(doitAttaquer(line,col,line+1,col) == 1)
					{
						move.end.line = line+1;
						move.end.col = col;

						numPiece= i+1;// si le mouvement n'est pas validé, on commencera le choix de pièce en danger à la pièce suivante
						break;// sort de la boucle de choix de pièce
					}
					//sinon vérifie si on peut fuir (0: on ne peut pas), si on peu fuir le mouvement est enregistré par la fonction fuite
					else if (fuite(line,col, &(move.end.line), &(move.end.col)) == 1)
					{
						
						numPiece= i+1;
						break;
					}
				}			
				else if( (line>0) && (contextPerso.board[line-1][col].content == couleurAdverse) )
				{
					if(doitAttaquer(line,col,line-1,col) == 1)
					{
						move.end.line = line-1;
						move.end.col = col;

						numPiece=i+1;
						break;
					}
					else if (fuite(line,col, &(move.end.line), &(move.end.col)) == 1)
					{
						numPiece=i+1;
						break;
					}
				}
				else if( (col<9) && (contextPerso.board[line][col+1].content == couleurAdverse) )
				{
					if(doitAttaquer(line,col,line,col+1) == 1)
					{
						move.end.line = line;
						move.end.col = col+1;
					
						numPiece=i+1;
						break;
					}
					else if (fuite(line,col, &(move.end.line), &(move.end.col)) == 1)
					{
						numPiece=i+1;
						break;
					}
				}
				else if( (col>0) && (contextPerso.board[line][col-1].content == couleurAdverse) )
				{
					if(doitAttaquer(line,col,line,col-1) == 1)
					{
						move.end.line = line;
						move.end.col = col-1;

						numPiece=i+1;
						break;
					}
					else if (fuite(line,col, &(move.end.line), &(move.end.col)) == 1)
					{
						numPiece=i+1;
						break;
					}
				}
			}
		}

		//si label == 1 : mouvement déjà décidé; sinon mouvement aléatoire
		//si move.end.line est supérieur à 0, un mouvement de fuite a été décidé
		if(move.end.line < 0)
		{
			do
			{
				i = (int)rand()%33;// choix aléatoire de la pièce
				j = (int)rand()%20;

				// on récupère la position de la pièce
				line = positionPiece[i].line;
				col = positionPiece[i].col;

			}while(line <0);// on ne choisit que des pièces encore en jeu
	
			//Position d'arrivée : si line ==-1 on ne veut pas faire de mouvement, les valeurs move.end sont correctement mises à jour (envoi des adresses)
			mouvementAleatoire(j,&(move.end.line),&(move.end.col),line,col);

		}

		//Position de départ
		move.start.line = line;
		move.start.col = col;

	}
	while(verificationMouvement(move, *gameState, couleur)!=0);// vérfication du mouvement

	//Cas où le mouvement va entrainer une attaque
	if(gameState->board[move.end.line][move.end.col].content == couleurAdverse)
	{
		attaque = 1;
	}
	//Cas ou aucune attaque ne va se passer
	else
	{
		attaque = 0;

		//On met a jour le contexte perso
		contextPerso.board[move.end.line][move.end.col].content = contextPerso.board[line][col].content;
		contextPerso.board[move.end.line][move.end.col].piece = contextPerso.board[line][col].piece;
		contextPerso.board[line][col].content = ECnone;
		contextPerso.board[line][col].piece = EPnone;
			
		//On a mis a jour la position du pion déplacé
		positionPiece[i].line = move.end.line;
		positionPiece[i].col = move.end.col;
	}

	afficheConsole(contextPerso, couleur, couleurAdverse);
	return move;
}

/*
En cas d'attaque, l'arbitre nous renvoi les informations la concernant
Cette fonction enregistre le résultat dans le contexte de jeu de l'ia 
*/
void AttackResult(SPos armyPos,EPiece armyPiece,SPos enemyPos,EPiece enemyPiece)
{
	//si attaque = 1 alors l'ia est l'attaquant
	
	printf("AttackResult IA\n");

	EPiece attaquant; // force de l'attaquant
	EPiece attaquer; // force de l'attaqué

	SBox newBox;//nouvelle case vide pour mettre à jour l'état de jeu interne

	int numPiece;//numéro de la pièce de l'ia

	// recherche de la pièce de l'ia dans positionPiece	
	for(numPiece=0;numPiece<33;numPiece++)
	{
		if(positionPiece[numPiece].line == armyPos.line && positionPiece[numPiece].col == armyPos.col)
		{
			break;
		}
	}
 
	//définit qui attaque et qui défend
	if(attaque == 1)
	{
		attaquant = armyPiece;
		attaquer = enemyPiece;
	}
	else
	{
		attaquant = enemyPiece;
		attaquer = armyPiece;
	}
			
	//CAS OU LES 2 PIÈCES SONT DE FORCE ÉQUIVALENTE
	if(attaquant == attaquer){

		//maj du nombre de pièces éliminées.
		contextPerso.redOut[attaquant]++;
		contextPerso.blueOut[attaquer]++;	

		//initialisation d'une case vide
		newBox.content = ECnone;
		newBox.piece = EPnone;

		//remplacement des cases du contexte de jeu par des cases vides
		contextPerso.board[armyPos.line][armyPos.col] = newBox;
		contextPerso.board[enemyPos.line][enemyPos.col] = newBox;
		
		positionPiece[numPiece].line = -1;//piece éliminée du jeu
	}
	
	/*
	 * CAS OU LA PIECE ATTAQUANT EST PLUS FORTE QUE LA PIÈCE ATTAQUÉE
	 * OU CAS OU LA PIECE ATTAQUANT EST UN DÉMINEUR ET LA PIECE ATTAQUÉE UNE BOMBE
	 * OU CAS OU LA PIECE ATTAQUANT EST UNE ESPIONNE ET LA PIECE ATTAQUÉE UN MARCHAL
	 */
	else if((attaquant > attaquer && attaquer!=EPbomb) || (attaquant == EPminer && attaquer == EPbomb) || (attaquant == EPspy && attaquer == EPmarshal))
	{

		//maj du nombre de pièces éliminées
		if((couleur == ECred && attaque == 1) || ( couleur == ECblue && attaque == 0)){
			contextPerso.blueOut[attaquer]++;
		} else {
			contextPerso.redOut[attaquer]++;
		}

		//initialisation d'une case vide
		newBox.content = ECnone;
		newBox.piece = EPnone;
	
		//l'ia attaque
		if(attaque == 1)
		{
			//maj de déplacement pour positionPiece
			positionPiece[numPiece].line = enemyPos.line;
			positionPiece[numPiece].col = enemyPos.col;		

			//déplacement pour le contexte perso
			contextPerso.board[enemyPos.line][enemyPos.col] = contextPerso.board[armyPos.line][armyPos.col];
			contextPerso.board[armyPos.line][armyPos.col] = newBox;//case libérée par le déplacement
		}
		//l'adversaire nous attaque
		else
		{
			//perte de notre piece
			positionPiece[numPiece].line = -1;

			contextPerso.board[enemyPos.line][enemyPos.col] = newBox;//la case de l'adversaire est libérée par son déplacment

			//création d'une box correspondant à l'adversaire
			newBox.content = couleurAdverse;
			newBox.piece = attaquant;

			//l'adversaire prend la place de notre pièce, on a maintenant sa valeur
			contextPerso.board[armyPos.line][armyPos.col] = newBox;	
		}
		
		
	}	
	//Cas où la pièce en défense gagne
	else
	{

		//maj du nombre de pièces éliminées
		if((couleur == ECred && attaque == 0) || ( couleur == ECblue && attaque == 1)){
			contextPerso.blueOut[attaquant]++;
		} else {
			contextPerso.redOut[attaquant]++;
		}

		//Initialisation d'une case vide
		newBox.content = ECnone;
		newBox.piece = EPnone;

		//l'ia attaque
		if(attaque == 1)
		{
			positionPiece[numPiece].line = -1;//pièce éliminée

			contextPerso.board[armyPos.line][armyPos.col] = newBox;//case libérée par le déplacement

			//création d'une box correspondant à l'adversaire
			newBox.content = couleurAdverse;
			newBox.piece = attaquant;

			//on a maintenant la valeur de l'adversaire
			contextPerso.board[enemyPos.line][enemyPos.col] = newBox;
		}
		//l'adversaire nous attaque
		else
		{
			// l'emplacement de l'adversaire est maintenant vide
			contextPerso.board[enemyPos.line][enemyPos.col] = newBox;
		}

	}	

	attaque = 0;//remise à 0 de la variable, au cas où l'adversaire attaque au prochain coup
}

//réponse en cas de pénalitée
void Penalty()
{
	printf("Penalty\n");
	penalite++;//Maj du nombre de pénalitées
	printf("Nombre de pénalité : %i\n",penalite);
}

/*Ensemble des fonctions liées à l'IA
---------------------------------------------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------------------------------------------
*/

/*
 * Fonction qui verifie que les pions déplacées par le joueur est correcte,
 * pour cela on verifie que tous les types de pions sont presents et qu'il y a le bon nombre de chaque type
 */
int vaEtViens(SMove move, SBox box){
 switch (repetitionMvt)
    {
		//Cas ou on aucune répétition
        case 0: 
				listMvt[repetitionMvt] = move;
				listBox[repetitionMvt] = box;
                repetitionMvt++;
                break;
                
		//Cas ou on 1 répétition, on vérifie que la pièce bougé n'est pas déjà enregistré
        case 1:
                if( box.piece == listBox[0].piece ){
                    listMvt[repetitionMvt] = move;
                    listBox[repetitionMvt] = box;
                    repetitionMvt++;
                }
                else{ 
					repetitionMvt = 1;
					listMvt[0] = move;
                    listBox[0] = box; }
                break;
          
        //Cas ou on 2 répétitions, on vérifie que la pièce bougé n'est pas déjà enregistré et qu'elle pas deja réalisé ce mouvement
        case 2:
            if( (box.piece == listBox[0].piece) && (CompareMoves(move,listMvt[0])==1) ){
                listMvt[repetitionMvt] = move;
                listBox[repetitionMvt] = box;
                repetitionMvt++;
            }
            else{ 
					repetitionMvt = 1;
					listMvt[0] = move;
                    listBox[0] = box; }
            break;
         
         //Cas ou on 3 répétitions, on vérifie que la pièce bougé n'est pas déjà enregistré et qu'elle pas deja réalisé ce mouvement
         case 3:
            if( (box.piece == listBox[0].piece) && (CompareMoves(move,listMvt[1])==1) ){
                repetitionMvt++;
            }
             else{ 
					repetitionMvt = 1;
					listMvt[0] = move;
                    listBox[0] = box; }
            break;
        default:
            repetitionMvt = 0;
            break;
    }
    
    //Si on a 4 répétitions alors on renvoie 1
    if(repetitionMvt == 4){
		return 1;
	}
	else{
		return 0;
	}
}

/*
 * Fonction qui va comparer 2 mouvements donnés en paramètres
 * Renvoie 1 quand ils sont identiques sinon 0
 */
int CompareMoves(SMove move1,SMove move2){
    if( move1.start.line == move2.start.line){
        if( move1.start.col == move2.start.col){
            if( move1.end.line == move2.end.line){
                if( move1.end.col == move2.end.col){
                    return 1;
                }
            }
        }
	}
	return 0;
}

/*
 * Fonction qui vérifie si le mouvement est possible ou pas.
 * Renvoie 0 si le mouvement est possible ou 1 si il est pas possible
 */
int verificationMouvement(SMove move, SGameState gameState,EColor color){
	
	
	SBox boxStart, boxEnd, newBox;
	
	if(move.start.line>=0 && move.start.line<=9 && move.start.col>=0 && move.start.col<=9){	
		
			boxStart = gameState.board[move.start.line][move.start.col];
			
			if(vaEtViens(move, boxStart) == 0){
				
			int t;
			//VERIFICATION QUE LE PION SELECTIONNER CORRESPOND A UN PION DE LA BONNE COULEUR
			if(boxStart.content == color){
				
				//VERIFICATION QUE LE PION SELECTIONNER PEUT ETRE BOUGER
				if(boxStart.piece!=EPnone && boxStart.piece!=EPbomb && boxStart.piece!=EPflag){
					if(move.end.line>=0 && move.end.line<=9 && move.end.col>=0 && move.end.col<=9){
						
						boxEnd = gameState.board[move.end.line][move.end.col];
			
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
											   if(startCol == endCol && startLine != endLine && startLine+1 == endLine && gameState.board[startLine+1][endCol].content == couleurAdverse) {
												   startLine++;
												}
											   
											   //Cas si le prochain mouvement n'est pas une case vide (mouvement ligne)
											   else if(startCol == endCol && startLine != endLine && gameState.board[startLine+1][endCol].content !=ECnone){
													return 1;
												}
												
												//Cas si le prochain mouvement est une case vide (mouvement ligne)
												else if(startCol == endCol && startLine != endLine && gameState.board[startLine+1][endCol].content == ECnone){
													startLine++;
												}
												 
												 //Cas si le prochain coup arrive a la ligne arrivé (mouvement col)
											   if(startLine == endLine && startCol != endCol && startCol+1 == endCol && gameState.board[startLine][startCol+1].content == couleurAdverse) {
												   startCol++;
												}
												 
												//Cas si le prochain mouvement n'est pas une case vide (mouvement colonne)
												else if (startLine == endLine && startCol != endCol && gameState.board[move.start.line][startCol+1].content != ECnone){
													 return 1;
												}
												
												//Cas si le prochain mouvement est une case vide (mouvement colonne)
												else if (startLine == endLine && startCol != endCol && gameState.board[move.start.line][startCol+1].content == ECnone){
													 startCol++;
												}
											}
											//CAS PION MONTANT
										   else if(startLine > endLine || startCol > endCol){
										   
											   //Cas si le prochain coup arrive a la ligne arrivé (mouvement ligne)
											   if(startCol == endCol && startLine != endLine && startLine-1 == endLine && gameState.board[startLine-1][endCol].content == couleurAdverse) {
												   startLine--;
												}
											   
											   //Cas si le prochain mouvement n'est pas une case vide (mouvement ligne)
											   else if(startCol == endCol && startLine != endLine && gameState.board[startLine-1][endCol].content !=ECnone){
													return 1;
												}
												
												//Cas si le prochain mouvement est une case vide (mouvement ligne)
												else if(startCol == endCol && startLine != endLine && gameState.board[startLine-1][endCol].content == ECnone){
													startLine--;
												}
												 
												 //Cas si le prochain coup arrive a la ligne arrivé (mouvement col)
											   if(startLine == endLine && startCol != endCol && startCol-1 == endCol && gameState.board[startLine][startCol-1].content == couleurAdverse) {
												   startCol--;
												}
												 
												//Cas si le prochain mouvement n'est pas une case vide (mouvement colonne)
												else if (startLine == endLine && startCol != endCol && gameState.board[move.start.line][startCol-1].content != ECnone){
													 return 1;
												}
												
												//Cas si le prochain mouvement est une case vide (mouvement colonne)
												else if (startLine == endLine && startCol != endCol && gameState.board[move.start.line][startCol-1].content == ECnone){
													 startCol--;
												}
											}
										}
									return 0;
								}   
							}
							
							//CAS D'UNE PIÈCE QUI BOUGE ET QUI N'EST PAS UN ÉCLAIREUR
							else{
								//Cas ou la piece peut bouger
								if((move.start.line == move.end.line && move.start.col+1 == move.end.col) ||
								   (move.start.line == move.end.line && move.start.col-1 == move.end.col) ||
								   (move.start.line+1 == move.end.line && move.start.col == move.end.col) ||
								   (move.start.line-1 == move.end.line && move.start.col == move.end.col)){
									   
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
 *Initialisation des pièces de l'ia
 */
void choixStrategieIA(int choix,EPiece boardInit[4][10])
{
	if(choix == 0){
		//4ème ligne
		boardInit[0][0] = EPsergeant;//4
		boardInit[0][1] = EPscout;//2
		boardInit[0][2] = EPminer;//3
		boardInit[0][3] = EPbomb;
		boardInit[0][4] = EPsergeant;//4
		boardInit[0][5] = EPminer;//3
		boardInit[0][6] = EPminer;//3
		boardInit[0][7] = EPbomb;
		boardInit[0][8] = EPflag;
		boardInit[0][9] = EPbomb;
		//3ème ligne
		boardInit[1][0] = EPbomb;
		boardInit[1][1] = EPcaptain;//6
		boardInit[1][2] = EPspy;
		boardInit[1][3] = EPmajor;//7
		boardInit[1][4] = EPlieutenant;//5
		boardInit[1][5] = EPscout;//2
		boardInit[1][6] = EPcaptain;//6
		boardInit[1][7] = EPlieutenant;//5
		boardInit[1][8] = EPbomb;
		boardInit[1][9] = EPsergeant;//4
		//2ème ligne
		boardInit[2][0] = EPminer;//3
		boardInit[2][1] = EPscout;//2
		boardInit[2][2] = EPcolonel;//8
		boardInit[2][3] = EPmajor;//7
		boardInit[2][4] = EPbomb;
		boardInit[2][5] = EPlieutenant;//5
		boardInit[2][6] = EPmarshal;//10
		boardInit[2][7] = EPmajor;//7
		boardInit[2][8] = EPlieutenant;//5
		boardInit[2][9] = EPcolonel;//8
		//1ère ligne
		boardInit[3][0] = EPgeneral;//9
		boardInit[3][1] = EPcaptain;//6
		boardInit[3][2] = EPscout;//2
		boardInit[3][3] = EPsergeant;//4
		boardInit[3][4] = EPscout;//2
		boardInit[3][5] = EPscout;//2
		boardInit[3][6] = EPscout;//2
		boardInit[3][7] = EPminer;//3
		boardInit[3][8] = EPcaptain;//6
		boardInit[3][9] = EPscout;//2

		// on enregistre les pièces dans l'ordre d'importance pour la fonction nextMove

		//2 à 2, les emplacements des pièces selon l'ordre d'importance choisit
		int tab[80] = {1,2,3,7,2,0,0,6,0,5,0,2,2,6,2,7,2,3,1,3,3,8,3,1,1,6,1,1,2,9,2,2,3,0,3,9,3,6,3,5,3,4,3,2,2,1,1,5,0,1,3,3,1,9,0,4,0,0,2,8,2,5,1,7,1,4,2,4,1,8,1,0,0,9,0,7,0,3,0,8};
		
		int j = 0,i;
		for(i=0;i<40;i++)
		{
			positionPiece[i].line = tab[j];
			j++;
			positionPiece[i].col = tab[j];
			j++;
		}	
	}
}


//mise à jour du contexte perso avec le gameState envoyé par l'arbitre
void majContextePerso(const SGameState * const gameState)
{
	int i,j;

	//balayage des lignes
	for(i=0;i<10;i++)
	{
		//balayage des colonnes
		for(j=0;j<10;j++)
		{
			//si le contexte interne ne correspond pas avec celui de l'arbitre
			if(contextPerso.board[i][j].content != gameState->board[i][j].content)
			{
				//si le contexte interne indique un adversaire connu : on veut garder sa valeur (le cas des scouts n'est pas traité au vu de leurs déplacements possibles)
				 if( contextPerso.board[i][j].content==couleurAdverse && contextPerso.board[i][j].piece!=EPnone && contextPerso.board[i][j].piece!=EPscout)
				 {
				 	//On vérifie si la pièce va à la ligne suivante
				 	if (i<9 && contextPerso.board[i+1][j].content != gameState->board[i+1][j].content)
				 	{
				 		contextPerso.board[i+1][j] = contextPerso.board[i][j];// déplacement de la pièce dans le contexte interne
				 		contextPerso.board[i][j] = gameState->board[i][j];//libère la case après déplacement
				 	}
				 	//On vérifie si la pièce va à la colonne suivante
				 	else if (j<9 && contextPerso.board[i][j+1].content != gameState->board[i][j+1].content)
				 	{
				 		contextPerso.board[i][j+1] = contextPerso.board[i][j];
				 		contextPerso.board[i][j] = gameState->board[i][j];
				 	}
				 }
				 //si le contexte de l'arbitre indique un adversaire 
				 else if (gameState->board[i][j].content == couleurAdverse)
				 {
				 	//On vérifie si la pièce vient de la ligne suivante
				 	if (i<9 && contextPerso.board[i+1][j].content != gameState->board[i+1][j].content)
				 	{
				 		contextPerso.board[i][j] = contextPerso.board[i+1][j];// déplacement de la pièce dans le contexte interne
				 		contextPerso.board[i+1][j] = gameState->board[i+1][j];//libère la case après déplacement
				 	}
				 	//On vérifie si la pièce vient de la colonne suivante
				 	else if (j<9 && contextPerso.board[i][j+1].content != gameState->board[i][j+1].content)
				 	{
				 		contextPerso.board[i][j] = contextPerso.board[i][j+1];
				 		contextPerso.board[i][j+1] = gameState->board[i][j+1];
				 	}
				 }
				 else
				 {
				 	contextPerso.board[i][j] = gameState->board[i][j];
				 }
			}
		}
	}
}


/*
Cette fonction renvoie 1 si l'ia doit attaquer, sinon 0
paramètres: ligne et colonne de la pièce de l'ia, puis lign et colonne de la pièce adverse
*/
int doitAttaquer(int lineArmy,int colArmy,int lineEnemy,int colEnemy)
{
	//on récupère les forces des deux pièces
	EPiece enemy = contextPerso.board[lineEnemy][colEnemy].piece;
	EPiece army = contextPerso.board[lineArmy][colArmy].piece;
	
	if (enemy==EPnone && army!=EPminer && army!=EPspy && army!=EPmarshal)//adversaire inconnu, et piece de l'ia n'est pas un démineur ni l'espion ni le marshal
		return 1;
	else if (enemy<army && enemy!=EPbomb)//avantage de force
		return 1;
	else if (enemy==EPbomb && army==EPminer)// si l'adversaire est une bombe et la piece de l'ia un démineur
		return 1;
	else if (army==EPspy && enemy==EPmarshal)//l'adversaire est le marshal et la pièce de l'ia est l'espion
		return 1;

	//S'il y a plus de 30 pièces éliminées pour l'ia, et l'adversaire n'est pas une bombe, on attaque tout le temps
	int l,k=0;
	for(l=0;l<40;l++)
	{
		if(positionPiece[l].line < 0)
			k++;
	}
	if(k>30 && enemy!=EPbomb)
		return 1;

	return 0;
}

/*
Cette fonction renvoie 1 si mouvement enregistré dans end.line et end.col du SMove de NextMove, si le retour est 0: on ne souhaite pas bouger (line ==-1)
paramètres: valeure aléatoire pour décider de la direction du mouvement,
l'adresse de la position de fin du mouvement sous la forme de deux pointeurs d'entiers, puis la position de départ sous la forme de deux entiers
*/
int	mouvementAleatoire(int j, int * line, int * col, int startLine, int startCol)
{
	//on récupère la force de la pièce à bouger
	EPiece army = contextPerso.board[startLine][startCol].piece;
	

	if( (j>=0) && (j<15) )//15 chances sur 20 d'aller en avant
	{
		*line = startLine+1;
		*col = startCol;

		if (mouvementSpyMiner(army,*line,*col)==1)//vérifie si un espion ou un démineur se met en danger lors du moouvement
		{
			*line = -1;
			return 0;
		}

		return 1;
	}		
	else if(j>=15 && j< 17)//2 chances sur 20 d'aller à gauche
	{
		*line = startLine;
		*col = startCol-1;

		if (mouvementSpyMiner(army,*line,*col)==1)
		{
			*line = -1;
			return 0;
		}

		return 1;
	}		
	else if(j>=17 && j<19)//2 chances sur 20 d'aller à droite
	{
		*line = startLine;
		*col = startCol+1;

		if (mouvementSpyMiner(army,*line,*col)==1)
		{
			*line = -1;
			return 0;
		}

		return 1;
	}		
	else if(j == 19)//1 chances sur 20 d'aller en arrière
	{
		*line = startLine-1;
		*col = startCol;

		if (mouvementSpyMiner(army,*line,*col)==1)
		{
			*line = -1;
			return 0;
		}

		return 1;
	}

	return 0;
}

/*
Cette fonction essaye de trouver une solution pour fuir
paramètres: position de début sous forme de deux entiers, position d'arrivée à remplir: 2 pointeurs d'entiers
renvoie 1 si la fuite est possible, 0 sinon
*/
int fuite(int line, int col, int * endLine, int * endCol)
{
	//on vérifie à chaque fois que l'on ne se dirige pas vers l'adversaire
	//on commence par vouloir aller en arrière
	if ( line>0 && contextPerso.board[line-1][col].content != couleurAdverse)
	{
		*endLine = line-1;// les valeurs sont directement modifiées (pointeurs)
		*endCol = col;
		return 1;
	}
	else if ( col>0 && contextPerso.board[line][col-1].content != couleurAdverse)
	{
		*endLine = line;
		*endCol = col-1;
		return 1;
	}
	else if ( col<9 && contextPerso.board[line][col+1].content != couleurAdverse)
	{
		*endLine = line;
		*endCol = col+1;
		return 1;
	}
	else if ( line<9 && contextPerso.board[line+1][col].content != couleurAdverse)
	{
		*endLine = line+1;
		*endCol = col;
		return 1;
	}
	
	//pas de fuite possible
	return 0;
}

/*
Cette fonction renvoie 1 si un demineur ou l'espion se met en danger avec un mouvement
paramètre: le type de pièce, puis la position du mouvement voulu sous la forme de deux entiers
*/
int mouvementSpyMiner(EPiece army,int i,int j)
{
	//s'il y a plus de 30 pièces éliminées pour l'ia, les pièces sensibles peuvent se mettre en danger 
	int l,k=0;
	for(l=0;l<40;l++)
	{
		if(positionPiece[l].line < 0)
			k++;
	}
	if(k>30)
		return 0;

	//si il s'agit de l'espion
	if (army == EPspy)
	{
		//On vérifie qu'il n'y a pas d'adversaires autour de la position finale
		if ( i<9 && contextPerso.board[i+1][j].content == couleurAdverse)
				return 1;
		
		if ( i>0 && contextPerso.board[i-1][j].content == couleurAdverse)
				return 1;

		if ( j<9 && contextPerso.board[i][j+1].content == couleurAdverse)
				return 1;

		if ( j>0 && contextPerso.board[i][j-1].content == couleurAdverse)
				return 1;
	}
	//si il s'agit du démineur
	else if (army == EPminer)
	{
		//On vérifie qu'il n'y a pas d'adversaires autour de la position finale autre que des bombes
		if ( i<9 && contextPerso.board[i+1][j].content == couleurAdverse && contextPerso.board[i+1][j].piece != EPbomb)
				return 1;
		

		if ( i>0 && contextPerso.board[i-1][j].content == couleurAdverse && contextPerso.board[i+1][j].piece != EPbomb)
				return 1;

		if ( j<9 && contextPerso.board[i][j+1].content == couleurAdverse && contextPerso.board[i+1][j].piece != EPbomb)
				return 1;

		if ( j>0 && contextPerso.board[i][j-1].content == couleurAdverse && contextPerso.board[i+1][j].piece != EPbomb)
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
