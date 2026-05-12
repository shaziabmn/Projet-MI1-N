#ifndef PLATEAU_H
#define PLATEAU_H

#include "types.h"
#include "display.h"
#include "stats.h"

// Initialise les 25 cartes melangees aleatoirement
void init_plateau(Jeu *jeu);

// Affiche le plateau avec couleurs, position des joueurs et timer
void afficher_plateau(Jeu *jeu);

// Cache toutes les cartes et renvoie le joueur actuel a son depart
void reinitialiser_plateau(Jeu *jeu);

#endif
