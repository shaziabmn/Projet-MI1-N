#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structures.h"
#include "affichage.h"
#include "statistiques.h"
#include "joueur.h"
#include "plateau.h"


// Retourne l'emoji à afficher pour une case
const char *emoji_case(int type, int revelee) {
    if (!revelee) {
        return " ◈ ";
    }
    switch (type) {
        case CASE_BASILIC:  return GRAS_ROUGE   "🐍 " REINIT;
        case CASE_ZOMBIE:   return GRAS_VERT    "🧟 " REINIT;
        case CASE_TROLL:    return GRAS_JAUNE   "👹 " REINIT;
        case CASE_HARPIE:   return GRAS_MAGENTA "🦅 " REINIT;
        case CASE_COFFRE:   return GRAS_JAUNE   "💰 " REINIT;
        case CASE_EPEE_FEU: return GRAS_ROUGE   "🗡  " REINIT;
        case CASE_BATON:    return GRAS_VERT    "🪃 " REINIT;
        case CASE_GRIMOIRE: return GRAS_BLEU    "📖 " REINIT;
        case CASE_DAGUE:    return GRAS_CYAN    "🔪 " REINIT;
        case CASE_PORTAIL:  return GRAS_CYAN    "🌌 " REINIT;
        case CASE_TOTEM:    return GRAS_MAGENTA "🗿 " REINIT;
        default:            return              " ? ";
    }
}


// Mélange un tableau d'entiers
void melanger(int *tableau, int taille) {
    int i, j, tmp;
    for (i = taille - 1; i > 0; i--) {
        j          = rand() % (i + 1);
        tmp        = tableau[i];
        tableau[i] = tableau[j];
        tableau[j] = tmp;
    }
}


// Initialise les 25 cartes mélangées aléatoirement
void init_plateau(Jeu *jeu) {
    int paquet[NB_CASES] = {
        CASE_BASILIC, CASE_BASILIC, CASE_BASILIC, CASE_BASILIC,
        CASE_ZOMBIE,  CASE_ZOMBIE,  CASE_ZOMBIE,  CASE_ZOMBIE,
        CASE_TROLL,   CASE_TROLL,   CASE_TROLL,   CASE_TROLL,
        CASE_HARPIE,  CASE_HARPIE,  CASE_HARPIE,  CASE_HARPIE,
        CASE_COFFRE,  CASE_COFFRE,
        CASE_EPEE_FEU, CASE_BATON, CASE_GRIMOIRE, CASE_DAGUE,
        CASE_PORTAIL,
        CASE_TOTEM, CASE_TOTEM
    };

    // Mélange le paquet
    melanger(paquet, NB_CASES);

    // Distribue les cartes dans le plateau 5x5
    int i, j;
    for (i = 0; i < TAILLE_PLATEAU; i++) {
        for (j = 0; j < TAILLE_PLATEAU; j++) {
            jeu->plateau[i][j].type    = paquet[i * TAILLE_PLATEAU + j];
            jeu->plateau[i][j].revelee = 0; // face cachée
        }
    }

    // Réinitialise l'état de fin de partie
    jeu->partie_finie = 0;
    jeu->gagnant      = -1;
}


// Affiche la légende 
void afficher_legende_plateau() {
    printf(GRAS_JAUNE"\n                           ✦ ───────── LÉGENDE ───────── ✦\n\n"REINIT);

    printf(           "                  🐍 = Basilic  🧟 = Zombie   👹 = Troll  🦅= Harpie\n");
    printf(           "                  🗡  = Épée  🪃  = Bâton   📖 = Grimoire  🔪 = Dague\n");
    printf(           "                  🌌 = Portail  🗿 = Totem    💰 = Coffre  ◈ = Caché\n\n");

    printf(GRAS_JAUNE"                           ✦ ─────────────────────────── ✦\n\n"REINIT);
}


