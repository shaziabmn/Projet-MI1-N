
// Gestion des statistiques et persistance (Personne 3)


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "types.h"
#include "display.h"
#include "stats.h"

// Heure de debut de la manche 
static time_t heure_debut = 0;


// Timer

void timer_start(void) {
    heure_debut = time(NULL);
}

void timer_afficher(void) {
    int total   = (int)difftime(time(NULL), heure_debut);
    int minutes = total / 60;
    int secondes = total % 60;
    printf("      Durée de la manche : %02d:%02d\n", minutes, secondes);
}

// Chargement depuis le fichier
// Format : NOM;PARTIES;VICTOIRES

void stats_charger(StatJoueur stats[], int *nb) {
    FILE *f;
    char ligne[256];
    *nb = 0;

    f = fopen(FICHIER_STATS, "r");
    if (f == NULL) return; // Fichier absent = premiere utilisation 

    while (fgets(ligne, sizeof(ligne), f) != NULL &&
           *nb < MAX_JOUEURS_DB) {
        char nom[NOM_MAX];
        int  parties, victoires;
        if (sscanf(ligne, "%63[^;];%d;%d", nom, &parties, &victoires) == 3) {
            strncpy(stats[*nb].nom, nom, NOM_MAX - 1);
            stats[*nb].parties   = parties;
            stats[*nb].victoires = victoires;
            (*nb)++;
        }
    }
    fclose(f);
}


// Sauvegarde dans le fichier
void stats_sauvegarder(StatJoueur stats[], int nb) {
    FILE *f;
    int i;

    f = fopen(FICHIER_STATS, "w");
    if (f == NULL) {
        printf(RED"  ❓ Erreur : Impossible de sauvegarder les statistiques.\n"RESET);
        return;
    }
    for (i = 0; i < nb; i++)
        fprintf(f, "%s;%d;%d\n",
                stats[i].nom, stats[i].parties, stats[i].victoires);
    fclose(f);
}


// Trouve un joueur par son nom, le cree si absent
// Retourne son index dans le tableau


int stats_trouver_ou_creer(StatJoueur stats[], int *nb, char *nom) {
    int i;
    for (i = 0; i < *nb; i++)
        if (strcmp(stats[i].nom, nom) == 0)
            return i;

    // Nouveau joueur 
    if (*nb >= MAX_JOUEURS_DB) return -1;
    strncpy(stats[*nb].nom, nom, NOM_MAX - 1);
    stats[*nb].parties   = 0;
    stats[*nb].victoires = 0;
    return (*nb)++;
}


// Met à jour les stats après une partie

void stats_enregistrer_partie(StatJoueur stats[], int *nb,
                               char *noms[], int nb_joueurs,
                               char *nom_gagnant) {
    int i, idx;

    for (i = 0; i < nb_joueurs; i++) {
        idx = stats_trouver_ou_creer(stats, nb, noms[i]);
        if (idx >= 0) stats[idx].parties++;
    }

    // On n'enregistre une victoire QUE s'il y a un vrai gagnant
    // (pas d'abandon ni d'absence de gagnant) 
    if (nom_gagnant != NULL && strcmp(nom_gagnant, "Personne") != 0) {
        idx = stats_trouver_ou_creer(stats, nb, nom_gagnant);
        if (idx >= 0) stats[idx].victoires++;
    }

    stats_sauvegarder(stats, *nb);
}

// Tri par victoires decroissantes (tri a bulles simple)

static void trier_stats(StatJoueur stats[], int nb) {
    int i, j;
    StatJoueur tmp;
    for (i = 0; i < nb - 1; i++)
        for (j = 0; j < nb - 1 - i; j++)
            if (stats[j].victoires < stats[j+1].victoires) {
                tmp        = stats[j];
                stats[j]   = stats[j+1];
                stats[j+1] = tmp;
            }
}

// Affiche le classement
void stats_afficher_classement(StatJoueur stats[], int nb) {
    int i;
    StatJoueur copie[MAX_JOUEURS_DB];

    // Affichage classement
    printf("\n");
    printf(BOLD_WHITE  "                             ┌─────────────────────────┐\n" RESET);
    printf(BOLD_MAGENTA "                             ✦      📊 CLASSEMENT      ✦\n" RESET);
    printf(BOLD_WHITE  "                             └─────────────────────────┘\n\n" RESET);

    if (nb == 0) {
        printf(BOLD_YELLOW "                       Aucun joueur enregistré pour l'instant.\n\n" RESET);
        return;
    }

    // Copie pour ne pas modifier l'original 
    for (i = 0; i < nb; i++) copie[i] = stats[i];
    trier_stats(copie, nb);

    for (i = 0; i < nb; i++) {
        const char *medaille = "  ";
        const char *couleur  = RESET;
        if      (i == 0) { medaille = "🥇"; couleur = BOLD_YELLOW; }
        else if (i == 1) { medaille = "🥈"; couleur = BOLD_WHITE;  }
        else if (i == 2) { medaille = "🥉"; couleur = BOLD_RED;    }

        printf("              %s%s%s  #%d  %-15s  —  %s%d partie(s)%s  -  %s%d victoire(s)%s\n",
            couleur, medaille, RESET, i + 1, copie[i].nom,
            CYAN, copie[i].parties, RESET,
            BLUE, copie[i].victoires, RESET);
    }
    printf("\n");
}


