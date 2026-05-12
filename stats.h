// Gestion des statistiques

#ifndef STATS_H
#define STATS_H

#include "types.h"

// Démarre le chronomètre de la manche
void timer_start(void);

// Affiche le temps écoulé depuis timer_start() au format mm:ss 
void timer_afficher(void);

// Charge les stats depuis le fichier joueurs.dat 
void stats_charger(StatJoueur stats[], int *nb);

// Sauvegarde les stats dans le fichier joueurs.dat 
void stats_sauvegarder(StatJoueur stats[], int nb);

// Trouve l'index d'un joueur par son nom, le cree si absent 
int stats_trouver_ou_creer(StatJoueur stats[], int *nb, char *nom);

// Met à jour les statistiques après une partie et sauvegarde
void stats_enregistrer_partie(StatJoueur stats[], int *nb,
                               char *noms[], int nb_joueurs,
                               char *nom_gagnant);

// Affiche le classement de tous les joueurs
void stats_afficher_classement(StatJoueur stats[], int nb);

#endif
