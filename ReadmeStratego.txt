***************************
Stratego
***************************



___________________________

Installation
___________________________



Se placer à la racine du dossier /Polystrat puis effectuer les commandes suivantes :


#Permet le chargement des librairie depuis le dossier courant
export LD_LIBRARY_PATH=LD_LIBRARY_PATH:.





#Compilation des librairies

make install


#Compilation des fichiers source (interface graphique et arbitre entre autres)
make




___________________________
Lancement de l'application 
___________________________



Structure de la commande :


./stratego [nombre de coups maximum] [nombre d'IA] ./lib/[nom de l'IA 1] ./lib/[nom de l'IA 2]






Exemples :



./stratego 100 2 ./lib/polylib.so ./lib/polylib2.so



./stratego 100 1 ./lib/polylib.so


./stratego 100 0



___________________________
Répartition du travail
___________________________

Binôme IA :

Arnaud Guinard
Benjamin Mouden


Binôme interface graphique :

François Chotard
Brieuc Evo



Remarque :

Nous avons souvent travaillé ensemble et donc mélangé les binômes de travail


___________________________
Stratégie de l'IA
___________________________

Nous avons commencé par faire une IA basique qui avance les pions aléatoirement.
Puis nous l'avons étoffé en la faisant sauvegarder le contexte et en ajoutant quelques règles de jeu comme:
fuir quand l'ennemi est plus fort, augmenter les chances d'aller vers l'avant, faire que les démineurs et les espions n'attaquent
que quand cela est nécessaire.