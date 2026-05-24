#ifndef TOUR_H
#define TOUR_H


#include "structures.h"

// Retourne le nom de l'arme
const char *nom_arme(int arme);

// Retourne le nom de la case
const char *nom_case(int type);

// Retourne le type d'arme antique que cherche un joueur
int arme_antique_du_joueur(int classe);

// Vérifie si une arme vainc un monstre
int arme_vainc_monstre(int arme, int monstre);

// Vérifie si un type de case est un monstre
int est_monstre(int type);

// Vérifie si un type de case est une arme antique
int est_arme_antique(int type);

// Joue un tour complet pour le joueur actuel
void jouer_tour(Jeu *jeu);

#endif
