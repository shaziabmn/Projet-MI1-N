#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "structures.h"
#include "affichage.h"
#include "statistiques.h"


// Heure de début de la manche
time_t heure_debut = 0;


// Démarre le chronomètre de la manche
void chrono_demarrer() {
    heure_debut = time(NULL); 
}


// Affiche le temps écoulé depuis chrono_demarrer()
void chrono_afficher() {
    int total    = time(NULL) - heure_debut; // Secondes écoulées
    int minutes  = total / 60;
    int secondes = total % 60;
    printf("      Durée de la manche : %02d:%02d\n", minutes, secondes);
}


// Charge les statistiques depuis le fichier joueurs.dat
void statistiques_charger(StatJoueur statistiques[], int *nb) {
    FILE *f;
    char ligne[256];
    *nb = 0;

    f = fopen(FICHIER_STATS, "r");
    if (f == NULL) {
        return;
    }

    while (fgets(ligne, sizeof(ligne), f) != NULL &&
           *nb < MAX_JOUEURS_FICHIER) {
        char nom[NOM_MAX];
        int  parties, victoires;
        if (sscanf(ligne, "%15[^;];%d;%d", nom, &parties, &victoires) == 3) {
            strcpy(statistiques[*nb].nom, nom);
            statistiques[*nb].parties   = parties;
            statistiques[*nb].victoires = victoires;
            (*nb)++;
        }
    }
    fclose(f);
}


// Sauvegarde les statistiques dans le fichier joueurs.dat
void statistiques_sauvegarder(StatJoueur statistiques[], int nb) {
    FILE *f;
    int i;

    f = fopen(FICHIER_STATS, "w");
    if (f == NULL) {
        printf(ROUGE"  ❓ Erreur : Impossible de sauvegarder les statistiques.\n"REINIT);
        return;
    }
    for (i = 0; i < nb; i++) {
        fprintf(f, "%s;%d;%d\n",
                statistiques[i].nom, statistiques[i].parties, statistiques[i].victoires);
    }
    fclose(f);
}


// Cherche un joueur par son nom, le crée s'il n'existe pas
int statistiques_trouver_ou_creer(StatJoueur statistiques[], int *nb, char *nom) {
    int i;
    for (i = 0; i < *nb; i++) {
        if (strcmp(statistiques[i].nom, nom) == 0) {
            return i;
        }
    }

    if (*nb >= MAX_JOUEURS_FICHIER) {
        return -1;
    }

    strcpy(statistiques[*nb].nom, nom);
    statistiques[*nb].parties   = 0;
    statistiques[*nb].victoires = 0;
    int idx = *nb;   // On garde l'ancienne valeur (indice du nouveau joueur)
    (*nb)++;         // On augmente le compteur
    return idx;
}



// Met à jour les statistiques après une partie et sauvegarde
void statistiques_enregistrer_partie(StatJoueur statistiques[], int *nb,
                                     Jeu *jeu, char *nom_gagnant) {
    int i, idx;

    // +1 partie pour chaque joueur
    for (i = 0; i < jeu->nb_joueurs; i++) {
        idx = statistiques_trouver_ou_creer(statistiques, nb, jeu->joueurs[i].nom);
        if (idx >= 0) {
            statistiques[idx].parties++;
        }
    }

    // +1 victoire pour le gagnant, s'il y en a un
    if (nom_gagnant != NULL) {
        idx = statistiques_trouver_ou_creer(statistiques, nb, nom_gagnant);
        if (idx >= 0) {
            statistiques[idx].victoires++;
        }
    }

    statistiques_sauvegarder(statistiques, *nb);
}


// Trie les statistiques 
void statistiques_trier(StatJoueur statistiques[], int nb) {
    int i, j;
    StatJoueur tmp;
    for (i = 0; i < nb - 1; i++) {
        for (j = 0; j < nb - 1 - i; j++) {
            if (statistiques[j].victoires < statistiques[j+1].victoires) {
                tmp               = statistiques[j];
                statistiques[j]   = statistiques[j+1];
                statistiques[j+1] = tmp;
            }
        }
    }
}


// Affiche le classement de tous les joueurs
void statistiques_afficher_classement(StatJoueur statistiques[], int nb) {
    int i;

    printf("\n");
    printf(GRAS_BLANC   "                             ┌─────────────────────────┐\n" REINIT);
    printf(GRAS_MAGENTA "                             ✦      📊 CLASSEMENT      ✦\n" REINIT);
    printf(GRAS_BLANC   "                             └─────────────────────────┘\n\n" REINIT);

    if (nb == 0) {
        printf(GRAS_MAGENTA "                       Aucun joueur enregistré pour l'instant.\n\n" REINIT);
        return;
    }

    statistiques_trier(statistiques, nb);

    for (i = 0; i < nb; i++) {
        printf("              #%d  %-15s  —  %s%d partie(s)%s  -  %s%d victoire(s)%s\n",
               i + 1, statistiques[i].nom,
               CYAN, statistiques[i].parties, REINIT,
               BLEU, statistiques[i].victoires, REINIT);
    }
    printf("\n");
}
