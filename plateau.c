#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "display.h"
#include "stats.h"
#include "player.h"
#include "plateau.h"


// Emoji d'une case selon son type et son etat
// Toutes les cellules sont rendues sur 3 colonnes visuelles
// (un emoji "large" = 2 colonnes + 1 espace, ou " ◈ " pour les caches)
static const char *emoji_case(int type, int revelee) {
    if (!revelee) return " ◈ ";
    switch (type) {
        case CASE_BASILIC:  return BOLD_RED    "🐍 " RESET;
        case CASE_ZOMBIE:   return BOLD_GREEN  "🧟 " RESET;
        case CASE_TROLL:    return BOLD_YELLOW "🧌 " RESET;
        case CASE_HARPIE:   return BOLD_MAGENTA"🦅 " RESET;
        case CASE_COFFRE:   return BOLD_YELLOW "💰 " RESET;
        case CASE_EPEE_FEU: return BOLD_RED    "🗡  " RESET;
        case CASE_BATON:    return BOLD_GREEN  "🪃 " RESET;
        case CASE_GRIMOIRE: return BOLD_BLUE   "📖 " RESET;
        case CASE_DAGUE:    return BOLD_CYAN   "🔪 " RESET;
        case CASE_PORTAIL:  return BOLD_CYAN   "🌌 " RESET;
        case CASE_TOTEM:    return BOLD_MAGENTA"🗿 " RESET;
        default:            return " ? ";
    }
}


// Melange Fisher-Yates

static void melanger(int *tableau, int taille) {
    int i, j, tmp;
    for (i = taille - 1; i > 0; i--) {
        j          = rand() % (i + 1);
        tmp        = tableau[i];
        tableau[i] = tableau[j];
        tableau[j] = tmp;
    }
}


// Initialise le plateau avec les 25 cartes melangees

void init_plateau(Jeu *jeu) {
    int deck[NB_CASES];
    int i, idx = 0;

    for (i = 0; i < 4; i++) deck[idx++] = CASE_BASILIC;
    for (i = 0; i < 4; i++) deck[idx++] = CASE_ZOMBIE;
    for (i = 0; i < 4; i++) deck[idx++] = CASE_TROLL;
    for (i = 0; i < 4; i++) deck[idx++] = CASE_HARPIE;
    for (i = 0; i < 2; i++) deck[idx++] = CASE_COFFRE;
    deck[idx++] = CASE_EPEE_FEU;
    deck[idx++] = CASE_BATON;
    deck[idx++] = CASE_GRIMOIRE;
    deck[idx++] = CASE_DAGUE;
    deck[idx++] = CASE_PORTAIL;
    for (i = 0; i < 2; i++) deck[idx++] = CASE_TOTEM;

    melanger(deck, NB_CASES);

    for (i = 0; i < TAILLE_PLATEAU; i++) {
        int j;
        for (j = 0; j < TAILLE_PLATEAU; j++) {
            jeu->plateau[i][j].type    = deck[i * TAILLE_PLATEAU + j];
            jeu->plateau[i][j].revelee = 0;
        }
    }

    jeu->partie_finie = 0 ;
    jeu->gagnant      = -1 ;

}


// Affichage de la légende

static void afficher_legende_plateau(void) {
    printf(BOLD_YELLOW"\n                           ✦ ───────── LÉGENDE ───────── ✦\n\n"RESET);

    printf("                  🐍 = Basilic  🧟 = Zombie  🧌 = Troll  🦅= Harpie\n");
    printf("                  🗡  = Épée  🪃  = Bâton  📖 = Grimoire  🔪 = Dague\n");
    printf("                  🌌 = Portail  🗿 = Totem   💰 = Coffre  ◈ = Caché\n\n");

    printf(BOLD_YELLOW"                           ✦ ─────────────────────────── ✦\n\n"RESET);
}


 // Affiche le plateau complet :
 // - titre
 // - timer
 // - tags [J1..J4] sur les 4 cotes (couleurs)
 // - grille avec joueurs sur leurs cases courantes
 // - bandeau des joueurs en bas (nom, statut)
 // - legende
 //
 //
 // Marges utilisees pour aligner toute la grille
 // MARGE_BORD : avant la bordure ┌ et avant │ (lignes de cellules)
 // MARGE_TAG  : avant les tags [J1] [J2] (colonne 2 du plateau)
 // Les tags [J3] / [J4] sont colles a gauche/droite des bordures.
 //
 //
 // /* MARGE_BORD = indent avant la bordure ┌/├/└ et avant │ (31 espaces)
#define MARGE_BORD "                               "
// MARGE_NUM = indent avant le numero de colonne 0 (33 espaces)
#define MARGE_NUM  "                                 "
// MARGE_TAGV = indent avant les tags verticaux [J1]/[J2] (40 espaces)
#define MARGE_TAGV "                                        "


