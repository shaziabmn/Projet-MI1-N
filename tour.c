// tour.c
// Logique d'un tour de jeu pour Memo-RPG.
//
// Etapes d'un tour :
// 1. Le joueur choisit une arme parmi 4
// 2. Le joueur choisit une case adjacente a reveler
// 3. On applique la consequence (monstre, coffre, arme...)
// 4. On repete jusqu'a la fin du tour (mort/bloque/victoire)

#include <stdio.h>
#include <string.h>
#include "types.h"
#include "display.h"
#include "player.h"
#include "tour.h"
#include "plateau.h"


// Noms et emojis
char *nom_arme(int arme) {
    switch (arme) {
        case ARME_BOUCLIER: return "Miroir 🪞";
        case ARME_TORCHE:   return "Feu 🔥";
        case ARME_HACHE:    return "Hache 🪓";
        case ARME_ARC:      return "Arc 🏹";
        default:            return "Inconnue";
    }
}

char *nom_case(int type) {
    switch (type) {
        case CASE_BASILIC:  return "Basilic 🐍";
        case CASE_ZOMBIE:   return "Zombie 🧟";
        case CASE_TROLL:    return "Troll 🧌";
        case CASE_HARPIE:   return "Harpie 🦅";
        case CASE_COFFRE:   return "Coffre au trésor 💰";
        case CASE_EPEE_FEU: return "Epée de feu 🗡";
        case CASE_BATON:    return "Bâton de contrôle des familiers 🪃";
        case CASE_GRIMOIRE: return "Grimoire interdit 📖";
        case CASE_DAGUE:    return "Dague de sommeil 🗡️";
        case CASE_PORTAIL:  return "Portail magique de téléportation 🌌";
        case CASE_TOTEM:    return "Totem de transmutation 🗿";
        default:            return "Inconnue";
    }
}

// Retourne le type de case correspondant a l'arme antique que cherche le joueur selon sa classe

int arme_antique_du_joueur(int classe) {
    switch (classe) {
        case CLASSE_GUERRIER: return CASE_EPEE_FEU;
        case CLASSE_RANGER:   return CASE_BATON;
        case CLASSE_MAGICIEN: return CASE_GRIMOIRE;
        case CLASSE_VOLEUR:   return CASE_DAGUE;
        default:              return -1;
    }
}

// Retourne 1 si l'arme choisie vainc ce type de monstre

int arme_vainc_monstre(int arme, int monstre) {
    if (arme == ARME_BOUCLIER && monstre == CASE_BASILIC) return 1;
    if (arme == ARME_TORCHE   && monstre == CASE_ZOMBIE)  return 1;
    if (arme == ARME_HACHE    && monstre == CASE_TROLL)   return 1;
    if (arme == ARME_ARC      && monstre == CASE_HARPIE)  return 1;
    return 0;
}

// Retourne 1 si le type est un monstre

int est_monstre(int type) {
    return (type == CASE_BASILIC || type == CASE_ZOMBIE ||  type == CASE_TROLL   || type == CASE_HARPIE);
}

// Retourne 1 si le type est une arme antique

int est_arme_antique(int type) {
    return (type == CASE_EPEE_FEU || type == CASE_BATON || type == CASE_GRIMOIRE || type == CASE_DAGUE);
}

// Lit un entier dans [min, max] de manière securisée

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

// Etape 1 : le joueur choisit une arme

static int choisir_arme(Jeu *jeu) {
    Joueur *jou = &jeu->joueurs[jeu->joueur_actuel];

    printf("\n\n");
    printf(BOLD_WHITE "                             ┌─────────────────────────┐\n" RESET);
    printf(BOLD_CYAN  "                             ✦   💣 CHOIX DE L'ARME    ✦\n" RESET);
    printf(BOLD_WHITE "                             └─────────────────────────┘\n\n" RESET);

    printf(BOLD_WHITE "                           1️⃣ . Miroir 🪞     (Basilic 🐍)\n" RESET);
    printf(BOLD_WHITE "                           2️⃣ . Feu 🔥         (Zombie 🧟)\n" RESET);
    printf(BOLD_WHITE "                           3️⃣ . Hache 🪓        (Troll 🧌)\n" RESET);
    printf(BOLD_WHITE "                           4️⃣ . Arc 🏹         (Harpie 🦅)\n" RESET);
    printf(BOLD_WHITE "                           0️⃣ . Abandonner la partie 🏳\n\n" RESET);

    printf(BOLD_CYAN   "\n  De quelle arme voulez-vous vous munir (0 à 4) ?   👉  "RESET);

    int choix = lire_entier(0, 4);
    if (choix == 0) return 0;
    jou->arme_active = choix - 1;
    printf("  ✦ Votre arme : %s\n\n", nom_arme(jou->arme_active));
    return 1;
}

