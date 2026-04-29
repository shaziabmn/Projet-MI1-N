#ifndef BOARD_H
#define BOARD_H

#include "display.h"

#include <stdlib.h>  
#include <time.h>

#define TAILLE 5

// Structure case du labyrinthe
typedef struct {
    int type;      // Type de case (monstre, coffre, arme, etc.)
    int visible;   // 0 = cachée, 1 = révélée
} Case;


void creer_labyrinthe(Case grille[TAILLE][TAILLE]);
void afficher_labyrinthe(Case grille[TAILLE][TAILLE]);
void afficher_symbole(int type);
void afficher_legende(void);


#endif
