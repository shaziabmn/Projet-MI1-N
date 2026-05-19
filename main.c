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


// Révèle toutes les cases du plateau (appelée en fin de partie)
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
    int         dans_menu       = 1; // 1 = on reste dans le menu principal
    int         rejouer         = 0; // 1 = on relance une partie
    int         memes_joueurs   = 0; // 1 = on rejoue avec les mêmes joueurs

    // Initialise le générateur de nombres aléatoires avec l'heure actuelle
    srand((unsigned int)time(NULL));

    statistiques_charger(statistiques, &nb_statistiques);

    afficher_jeu();

    // Boucle du menu principal
    while (dans_menu) {
        afficher_menu();

        int choix_menu = lire_entier(1, 4);

        if (choix_menu == 4) {  // Quitte le jeu
            dans_menu = 0;
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
            clrscr();
            continue;
        }

        // Choix 1 : lancer une nouvelle partie
        rejouer       = 1;
        memes_joueurs = 0;

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

            // Fin de partie : révèle tout le plateau et l'affiche
            reveler_tout(&jeu);
            afficher_plateau(&jeu);

            // Affichage du résultat (victoire ou abandon)
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

            // Enregistrement des statistiques (le gagnant est NOM_AUCUN_GAGNANT si abandon)
            char *nom_gagnant;
            if (jeu.gagnant >= 0) {
                nom_gagnant = jeu.joueurs[jeu.gagnant].nom;
            } else {
                nom_gagnant = NOM_AUCUN_GAGNANT;
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
            printf(           "                           2️⃣ . Changer d'aventuriers 🌀\n");
            printf(           "                           3️⃣ . Retour au menu principal 📜\n\n" REINIT);
            printf(GRAS_CYAN "\n  Appuyez sur 1, 2 ou 3 pour choisir :   👉  " REINIT);

            int post_choix = lire_entier(1, 3);

            if (post_choix == 1) {        // Rejouer avec les mêmes joueurs
                memes_joueurs = 1;
                rejouer       = 1;
            } else if (post_choix == 2) { // Choisir de nouveaux joueurs
                memes_joueurs = 0;
                rejouer       = 1;
            } else {                      // Retour au menu principal
                rejouer   = 0;
                dans_menu = 1;
            }
        }
    }

    printf("\n\n  Merci d'avoir joué à " GRAS_JAUNE "MEMO-RPG" REINIT " ! À bientôt... ⚰️\n\n");
    return 0;
}