// Remplit les tableaux lignes[] et cols[] avec les cases
// adjacentes encore cachees autour de la position (l, c).
// Retourne le nombre de cases trouvees.
static int cases_adjacentes_cachees(Jeu *jeu, int l, int c,
                                     int lignes[], int cols[]) {
    int dl[4] = {-1, 1, 0,  0};
    int dc[4] = { 0, 0,-1,  1};
    int count = 0;
    int i;

    for (i = 0; i < 4; i++) {
        int nl = l + dl[i];
        int nc = c + dc[i];
        if (nl < 0 || nl >= TAILLE_PLATEAU) continue;
        if (nc < 0 || nc >= TAILLE_PLATEAU) continue;
        if (!jeu->plateau[nl][nc].revelee) {
            lignes[count] = nl;
            cols[count]   = nc;
            count++;
        }
    }
    return count;
}

// Remplit lignes[] et cols[] avec TOUTES les cases cachees.
// Retourne le nombre de cases trouvees.
static int toutes_cases_cachees(Jeu *jeu, int lignes[], int cols[]) {
    int count = 0;
    int i, j;
    for (i = 0; i < TAILLE_PLATEAU; i++)
        for (j = 0; j < TAILLE_PLATEAU; j++)
            if (!jeu->plateau[i][j].revelee) {
                lignes[count] = i;
                cols[count]   = j;
                count++;
            }
    return count;
}


// Etape 2 : le joueur choisit une case adjacente. Retourne 1 si une case a ete choisie, 0 si bloque

static int choisir_case_adjacente(Jeu *jeu, int *out_l, int *out_c) {
    Joueur *jou = &jeu->joueurs[jeu->joueur_actuel];
    int lignes[4], cols[4];
    int count = cases_adjacentes_cachees(jeu, jou->ligne, jou->col,
                                          lignes, cols);
    if (count == 0) return 0;

    printf("\n\n");
    printf(BOLD_WHITE   "                             ┌─────────────────────────┐\n" RESET);
    printf(BOLD_MAGENTA "                             ✦   🗺️  CHOIX DE LA CASE   ✦\n" RESET);
    printf(BOLD_WHITE   "                             └─────────────────────────┘\n\n" RESET);

    int i;
    for (i = 0; i < count; i++)
        printf(BOLD_WHITE "                           %d️⃣ . Ligne %d, Colonne %d\n" RESET, i + 1, lignes[i], cols[i]);
    printf(BOLD_MAGENTA "\n  Quelle case voulez-vous révéler (1 à %d) ?   👉  " RESET, count);

    int choix = lire_entier(1, count);
    *out_l = lignes[choix - 1];
    *out_c = cols[choix - 1];
    return 1;
    clrscr(); // Efface l'écran
}

// Choix d'une case parmi toutes les cases cachees (portail). Retourne 1 si une case a ete choisie, 0 si aucune case cachee


static int choisir_case_quelconque(Jeu *jeu, int *out_l, int *out_c) {
    int lignes[NB_CASES], cols[NB_CASES];
    int count = toutes_cases_cachees(jeu, lignes, cols);
    if (count == 0) return 0;

    printf("\n\n");
    printf(BOLD_WHITE "                             ┌─────────────────────────┐\n" RESET);
    printf(BOLD_BLUE  "                             ✦   🌌 PORTAIL MAGIQUE    ✦\n" RESET);
    printf(BOLD_WHITE "                             └─────────────────────────┘\n\n" RESET);

    int i;
    for (i = 0; i < count; i++)
        printf(BOLD_WHITE "    [%2d] Ligne %d, Colonne %d\n" RESET,
               i + 1, lignes[i], cols[i]);
     printf(BOLD_BLUE "\n  Quel est votre choix (1 à %d) ?   👉  " RESET, count);

    int choix = lire_entier(1, count);
    *out_l = lignes[choix - 1];
    *out_c = cols[choix - 1];
    return 1;
}


