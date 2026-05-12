#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "types.h"
#include "display.h"
#include "player.h"
#include "plateau.h"
#include "tour.h"
#include "stats.h"


// Révèle toutes les cartes à la fin de la partie
static void reveler_tout(Jeu *jeu) {
    int i, j;
    for (i = 0; i < TAILLE_PLATEAU; i++)
        for (j = 0; j < TAILLE_PLATEAU; j++)
            jeu->plateau[i][j].revelee = 1;
}


// Lit un entier dans [min, max] de maniere securisee

static int lire_entier(int min, int max) {
    int valeur;
    char buffer[64];
    while (1) {
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) return min;

        // Vide le reste du stdin si la saisie depasse la taille du buffer
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] != '\n') {
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
        }

        if (sscanf(buffer, "%d", &valeur) == 1 &&
            valeur >= min && valeur <= max)
            return valeur;
        printf(RED "  ❓ Choix invalide. Veuillez entrer un nombre entre %d et %d :  👉  " RESET, min, max);
    }
}

// Programme principal

int main(void) {
    Jeu         jeu;
    StatJoueur  stats[MAX_JOUEURS_DB];
    int         nb_stats    = 0;
    int         dans_menu   = 1;
    int         rejouer     = 0;
    int         memes_joueurs = 0;

    srand((unsigned int)time(NULL));

    // Charge les stats existantes depuis le fichier 
    stats_charger(stats, &nb_stats);

    // Ecran d'accueil 
    afficher_jeu();

    while (dans_menu) {
        afficher_menu();

        int choix_menu = lire_entier(1, 3);

        if (choix_menu == 3) {
            dans_menu = 0;
            break;
        }

        if (choix_menu == 2) {
            // Voir le classement 
            clrscr();
            stats_afficher_classement(stats, nb_stats);
            printf("\n  Appuyez sur " BOLD_CYAN "ENTRER" RESET " pour revenir au menu...   👉  ");
            { int c; while ((c = getchar()) != '\n' && c != EOF); }
            clrscr();
            continue;
        }

        // Nouvelle partie 
        rejouer       = 1;
        memes_joueurs = 0;

        while (rejouer) {
            // Melange les cartes du plateau 
            init_plateau(&jeu);

            // Demande noms/classes ou remet a zero si memes joueurs 
            if (!memes_joueurs) {
                init_joueurs(&jeu);
            } else {
                reinit_joueurs_pour_rejouer(&jeu);
            }

            // Démarre le chronomètre
            timer_start();

            // Boucle de jeu 
            while (!jeu.partie_finie) {
                jouer_tour(&jeu);
            }

            // Fin de partie : revele tout le plateau 
            reveler_tout(&jeu);
            afficher_plateau(&jeu);

        
           

            // Affichage du gagnant 

            if (jeu.gagnant >= 0) {
            printf("\n");
            printf(BOLD_WHITE  "                             ┌─────────────────────────┐\n" RESET);
            printf(BOLD_YELLOW "                             ✦       🏆 VICTOIRE       ✦\n" RESET);
            printf(BOLD_WHITE  "                             └─────────────────────────┘\n\n" RESET);
            printf(BOLD_YELLOW "\n              Félicitations ! " BOLD_WHITE "%s" RESET " a remporté la partie de Memo-RPG !\n" RESET, jeu.joueurs[jeu.gagnant].nom);
            printf("                       ");
            timer_afficher();
            printf("\n");

            } else {

            // Affichage de la partie abandonnée
            printf("\n");
            printf(BOLD_WHITE "                             ┌─────────────────────────┐\n" RESET);
            printf(BOLD_RED   "                             ✦  👎  PARTIE ABANDONNÉE  ✦\n" RESET);
            printf(BOLD_WHITE "                             └─────────────────────────┘\n\n" RESET);
            printf(BOLD_RED   "                          Personne n'a remporté la partie.\n" RESET);
            printf("                       ");
            timer_afficher();
            printf("\n");

            }

            // Enregistre et affiche les statistiques

            char *noms[NB_JOUEURS_MAX];
            int i;
            for (i = 0; i < jeu.nb_joueurs; i++)
                noms[i] = jeu.joueurs[i].nom;

            char *nom_gagnant = (jeu.gagnant >= 0)
                                ? jeu.joueurs[jeu.gagnant].nom
                                : "Personne";

            stats_enregistrer_partie (stats, &nb_stats, noms, jeu.nb_joueurs, nom_gagnant);
            stats_afficher_classement (stats, nb_stats);

            printf("\n  Appuyez sur " BOLD_CYAN "ENTRER" RESET " pour continuer...   👉  ");
            { int c; while ((c = getchar()) != '\n' && c != EOF); }
            clrscr();

    // Menu après une partie

    printf("\n");
    printf(BOLD_WHITE "                             ┌─────────────────────────┐\n" RESET);
    printf(BOLD_CYAN  "                             ✦   🔄 MENU POST-PARTIE   ✦\n" RESET);
    printf(BOLD_WHITE "                             └─────────────────────────┘\n\n" RESET);
    printf(BOLD_CYAN  "                           1️⃣ . Repartir à l'aventure 🚪\n");
    printf(           "                           2️⃣ . Changer d'aventuriers 🌀\n");
    printf(           "                           3️⃣ . Retour au menu principal 📜\n\n"RESET);

    printf(BOLD_CYAN "\n  Appuyez sur 1, 2 ou 3 pour choisir :   👉  "RESET);


            int post_choix = lire_entier(0, 3);

            if (post_choix == 1) {
                memes_joueurs = 1;
                rejouer       = 1;
            } else if (post_choix == 2) {
                memes_joueurs = 0;
                rejouer       = 1;
            } else {
                rejouer     = 0;
                dans_menu   = 1;
            }
        }
    }
    printf("\n\n  Merci d'avoir joué à " BOLD_YELLOW "MEMO-RPG" RESET " ! À bientôt... ⚰️\n\n");
    return 0;
}
