#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "structures.h"
#include "affichage.h"
#include "joueur.h"
#include "plateau.h"
#include "tour.h"
#include "statistiques.h"


// Révèle toutes les cases du plateau 
void reveler_tout(Jeu *jeu) {
    int i, j;
    for (i = 0; i < TAILLE_PLATEAU; i++) {
        for (j = 0; j < TAILLE_PLATEAU; j++) {
            jeu->plateau[i][j].revelee = 1;
        }
    }
}


// Programme principal
int main() {
    Jeu         jeu;
    StatJoueur  statistiques[MAX_JOUEURS_FICHIER];
    int         nb_statistiques = 0;

    // Initialise le générateur de nombres aléatoires avec l'heure actuelle
    srand(time(NULL));

    statistiques_charger(statistiques, &nb_statistiques);

    afficher_jeu();

    // Boucle du menu principal
    while (1) {
        afficher_menu();

        int choix_menu = lire_entier(1, 4);

        if (choix_menu == 4) {  // Quitte le jeu
            break;
        }

        if (choix_menu == 3) {  // Affiche les règles
            afficher_regles();
            continue;
        }

        if (choix_menu == 2) {  // Affiche le classement
            clrscr();
            statistiques_afficher_classement(statistiques, nb_statistiques);
            printf("\n  Appuyez sur " GRAS_CYAN "ENTRER" REINIT " pour revenir au menu...   👉  ");
            vider_buffer();
            continue;
        }

        // Choix 1 : lancer une nouvelle partie
        int rejouer       = 1;
        int memes_joueurs = 0;

        // Boucle de parties (permet de rejouer sans repasser par le menu)
        while (rejouer) {
            init_plateau(&jeu);

            if (!memes_joueurs) {
                init_joueurs(&jeu);
            } else {
                reinit_joueurs_pour_rejouer(&jeu);
            }

            chrono_demarrer();

            // Chaque appel joue un tour complet pour le joueur actuel
            while (!jeu.partie_finie) {
                jouer_tour(&jeu);
            }

            // Révèle tout le plateau et l'affiche
            reveler_tout(&jeu);
            afficher_plateau(&jeu);

            // Affichage du résultat
            if (jeu.gagnant >= 0) {
                printf("\n");
                printf(GRAS_BLANC  "                             ┌─────────────────────────┐\n" REINIT);
                printf(GRAS_JAUNE  "                             ✦       🏆 VICTOIRE       ✦\n" REINIT);
                printf(GRAS_BLANC  "                             └─────────────────────────┘\n\n" REINIT);
                printf(GRAS_JAUNE "\n              Félicitations ! " GRAS_BLANC "%s" REINIT " a remporté la partie de Memo-RPG !\n" REINIT, jeu.joueurs[jeu.gagnant].nom);
                printf("                       ");
                chrono_afficher();
                printf("\n");
            } else {
                printf("\n");
                printf(GRAS_BLANC "                             ┌─────────────────────────┐\n" REINIT);
                printf(GRAS_JAUNE "                             ✦        👎 ABANDON       ✦\n" REINIT);
                printf(GRAS_BLANC "                             └─────────────────────────┘\n\n" REINIT);
                printf(GRAS_JAUNE "                          Personne n'a remporté la partie.\n" REINIT);
                printf("                       ");
                chrono_afficher();
                printf("\n");
            }

            // Enregistrement des statistiques
            char *nom_gagnant;
            if (jeu.gagnant >= 0) {
                nom_gagnant = jeu.joueurs[jeu.gagnant].nom;
            } else {
                nom_gagnant = NULL;
            }

            statistiques_enregistrer_partie(statistiques, &nb_statistiques, &jeu, nom_gagnant);
            statistiques_afficher_classement(statistiques, nb_statistiques);

            printf("\n  Appuyez sur " GRAS_CYAN "ENTRER" REINIT " pour continuer...   👉  ");
            vider_buffer();
            clrscr();

            // Menu post-partie
            printf("\n");
            printf(GRAS_BLANC "                             ┌─────────────────────────┐\n" REINIT);
            printf(GRAS_CYAN  "                             ✦   🔄 MENU POST-PARTIE   ✦\n" REINIT);
            printf(GRAS_BLANC "                             └─────────────────────────┘\n\n" REINIT);
            printf(GRAS_CYAN  "                           1️⃣ . Repartir à l'aventure 🚪\n");
            printf(           "                           2️⃣ . Retour au menu principal 📜\n\n" REINIT);
            printf(GRAS_CYAN "\n  Appuyez sur 1 ou 2 pour choisir :   👉  " REINIT);

            int post_choix = lire_entier(1, 2);

            if (post_choix == 1) {  // Rejouer avec les mêmes joueurs
                memes_joueurs = 1;
            } else {                // Retour au menu principal
                rejouer = 0;
            }
        }
    }

    printf("\n\n  Merci d'avoir joué à " GRAS_JAUNE "MEMO-RPG" REINIT " ! À bientôt... ⚰️\n\n");
    return 0;
}