// Affiche le plateau avec couleurs, position des joueurs et chrono
void afficher_plateau(Jeu *jeu) {
    int i, j;

    clrscr();

    printf("\n");
    printf(GRAS_BLANC "                             ┌─────────────────────────┐\n" REINIT);
    printf(GRAS_VERT  "                             ✦     🍃  LABYRINTHE      ✦\n" REINIT);
    printf(GRAS_BLANC "                             └─────────────────────────┘\n\n" REINIT);

    // Affiche le chronomètre de la manche
    printf("                       ");
    chrono_afficher();
    printf("\n");

    // J1 au-dessus de la colonne 2
    if (jeu->nb_joueurs >= 1) {
        printf("                                        %s%s\n" REINIT,
               couleur_joueur(0), emoji_classe(jeu->joueurs[0].classe));
    } else {
        printf("\n");
    }

    // Numéros de colonnes (0 à 4)
    printf("                                 ");
    for (j = 0; j < TAILLE_PLATEAU; j++) {
        printf(GRAS_VERT "%d   " REINIT, j);
    }
    printf("\n");

    // Bordure supérieure de la grille
    printf("                               ┌");
    for (j = 0; j < TAILLE_PLATEAU; j++) {
        printf("───");
        if (j < TAILLE_PLATEAU - 1) {
            printf("┬");
        }
    }
    printf("┐\n");

    // Lignes du plateau
    for (i = 0; i < TAILLE_PLATEAU; i++) {

        // J3 à gauche de la ligne 2, J4 à droite de la ligne 2
        if (i == 2 && jeu->nb_joueurs >= 3) {
            printf("                        %s%s" REINIT " ", couleur_joueur(2), emoji_classe(jeu->joueurs[2].classe));
        } else {
            printf("                            ");
        }

        // Numéro de ligne
        printf(GRAS_VERT "%d" REINIT "  │", i);

        // Cases de la ligne : révélée ou cachée
        for (j = 0; j < TAILLE_PLATEAU; j++) {
            printf("%s│", emoji_case(jeu->plateau[i][j].type, jeu->plateau[i][j].revelee));
        }

        // J4 à droite de la ligne 2
        if (i == 2 && jeu->nb_joueurs >= 4) {
            printf("  %s%s" REINIT, couleur_joueur(3), emoji_classe(jeu->joueurs[3].classe));
        }

        printf("\n");

        // Séparateur entre les lignes
        if (i < TAILLE_PLATEAU - 1) {
            printf("                               ├");
            for (j = 0; j < TAILLE_PLATEAU; j++) {
                printf("───");
                if (j < TAILLE_PLATEAU - 1) {
                    printf("┼");
                }
            }
            printf("┤\n");
        }
    }

    // Bordure inférieure de la grille
    printf("                               └");
    for (j = 0; j < TAILLE_PLATEAU; j++) {
        printf("───");
        if (j < TAILLE_PLATEAU - 1) {
            printf("┴");
        }
    }
    printf("┘\n");

    // J2 en dessous de la colonne 2
    if (jeu->nb_joueurs >= 2) {
        printf("                                        %s%s\n" REINIT, couleur_joueur(1), emoji_classe(jeu->joueurs[1].classe));
    } else {
        printf("\n");
    }
    printf("\n");

    // Bandeau des joueurs : nom, classe, statut coffre et arme
    printf(GRAS_BLANC "  Joueurs :\n" REINIT);
    for (i = 0; i < jeu->nb_joueurs; i++) {
        Joueur *jou = &jeu->joueurs[i];

        // Emoji de l'arme antique recherchée
        const char *cible_emoji = "❓";
        switch (jou->classe) {
            case CLASSE_GUERRIER: cible_emoji = "🗡 "; break;
            case CLASSE_RANGER:   cible_emoji = "🪃"; break;
            case CLASSE_MAGICIEN: cible_emoji = "📖"; break;
            case CLASSE_VOLEUR:   cible_emoji = "🔪"; break;
        }

        // Affiche : [COFFRE] et [ARME] si obtenus
        const char *statut_coffre;
        if (jou->a_coffre) {
            statut_coffre = GRAS_JAUNE "[COFFRE]" REINIT;
        } else {
            statut_coffre = "        ";
        }

        const char *statut_arme;
        if (jou->a_arme) {
            statut_arme = GRAS_VERT "[ARME]  " REINIT;
        } else {
            statut_arme = "        ";
        }

        printf("    %s[J%d]%s %s%-10s  cherche %s  %s  %s\n",
               couleur_joueur(i), i + 1, REINIT,
               emoji_classe(jou->classe), jou->nom,
               cible_emoji,
               statut_coffre, statut_arme);
    }
    printf("\n");

    afficher_legende_plateau();
}


// Cache toutes les cartes et renvoie le joueur actuel à sa position de départ
void reinitialiser_plateau(Jeu *jeu) {
    int i, j;
    for (i = 0; i < TAILLE_PLATEAU; i++) {
        for (j = 0; j < TAILLE_PLATEAU; j++) {
            jeu->plateau[i][j].revelee = 0;
        }
    }

    Joueur *jou = &jeu->joueurs[jeu->joueur_actuel];
    jou->ligne  = jou->ligne_depart;
    jou->col    = jou->col_depart;
}
