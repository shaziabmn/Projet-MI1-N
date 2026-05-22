memorpg : main.o joueur.o plateau.o tour.o statistiques.o affichage.o
	gcc main.o joueur.o plateau.o tour.o statistiques.o affichage.o -o memorpg

main.o : main.c structures.h affichage.h joueur.h plateau.h tour.h statistiques.h
	gcc -c main.c

joueur.o : joueur.c structures.h affichage.h joueur.h
	gcc -c joueur.c

plateau.o : plateau.c structures.h affichage.h statistiques.h joueur.h plateau.h
	gcc -c plateau.c

tour.o : tour.c structures.h affichage.h joueur.h tour.h plateau.h
	gcc -c tour.c

statistiques.o : statistiques.c structures.h affichage.h statistiques.h
	gcc -c statistiques.c

affichage.o : affichage.c affichage.h joueur.h
	gcc -c affichage.c

clean :
	rm -f *.o memorpg