// Affichage du labyrinthe

void afficher_plateau(Jeu *jeu) {
    int i, j, p;

    clrscr(); // Efface l'écran

    printf("\n");
    printf(BOLD_WHITE "                             ┌─────────────────────────┐\n" RESET);
    printf(BOLD_GREEN "                             ✦     🍃  LABYRINTHE      ✦\n" RESET);
    printf(BOLD_WHITE "                             └─────────────────────────┘\n\n" RESET);

    // Timer
    printf("                       ");
    timer_afficher();
    printf("\n");

    // Numeros de colonnes 
    // Border ┌ a la col 32, centre cellule 0 = col 34
    // On v eut "0" en col 34 -> 33 espaces avant "0"
    printf(MARGE_NUM);
    for (j = 0; j < TAILLE_PLATEAU; j++)
        printf(BOLD_GREEN "%d   " RESET, j);
        printf("\n");

    // Bordure superieure 
    printf(MARGE_BORD "┌");
    for (j = 0; j < TAILLE_PLATEAU; j++) {
        printf("───");
        if (j < TAILLE_PLATEAU - 1) printf("┬");
    }
    printf("┐\n");

    // Lignes du plateau 
    for (i = 0; i < TAILLE_PLATEAU; i++) {
        // Indent ligne : 28 espaces + "N  " (3 chars) = 31 chars (MARGE_BORD) 
        printf("                            ");
        printf(BOLD_GREEN "%d" RESET "  │", i);

        for(j = 0; j < TAILLE_PLATEAU; j++) {
            const char *contenu   = NULL;
            const char *couleur_j = NULL;
            for (p = 0; p < jeu->nb_joueurs; p++) {
                if (jeu->joueurs[p].actif &&
                    jeu->joueurs[p].ligne == i &&
                    jeu->joueurs[p].col   == j) {
                    contenu   = emoji_classe(jeu->joueurs[p].classe);
                    couleur_j = couleur_joueur(p);
                    break;
                }
            }

            // Chaque cellule = 3 colonnes (emoji 2 wide + 1 espace, ou " ◈ ") 
            if (contenu == NULL)
                printf("%s│", emoji_case(jeu->plateau[i][j].type,
                                          jeu->plateau[i][j].revelee));
            else
                printf("%s%s%s│", couleur_j, contenu, RESET);
        }

        printf("\n");

        if (i < TAILLE_PLATEAU - 1) {
            printf(MARGE_BORD "├");
            for (j = 0; j < TAILLE_PLATEAU; j++) {
                printf("───");
                if (j < TAILLE_PLATEAU - 1) printf("┼");
            }
            printf("┤\n");
        }
    }

    // Bordure inferieure 
    printf(MARGE_BORD "└");
    for (j = 0; j < TAILLE_PLATEAU; j++) {
        printf("───");
        if (j < TAILLE_PLATEAU - 1) printf("┴");
    }
    printf("┘\n\n");

    // Bandeau des joueurs (toujours visible) 
    // Affiche pour chaque joueur : couleur, classe, nom,
    // arme antique cherchee, et drapeau coffre/arme trouvee
    printf(BOLD_WHITE "  Joueurs :\n" RESET);
    for (i = 0; i < jeu->nb_joueurs; i++) {
        Joueur *j = &jeu->joueurs[i];
        const char *fleche = (i == jeu->joueur_actuel && !jeu->partie_finie)
                             ? BOLD_YELLOW "▶ " RESET : "  ";
        const char *cible_emoji = "❓";
        switch (j->classe) {
            case CLASSE_GUERRIER: cible_emoji = "🗡 "; break;
            case CLASSE_RANGER:   cible_emoji = "🪃"; break;
            case CLASSE_MAGICIEN: cible_emoji = "📖"; break;
            case CLASSE_VOLEUR:   cible_emoji = "🔪"; break;
        }
        printf("    %s%s[J%d]%s %s%-10s  cherche %s   %s %s\n",
               fleche,
               couleur_joueur(i), i + 1, RESET,
               emoji_classe(j->classe),
               j->nom,
               cible_emoji,
               j->a_coffre ? "💰" : "⬜",
               j->a_arme   ? "🏆" : "⬜");
    }
    printf("\n");

    afficher_legende_plateau();
}

// Cache toutes les cartes et renvoie le joueur actuel a son depart

void reinitialiser_plateau(Jeu *jeu) {
    int i, j;
    for (i = 0; i < TAILLE_PLATEAU; i++)
        for (j = 0; j < TAILLE_PLATEAU; j++)
            jeu->plateau[i][j].revelee = 0;

    Joueur *jou = &jeu->joueurs[jeu->joueur_actuel];
    jou->ligne  = jou->ligne_depart;
    jou->col    = jou->col_depart;
}