// Etape 3 : Applique la consequence de la case revelee. Retourne le resultat du tour.

static int resoudre_case(Jeu *jeu, int ligne, int col, int *portail_actif) {
    Joueur *j  = &jeu->joueurs[jeu->joueur_actuel];
    int     type = jeu->plateau[ligne][col].type;

    // Révèle la case et déplace le joueur 
    jeu->plateau[ligne][col].revelee = 1;
    j->ligne = ligne;
    j->col   = col;

    // Affiche le plateau avec la carte révélée 
    afficher_plateau(jeu);

    printf(BOLD_YELLOW"\n  ✦ Carte révélée : %s\n"RESET, nom_case(type));

    // Monstre 
    if (est_monstre(type)) {
        if (arme_vainc_monstre(j->arme_active, type)) {
            printf(BOLD_GREEN"  ✅ Votre %s vainc le monstre. Continuez !\n"RESET, nom_arme(j->arme_active));
        } else {
            printf(BOLD_RED"  ❌ Votre %s est inefficace. %s est vaincu !\n"RESET, nom_arme(j->arme_active), j->nom);
            return TOUR_MORT;
        }
    }

    // Coffre
    else if (type == CASE_COFFRE) {
        j->a_coffre = 1;
        printf("  💰 Coffre trouvé ! Il reste a trouver votre arme antique.\n");
    }

    // Arme antique 
    else if (est_arme_antique(type)) {
        if (type == arme_antique_du_joueur(j->classe)) {
            j->a_arme = 1;
            printf(BOLD_WHITE"  🏆 C'est VOTRE arme antique ! Il reste le coffre.\n"RESET);
        } else {
            printf(BOLD_WHITE"  👎 Ce n'est pas votre arme. Continuez !\n"RESET);
        }
    }

    // Portail
    else if (type == CASE_PORTAIL) {
        printf("  🌌 Portail ! Vous pouvez vous téléporter sur n'importe quelle case cachée.\n");
        *portail_actif = 1;
    }

    // Totem 
    else if (type == CASE_TOTEM) {
        printf("  🗿 Totem ! Vous pouvez échanger sa place avec une autre case cachée.\n");

        printf("\n\n");
        printf(BOLD_WHITE   "                             ┌─────────────────────────┐\n" RESET);
        printf(BOLD_MAGENTA "                             ✦  🗿 TOTEM TRANSMUTATION ✦\n" RESET);
        printf(BOLD_WHITE   "                             └─────────────────────────┘\n\n" RESET);

        // Collecte les cases cachees (hors totem actuel) 
        int lignes[NB_CASES], cols[NB_CASES];
        int count = 0;
        int i, j;
        for (i = 0; i < TAILLE_PLATEAU; i++)
            for (j = 0; j < TAILLE_PLATEAU; j++)
                if (!jeu->plateau[i][j].revelee &&
                    !(i == ligne && j == col)) {
                    lignes[count] = i;
                    cols[count]   = j;
                    count++;
                }

        if (count > 0) {
            for (i = 0; i < count; i++)
                printf(BOLD_WHITE "    [%2d] Ligne %d, Colonne %d\n" RESET, i + 1, lignes[i], cols[i]);
            printf(BOLD_MAGENTA "\n  Quel est votre choix (1 à %d) ?   👉  " RESET, count);

            int choix   = lire_entier(1, count);
            int sl      = lignes[choix - 1];
            int sc      = cols[choix - 1];

            // Echange le totem avec la case choisie 
            Case tmp              = jeu->plateau[sl][sc];
            jeu->plateau[sl][sc] = jeu->plateau[ligne][col];
            jeu->plateau[ligne][col] = tmp;

            // Les deux cases redeviennent cachees 
            jeu->plateau[sl][sc].revelee      = 0;
            jeu->plateau[ligne][col].revelee  = 0;

            printf("  🔁 Les cases ont été échangées. Fin du tour !\n");
        }
        return TOUR_TOTEM;
    }

    // Victoire ? 
    if (j->a_coffre && j->a_arme) {
        printf("\n  🎉 VICTOIRE ! %s a trouvé son arme ET un coffre !\n", j->nom);
        return TOUR_VICTOIRE;
    }

    // Joueur bloque ? 
    int lignes_adj[4], cols_adj[4];
    int adj = cases_adjacentes_cachees(jeu, j->ligne, j->col,
                                        lignes_adj, cols_adj);
    if (adj == 0 && !(*portail_actif)) {
        printf("  🚧 Plus de cases adjacentes cachées. Fin du tour.\n");
        return TOUR_BLOQUE;
    }

    return TOUR_CONTINUE;
}

