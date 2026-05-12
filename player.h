#ifndef PLAYER_H
#define PLAYER_H

// Gestion des joueurs : initialisation, classes, couleurs, rotation

#include "types.h"

// Couleur ANSI attribuee a chaque joueur selon son indice
//   J1 = vert, J2 = rouge, J3 = cyan, J4 = jaune
const char *couleur_joueur(int idx);

// Emoji de la classe d'un joueur
const char *emoji_classe(int classe);

// Nom de la classe (pour affichage)
const char *nom_classe(int classe);

// Demande le nom d'un joueur (avec gestion du buffer trop long)
void nom_joueur(Joueur *j, int numero);

// Demande noms et assigne classes/positions des joueurs
void init_joueurs(Jeu *jeu);

// Remet les joueurs a leur depart sans redemander les noms
void reinit_joueurs_pour_rejouer(Jeu *jeu);

// Passe au joueur actif suivant (tourne en boucle)
void joueur_suivant(Jeu *jeu);

#endif
