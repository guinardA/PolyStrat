***************************
Stratego
***************************



___________________________

Installation
___________________________



Se placer � la racine du dossier /Polystrat puis effectuer les commandes suivantes :


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
R�partition du travail
___________________________

Bin�me IA :

Arnaud Guinard
Benjamin Mouden


Bin�me interface graphique :

Fran�ois Chotard
Brieuc Evo



Remarque :

Nous avons souvent travaill� ensemble et donc m�lang� les bin�mes de travail


___________________________
Strat�gie de l'IA
___________________________

Nous avons commenc� par faire une IA basique qui avance les pions al�atoirement.
Puis nous l'avons �toff� en la faisant sauvegarder le contexte et en ajoutant quelques r�gles de jeu comme:
fuir quand l'ennemi est plus fort, augmenter les chances d'aller vers l'avant, faire que les d�mineurs et les espions n'attaquent
que quand cela est n�cessaire.