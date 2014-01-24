# définition des cibles particulières
.PHONY: clean, mrproper

#création de l'exécutable "stratego"
all: ./src/stratego.c clean	
		gcc -g ./src/IGStratego.c ./src/dupplicationArbitre.c ./src/verificationArbitre.c ./src/affichageArbitre.c ./src/stratego.c -o stratego -ldl -lSDL -lSDLmain -lSDL_image -lSDL_ttf -lpthread -D_REENTRANT -lX11

install: polylib.so polylib2.so
		

polylib.so: polylib.o
		gcc -shared -o ./lib/polylib.so *.o	

polylib2.so: polylib.o
		gcc -shared -o ./lib/polylib2.so *.o	

polylib.o: ./src/PolyLib.c	
		gcc -fPIC -c ./src/PolyLib.c

# suppression des fichiers temporaires
clean:
		rm -rf *.o
 
# suppression de tous les fichiers, sauf les sources,
# en vue d'une reconstruction complète
mrproper: clean
		rm -rf stratego
