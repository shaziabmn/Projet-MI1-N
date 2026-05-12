#ifndef TOUR_H
#define TOUR_H

// Gestion du déroulement d'un tour de jeu.

#include "types.h"

// Retourne le nom de l'arme choisie
char *nom_arme(int arme);

// Retourne le nom de la case
char *nom_case(int type);

// Retourne le type d'arme antique que cherche un joueur
int arme_antique_du_joueur(int classe);

// Verifie si une arme vainc un monstre
int arme_vainc_monstre(int arme, int monstre);

// Verifie si un type de case est un monstre
int est_monstre(int type);

// Verifie si un type de case est une arme antique
int est_arme_antique(int type);

// Joue un tour complet pour le joueur actuel
// Retourne le resultat du tour (TOUR_VICTOIRE, TOUR_MORT...)
int jouer_tour(Jeu *jeu);

#endif
