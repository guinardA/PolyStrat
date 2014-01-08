# définition des cibles particulières
.PHONY: clean, mrproper

#création de l'exécutable "stratego"
all: stratego.c clean	
		gcc -g IGStratego.c stratego.c -o stratego -ldl -lSDL -lSDLmain -lSDL_image

install: polylib.so
		sudo mv polylib.so /usr/lib/

polylib.so: polylib.o
		gcc -shared -o polylib.so *.o

polylib.o: PolyLib.c	
		gcc -fPIC -c PolyLib.c

# suppression des fichiers temporaires
clean:
		rm -rf *.o
 
# suppression de tous les fichiers, sauf les sources,
# en vue d'une reconstruction complète
mrproper: clean
		rm -rf stratego