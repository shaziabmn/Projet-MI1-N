#ifndef PLATEAU_H
#define PLATEAU_H

#include "structures.h"
#include "affichage.h"
#include "statistiques.h"

// Initialise les 25 cartes mélangées aléatoirement
void init_plateau(Jeu *jeu);

// Affiche le plateau avec couleurs, position des joueurs et chrono
void afficher_plateau(Jeu *jeu);

// Cache toutes les cartes et renvoie le joueur actuel à son départ
void reinitialiser_plateau(Jeu *jeu);

#endif
