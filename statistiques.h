#ifndef STATISTIQUES_H
#define STATISTIQUES_H

#include "structures.h"

// Démarre le chronomètre de la manche
void chrono_demarrer();

// Affiche le temps écoulé depuis chrono_demarrer()
void chrono_afficher();

// Charge les statistiques depuis le fichier joueurs.dat
void statistiques_charger(StatJoueur statistiques[], int *nb);

// Sauvegarde les statistiques dans le fichier joueurs.dat
void statistiques_sauvegarder(StatJoueur statistiques[], int nb);

// Cherche un joueur par son nom, le crée s'il n'existe pas
int statistiques_trouver_ou_creer(StatJoueur statistiques[], int *nb, char *nom);

// Met à jour les statistiques après une partie et sauvegarde
void statistiques_enregistrer_partie(StatJoueur statistiques[], int *nb, Jeu *jeu, char *nom_gagnant);

// Affiche le classement de tous les joueurs
void statistiques_afficher_classement(StatJoueur statistiques[], int nb);

#endif