// Tour complet pour le joueur actuel

int jouer_tour(Jeu *jeu) {
    Joueur *j       = &jeu->joueurs[jeu->joueur_actuel];
    int     resultat   = TOUR_CONTINUE;
    int     portail    = 0;

    // Affiche le plateau au debut du tour
    afficher_plateau(jeu);

    {
        const char *coul;
        switch (jeu->joueur_actuel) {
            case 0:  coul = BOLD_GREEN;  break;
            case 1:  coul = BOLD_RED;    break;
            case 2:  coul = BOLD_CYAN;   break;
            case 3:  coul = BOLD_YELLOW; break;
            default: coul = BOLD_WHITE;
        }

        printf("\n\n");
        printf(BOLD_WHITE "                             ┌─────────────────────────┐\n" RESET);
        printf(         "%s                             ✦   🎲 TOUR DU JOUEUR %d   ✦%s\n", coul, jeu->joueur_actuel + 1, RESET);
        printf(BOLD_WHITE "                             └─────────────────────────┘\n\n" RESET);
    }
    printf("  Appuyez sur " BOLD_CYAN "ENTRER" RESET " pour commencer...   👉  ");
    { int c; while ((c = getchar()) != '\n' && c != EOF); }

    while (resultat == TOUR_CONTINUE) {

        // Étape 1 : Choisir une arme (ou abandonner) 
        afficher_plateau(jeu);
        if (!choisir_arme(jeu)) {
            resultat = TOUR_ABANDON;
            break;
        }

        // Étape 2 : Choisir une case
        int ligne, col, trouve;

        if (portail) {
            trouve = choisir_case_quelconque(jeu, &ligne, &col);
            portail = 0;
        } else {
            trouve = choisir_case_adjacente(jeu, &ligne, &col);
        }

        if (!trouve) {
            printf("  🚧 Aucune case disponible. Fin du tour.\n");
            resultat = TOUR_BLOQUE;
            break;
        }

        // Étape 3 : Réveler la case et appliquer 
        resultat = resoudre_case(jeu, ligne, col, &portail);

        if (resultat == TOUR_CONTINUE) {
            printf("\n  Appuyez sur " BOLD_CYAN "ENTRER" RESET " pour continuer...   👉  ");
            { int c; while ((c = getchar()) != '\n' && c != EOF); }
            clrscr();
        }
    }

    // Fin du tour
    if (resultat == TOUR_VICTOIRE) {

        jeu->partie_finie = 1;
        jeu->gagnant      = jeu->joueur_actuel;
        j->actif        = 0;

    } else if (resultat == TOUR_ABANDON) {

        // Abandon : la partie se termine sans gagnant

        printf(BOLD_WHITE"\n  🏳  %s a abandonné la partie. Fin de la partie !\n"RESET, j->nom);
        printf("\n  Appuyez sur " BOLD_CYAN "ENTRER" RESET " pour continuer...   👉  ");
        { int c; while ((c = getchar()) != '\n' && c != EOF); }
        clrscr();
        jeu->partie_finie = 1;
        jeu->gagnant      = -1;
    } else {

        // Mort, bloqué ou totem : reset et joueur suivant
        printf(BOLD_WHITE"\n  🏁  %s retourne à sa position de départ.\n"RESET, j->nom);
        printf("\n  Appuyez sur " BOLD_CYAN "ENTRER" RESET " pour continuer...   👉  ");
        { int c; while ((c = getchar()) != '\n' && c != EOF); }
        clrscr();
        reinitialiser_plateau(jeu);
        joueur_suivant(jeu);
    }

    return resultat;
}

