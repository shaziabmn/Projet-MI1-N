#ifndef JOUEUR_H
#define JOUEUR_H


#include "structures.h"

// Couleur attribuée à chaque joueur selon son indice
const char *couleur_joueur(int idx);

// Emoji de la classe d'un joueur
const char *emoji_classe(int classe);

// Nom de la classe d'un joueur
const char *nom_classe(int classe);

// Lit un entier dans [min, max]
int lire_entier(int min, int max);

// Vide le reste du buffer d'entrée jusqu'au saut de ligne
void vider_buffer(void);

// Demande le nom d'un joueur
void nom_joueur(Joueur *j, int numero);

// Demande les noms et assigne classes et positions des joueurs
void init_joueurs(Jeu *jeu);

// Remet les joueurs à leur position de départ sans redemander les noms
void reinit_joueurs_pour_rejouer(Jeu *jeu);

// Passe au joueur suivant
void joueur_suivant(Jeu *jeu);

#endif
