#ifndef AFFICHAGE_H
#define AFFICHAGE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


#define REINIT       "\033[0m" // Réinitialise la couleur à la valeur par défaut
#define GRAS        "\033[1m"  // Met le texte en gras

// Met le texte en couleurs
#define ROUGE         "\033[31m"
#define VERT       "\033[32m"
#define JAUNE      "\033[33m"
#define BLEU        "\033[34m"
#define MAGENTA     "\033[35m"
#define CYAN        "\033[36m"
#define BLANC       "\033[37m"

// Met le texte en couleurs et en gras
#define GRAS_ROUGE    "\033[1;31m"
#define GRAS_VERT  "\033[1;32m"
#define GRAS_JAUNE "\033[1;33m"
#define GRAS_BLEU   "\033[1;34m"
#define GRAS_MAGENTA "\033[1;35m"
#define GRAS_CYAN   "\033[1;36m"
#define GRAS_BLANC  "\033[1;37m"

// Efface l'écran
#define clrscr() printf("\033[H\033[2J")

void afficher_jeu();
void afficher_menu();
void afficher_regles();

#endif

