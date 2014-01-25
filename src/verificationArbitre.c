/**
 * Projet Stratego, 3ème année de polytech
 * Réalisé par : -Guinard Arnaud
 * 				 -Mouden Benjamin
 * 				 -Evo Brieuc
 * 				 -Chotard François
 */

#include "PolyLib.h"

SMove listMvtJ1[3], listMvtJ2[3];
SBox listBoxJ1[3],listBoxJ2[3];
int repetitionMvtJ1 = 0,repetitionMvtJ2 = 0;

/*
 * Fonction qui verifie que les pions place par le joueur est correcte,
 * pour cela on verifie que tous les types de pions sont presents et qu'il y a le bon nombre de chaque type
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
		afficherMessageEcran("ERREUR Bomb", 1000);
		return 1;
	} else if(Spy != 1){
		afficherMessageEcran("ERREUR Spy", 1000);
		return 1;
	} else if(Scout != 8){
		afficherMessageEcran("ERREUR Scout", 1000);
		return 1;
	} else if(Miner != 5){
		afficherMessageEcran("ERREUR Miner", 1000);
		return 1;
	} else if(Sergeant != 4){
		afficherMessageEcran("ERREUR Sergeant", 1000);
		return 1;
	} else if(Lieutenant != 4){
		afficherMessageEcran("ERREUR Lieutenant", 1000);
		return 1;
	} else if(Captain != 4){
		afficherMessageEcran("ERREUR Captain", 1000);
		return 1;
	} else if(Major != 3){
		afficherMessageEcran("ERREUR Major", 1000);
		return 1;
	} else if(Colonel != 2){
		afficherMessageEcran("ERREUR Colonel", 1000);
		return 1;
	} else if(General != 1){
		afficherMessageEcran("ERREUR General", 1000);
		return 1;
	} else if(Marshal != 1){
		afficherMessageEcran("ERREUR Marshal", 1000);
		return 1;
	}else if(Flag != 1){
		afficherMessageEcran("ERREUR Flag", 1000);
		return 1;
	}
	return 0;
}

/*
 * Fonction qui va comparer 2 move donné en paramètre
 * Renvoie 1 quand ils sont identique sinon 0
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
 * Méthode qui vérifie que le joueur 1 ne réalise par de va et viens
 */
int vaEtViensJ1(SMove move, SBox box){
	
	    switch (repetitionMvtJ1)
    {
		//Cas ou on aucune répétition
        case 0: 
				listMvtJ1[repetitionMvtJ1] = move;
				listBoxJ1[repetitionMvtJ1] = box;
                repetitionMvtJ1++;
                break;
                
		//Cas ou on 1 répétition, on vérifie que la pièce bougé n'est pas déjà enregistré
        case 1:
                if( box.piece == listBoxJ1[0].piece ){
                    listMvtJ1[repetitionMvtJ1] = move;
                    listBoxJ1[repetitionMvtJ1] = box;
                    repetitionMvtJ1++;
                }
                else{ 
					repetitionMvtJ1 = 1;
					listMvtJ1[0] = move;
                    listBoxJ1[0] = box; }
                break;
          
        //Cas ou on 2 répétitions, on vérifie que la pièce bougé n'est pas déjà enregistré et qu'elle pas deja réalisé ce mouvement
        case 2:
            if( (box.piece == listBoxJ1[0].piece) && (CompareMoves(move,listMvtJ1[0])==1) ){
                listMvtJ1[repetitionMvtJ1] = move;
                listBoxJ1[repetitionMvtJ1] = box;
                repetitionMvtJ1++;
            }
            else{ 
					repetitionMvtJ1 = 1;
					listMvtJ1[0] = move;
                    listBoxJ1[0] = box; }
            break;
         
         //Cas ou on 3 répétitions, on vérifie que la pièce bougé n'est pas déjà enregistré et qu'elle pas deja réalisé ce mouvement
         case 3:
            if( (box.piece == listBoxJ1[0].piece) && (CompareMoves(move,listMvtJ1[1])==1) ){
                repetitionMvtJ1++;
            }
             else{ 
					repetitionMvtJ1 = 1;
					listMvtJ1[0] = move;
                    listBoxJ1[0] = box; }
            break;
        default:
            repetitionMvtJ1 = 0;
            break;
    }
    
    //Si on a 4 répétitions alors on renvoie 1
    if(repetitionMvtJ1 == 4){
		return 1;
	}
	else{
		return 0;
	}
}

/*
 * Méthode qui vérifie que le joueur 1 ne réalise par de va et viens
 */
int vaEtViensJ2(SMove move, SBox box){
	
	    switch (repetitionMvtJ2)
    {
		//Cas ou on aucune répétition
        case 0: 
				listMvtJ2[repetitionMvtJ2] = move;
				listBoxJ2[repetitionMvtJ2] = box;
                repetitionMvtJ2++;
                break;
                
       //Cas ou on 1 répétition, on vérifie que la pièce bougé n'est pas déjà enregistré
        case 1:
                if( box.piece == listBoxJ2[0].piece ){
                    listMvtJ2[repetitionMvtJ2] = move;
                    listBoxJ2[repetitionMvtJ2] = box;
                    repetitionMvtJ2++;
                }
                else{ 
					repetitionMvtJ2 = 1;
					listMvtJ2[0] = move;
                    listBoxJ2[0] = box; }
                break;
                
        //Cas ou on 2 répétitions, on vérifie que la pièce bougé n'est pas déjà enregistré et qu'elle pas deja réalisé ce mouvement
        case 2:
            if( (box.piece == listBoxJ2[0].piece) && (CompareMoves(move,listMvtJ2[0])==1) ){
                listMvtJ2[repetitionMvtJ2] = move;
                listBoxJ2[repetitionMvtJ2] = box;
                repetitionMvtJ2++;
            }
            else{ 
					repetitionMvtJ2 = 1;
					listMvtJ2[0] = move;
                    listBoxJ2[0] = box; }
            break;
            
         //Cas ou on 3 répétitions, on vérifie que la pièce bougé n'est pas déjà enregistré et qu'elle pas deja réalisé ce mouvement
         case 3:
            if( (box.piece == listBoxJ2[0].piece) && (CompareMoves(move,listMvtJ2[1])==1) ){
                repetitionMvtJ2++;
            }
             else{ 
					repetitionMvtJ2 = 1;
					listMvtJ2[0] = move;
                    listBoxJ2[0] = box; }
            break;
        default:
            repetitionMvtJ2 = 0;
            break;
    }
    
    //Si on a 4 répétitions alors on renvoie 1
    if(repetitionMvtJ2 == 4){
		return 1;
	}
	else{
		return 0;
	}
}


