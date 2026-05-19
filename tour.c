#include <stdio.h>
#include <string.h>
#include "structures.h"
#include "affichage.h"
#include "joueur.h"
#include "tour.h"
#include "plateau.h"


// Retourne le nom de l'arme
const char *nom_arme(int arme) {
    switch (arme) {
        case ARME_BOUCLIER: return "Bouclier réfléchissant 🪞";
        case ARME_TORCHE:   return "Torche 🔥";
        case ARME_HACHE:    return "Hache de pierre 🪓";
        case ARME_ARC:      return "Arc long 🏹";
        default:            return "Inconnue";
    }
}


// Retourne le nom du type de case
const char *nom_case(int type) {
    switch (type) {
        case CASE_BASILIC:  return "Basilic 🐍";
        case CASE_ZOMBIE:   return "Zombie 🧟";
        case CASE_TROLL:    return "Troll 👹";
        case CASE_HARPIE:   return "Harpie 🦅";
        case CASE_COFFRE:   return "Coffre au trésor 💰";
        case CASE_EPEE_FEU: return "Epée de feu 🗡";
        case CASE_BATON:    return "Bâton de contrôle des familiers 🪃";
        case CASE_GRIMOIRE: return "Grimoire interdit 📖";
        case CASE_DAGUE:    return "Dague de sommeil 🔪";
        case CASE_PORTAIL:  return "Portail magique de téléportation 🌌";
        case CASE_TOTEM:    return "Totem de transmutation 🗿";
        default:            return "Inconnue";
    }
}


// Retourne l'arme antique que doit trouver le joueur selon sa classe
int arme_antique_du_joueur(int classe) {
    switch (classe) {
        case CLASSE_GUERRIER: return CASE_EPEE_FEU;
        case CLASSE_RANGER:   return CASE_BATON;
        case CLASSE_MAGICIEN: return CASE_GRIMOIRE;
        case CLASSE_VOLEUR:   return CASE_DAGUE;
        default:              return -1;
    }
}


// Retourne 1 si l'arme permet de vaincre le monstre, 0 sinon.
int arme_vainc_monstre(int arme, int monstre) {
    if (arme == ARME_BOUCLIER     && monstre == CASE_BASILIC) return 1;
    if (arme == ARME_TORCHE       && monstre == CASE_ZOMBIE)  return 1;
    if (arme == ARME_HACHE && monstre == CASE_TROLL)   return 1;
    if (arme == ARME_ARC     && monstre == CASE_HARPIE)  return 1;
    return 0;
}


// Retourne 1 si le type de case est un monstre, 0 sinon.
int est_monstre(int type) {
    return (type == CASE_BASILIC || type == CASE_ZOMBIE ||
            type == CASE_TROLL   || type == CASE_HARPIE);
}


// Retourne 1 si le type de case est une arme antique, 0 sinon.
int est_arme_antique(int type) {
    return (type == CASE_EPEE_FEU || type == CASE_BATON ||
            type == CASE_GRIMOIRE || type == CASE_DAGUE);
}


// Retourne 1 si la case (ligne, colonne) est dans le plateau ET pas révélée.
int case_jouable(Jeu *jeu, int ligne, int colonne) {
    if (ligne < 0 || ligne >= TAILLE_PLATEAU) {
        return 0;
    }
    if (colonne < 0 || colonne >= TAILLE_PLATEAU) {
        return 0;
    }
    if (jeu->plateau[ligne][colonne].revelee) {
        return 0;
    }
    return 1;
}


// Gère le totem : le joueur choisit une case cachée à échanger avec le totem
void gerer_totem(Jeu *jeu, int ligne, int col) {
    printf("\n\n");
    printf(GRAS_BLANC   "                             ┌─────────────────────────┐\n" REINIT);
    printf(GRAS_MAGENTA "                             ✦  🗿 TOTEM TRANSMUTATION ✦\n" REINIT);
    printf(GRAS_BLANC   "                             └─────────────────────────┘\n\n" REINIT);

    // Collecte toutes les cases cachées sauf la case du totem lui-même
    int lignes[NB_CASES], cols[NB_CASES];
    int count = 0;
    int i, k;
    for (i = 0; i < TAILLE_PLATEAU; i++) {
        for (k = 0; k < TAILLE_PLATEAU; k++) {
            if (!jeu->plateau[i][k].revelee && !(i == ligne && k == col)) {
                lignes[count] = i;
                cols[count]   = k;
                count++;
            }
        }
    }

    if (count == 0) {
        return; // Aucune case à échanger
    }

    // Affiche la liste des cases disponibles
    for (i = 0; i < count; i++) {
        printf(GRAS_BLANC "    [%2d] Ligne %d, Colonne %d\n" REINIT, i + 1, lignes[i], cols[i]);
    }
    printf(GRAS_MAGENTA "\n  Quel est votre choix (1 à %d) ?   👉  " REINIT, count);

    int choix = lire_entier(1, count);
    int sl    = lignes[choix - 1];
    int sc    = cols[choix - 1];

    // Échange les deux cases dans le tableau
    Case tmp                 = jeu->plateau[sl][sc];
    jeu->plateau[sl][sc]     = jeu->plateau[ligne][col];
    jeu->plateau[ligne][col] = tmp;

    // Les deux cases redeviennent cachées après l'échange
    jeu->plateau[sl][sc].revelee     = 0;
    jeu->plateau[ligne][col].revelee = 0;

    printf("  🔁 Les cases ont été échangées. Fin du tour !\n");
}



// Affiche les 4 armes disponibles et l'option d'abandon
int choisir_arme(Jeu *jeu) {
    Joueur *jou = &jeu->joueurs[jeu->joueur_actuel];

    printf("\n\n");
    printf(GRAS_BLANC "                             ┌─────────────────────────┐\n" REINIT);
    printf(GRAS_CYAN  "                             ✦   💣 CHOIX DE L'ARME    ✦\n" REINIT);
    printf(GRAS_BLANC "                             └─────────────────────────┘\n\n" REINIT);

    printf(GRAS_BLANC "                         1️⃣ . Bouclier réfléchissant 🪞 (Basilic 🐍)\n" REINIT);
    printf(GRAS_BLANC "                         2️⃣ . Torche 🔥                 (Zombie 🧟)\n" REINIT);
    printf(GRAS_BLANC "                         3️⃣ . Hache de pierre 🪓        (Troll 👹)\n" REINIT);
    printf(GRAS_BLANC "                         4️⃣ . Arc long 🏹               (Harpie 🦅)\n" REINIT);
    printf(GRAS_BLANC "                         0️⃣ . Abandonner la partie 🏳\n\n" REINIT);

    printf(GRAS_CYAN   "\n  De quelle arme voulez-vous vous munir (0 à 4) ?   👉  "REINIT);

    int choix = lire_entier(0, 4);
    if (choix == 0) {
        return 0; // Le joueur abandonne
    }

    // Stocke l'arme choisie dans le joueur
    jou->arme_active = choix - 1;
    printf("  ✦ Votre arme : %s\n\n", nom_arme(jou->arme_active));
    return 1;
}


// Liste les coordonnées de toutes les cases cachées du plateau et retourne leur nombre
int toutes_cases_cachees(Jeu *jeu, int lignes[], int cols[]) {
    int count = 0;
    int i, j;
    for (i = 0; i < TAILLE_PLATEAU; i++) {
        for (j = 0; j < TAILLE_PLATEAU; j++) {
            if (!jeu->plateau[i][j].revelee) {
                lignes[count] = i;
                cols[count]   = j;
                count++;
            }
        }
    }
    return count;
}


// Étape 2 : le joueur choisit une case adjacente à révéler.
// Retourne 1 si une case a été choisie, 0 si le joueur est bloqué.
int choisir_direction(Jeu *jeu, int *ligne_choisie, int *col_choisie) {
    Joueur *jou = &jeu->joueurs[jeu->joueur_actuel];
    int l = jou->ligne; // Ligne actuelle du joueur
    int c = jou->col;   // Colonne actuelle du joueur
    int nl, nc, choix;

    // Vérifie qu'au moins une direction est jouable
    if (!case_jouable(jeu, l-1, c) &&
        !case_jouable(jeu, l+1, c) &&
        !case_jouable(jeu, l, c-1) &&
        !case_jouable(jeu, l, c+1)) {
        return 0; // Bloqué : aucune case disponible
    }

    printf("\n\n");
    printf(GRAS_BLANC   "                             ┌─────────────────────────┐\n" REINIT);
    printf(GRAS_MAGENTA "                             ✦     🗺️  DÉPLACEMENT      ✦\n" REINIT);
    printf(GRAS_BLANC   "                             └─────────────────────────┘\n\n" REINIT);

    // Affiche chaque direction : disponible (blanc) ou impossible (rouge)
    if (case_jouable(jeu, l-1, c)) {
        printf(GRAS_BLANC "                           1️⃣ . Haut\n"            REINIT);
    } else {
        printf(GRAS_ROUGE "                           1️⃣ . Haut   (impossible)\n" REINIT);
    }

    if (case_jouable(jeu, l+1, c)) {
        printf(GRAS_BLANC "                           2️⃣ . Bas\n"             REINIT);
    } else {
        printf(GRAS_ROUGE "                           2️⃣ . Bas    (impossible)\n" REINIT);
    }

    if (case_jouable(jeu, l, c-1)) {
        printf(GRAS_BLANC "                           3️⃣ . Gauche\n"          REINIT);
    } else {
        printf(GRAS_ROUGE "                           3️⃣ . Gauche (impossible)\n" REINIT);
    }

    if (case_jouable(jeu, l, c+1)) {
        printf(GRAS_BLANC "                           4️⃣ . Droite\n"          REINIT);
    } else {
        printf(GRAS_ROUGE "                           4️⃣ . Droite (impossible)\n" REINIT);
    }

    printf(GRAS_MAGENTA "\n  Où voulez-vous aller (1 à 4) ?   👉  " REINIT);

    // Boucle jusqu'à ce que le joueur choisisse une direction valide
    while (1) {
        choix = lire_entier(1, 4);
        nl = l;
        nc = c;
        if      (choix == 1) { nl = l - 1; } // Haut
        else if (choix == 2) { nl = l + 1; } // Bas
        else if (choix == 3) { nc = c - 1; } // Gauche
        else if (choix == 4) { nc = c + 1; } // Droite

        // Vérifie que la case candidate est dans le plateau
        if (nl < 0 || nl >= TAILLE_PLATEAU || nc < 0 || nc >= TAILLE_PLATEAU) {
            printf(GRAS_ROUGE "  ❌ Bord du plateau ! Choisissez une autre direction :   👉  " REINIT);
            continue;
        }
        // Vérifie que la case n'est pas déjà révélée
        if (jeu->plateau[nl][nc].revelee) {
            printf(GRAS_ROUGE "  ❌ Case déjà révélée ! Choisissez une autre direction :   👉  " REINIT);
            continue;
        }
        // Direction valide : on retourne les coordonnées
        *ligne_choisie = nl;
        *col_choisie   = nc;
        return 1;
    }
}


// Portail magique : le joueur peut choisir n'importe quelle case cachée.
// Retourne 1 si un choix a été fait, 0 si plus aucune case n'est cachée.
int choisir_case_quelconque(Jeu *jeu, int *ligne_choisie, int *col_choisie) {
    int lignes[NB_CASES], cols[NB_CASES];
    int count = toutes_cases_cachees(jeu, lignes, cols);
    if (count == 0) {
        return 0; // Plus aucune case cachée
    }

    printf("\n\n");
    printf(GRAS_BLANC "                             ┌─────────────────────────┐\n" REINIT);
    printf(GRAS_BLEU  "                             ✦   🌌 PORTAIL MAGIQUE    ✦\n" REINIT);
    printf(GRAS_BLANC "                             └─────────────────────────┘\n\n" REINIT);

    // Liste toutes les cases cachées avec leur position
    int i;
    for (i = 0; i < count; i++) {
        printf(GRAS_BLANC "    [%2d] Ligne %d, Colonne %d\n" REINIT,
               i + 1, lignes[i], cols[i]);
    }
    printf(GRAS_BLEU "\n  Quel est votre choix (1 à %d) ?   👉  " REINIT, count);

    int choix = lire_entier(1, count);
    // Récupère les coordonnées de la case choisie
    *ligne_choisie = lignes[choix - 1];
    *col_choisie   = cols[choix - 1];
    return 1;
}


// Étape 3 : révèle la case et applique sa conséquence selon son type
// (monstre, coffre, arme antique, portail, totem).
// Retourne un code TOUR_* selon le résultat du tour.
int resoudre_case(Jeu *jeu, int ligne, int col, int *portail_actif) {
    Joueur *j  = &jeu->joueurs[jeu->joueur_actuel];
    int     type = jeu->plateau[ligne][col].type;

    // Révèle la case et déplace le joueur dessus
    jeu->plateau[ligne][col].revelee = 1;
    j->ligne = ligne;
    j->col   = col;

    // Rafraîchit l'affichage avec la carte révélée
    afficher_plateau(jeu);

    printf(GRAS_JAUNE"\n  ✦ Carte révélée : %s\n"REINIT, nom_case(type));

    // --- Monstre ---
    if (est_monstre(type)) {
        if (arme_vainc_monstre(j->arme_active, type)) {
            printf(GRAS_VERT"  ✅ Votre %s vainc le monstre. Continuez !\n"REINIT, nom_arme(j->arme_active));
        } else {
            printf(GRAS_ROUGE"  ❌ Votre %s est inefficace. %s est vaincu !\n"REINIT, nom_arme(j->arme_active), j->nom);
            return TOUR_MORT; // Fin de tour : le joueur repart au début
        }
    }

    // Coffre
    else if (type == CASE_COFFRE) {
        j->a_coffre = 1;
        if (j->a_arme) {
            printf("  💰 Coffre trouvé ! Vous avez aussi votre arme... la victoire approche !\n");
        } else {
            printf("  💰 Coffre trouvé ! Il reste à trouver votre arme antique.\n");
        }
    }

    // Arme antique
    else if (est_arme_antique(type)) {
        if (type == arme_antique_du_joueur(j->classe)) {
            j->a_arme = 1;
            if (j->a_coffre) {
                printf(GRAS_BLANC"  🏆 C'est VOTRE arme antique ! Vous avez aussi le coffre... la victoire approche !\n"REINIT);
            } else {
                printf(GRAS_BLANC"  🏆 C'est VOTRE arme antique ! Il reste le coffre.\n"REINIT);
            }
        } else {
            printf(GRAS_BLANC"  👎 Ce n'est pas votre arme. Continuez !\n"REINIT);
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
        gerer_totem(jeu, ligne, col);
        return TOUR_TOTEM; // Fin de tour après un totem
    }

    // Victoire 
    if (j->a_coffre && j->a_arme) {
        printf("\n  🎉 VICTOIRE ! %s a trouvé son arme ET un coffre !\n", j->nom);
        return TOUR_VICTOIRE;
    }

    // Joueur bloqué 
    int bloque = !case_jouable(jeu, j->ligne-1, j->col) &&
                 !case_jouable(jeu, j->ligne+1, j->col) &&
                 !case_jouable(jeu, j->ligne, j->col-1) &&
                 !case_jouable(jeu, j->ligne, j->col+1);
    if (bloque && !(*portail_actif)) {
        printf("  🚧 Plus de direction possible. Fin du tour.\n");
        return TOUR_BLOQUE;
    }

    return TOUR_CONTINUE; // Le tour peut continuer
}


// Joue un tour complet pour le joueur actuel :
// choix arme → choix case → résolution, jusqu'à mort/victoire/blocage/abandon.
int jouer_tour(Jeu *jeu) {
    Joueur *j       = &jeu->joueurs[jeu->joueur_actuel];
    int     resultat   = TOUR_CONTINUE;
    int     portail    = 0; // 1 si le portail est actif ce tour

    // Affiche le plateau au début du tour
    afficher_plateau(jeu);

    printf("\n\n");
    printf(GRAS_BLANC "                             ┌─────────────────────────┐\n" REINIT);
    printf("%s                             ✦   🎲 TOUR DU JOUEUR %d   ✦%s\n",
           couleur_joueur(jeu->joueur_actuel), jeu->joueur_actuel + 1, REINIT);
    printf(GRAS_BLANC "                             └─────────────────────────┘\n\n" REINIT);
    printf("  Appuyez sur " GRAS_CYAN "ENTRER" REINIT " pour commencer...   👉  ");
    vider_buffer();

    // Boucle principale du tour
    while (resultat == TOUR_CONTINUE) {

        // Étape 1 : Choisir une arme (ou abandonner)
        afficher_plateau(jeu);
        if (!choisir_arme(jeu)) {
            resultat = TOUR_ABANDON;
            break;
        }

        // Étape 2 : Choisir une case à révéler
        int ligne, col, trouve;

        if (portail) {
            // Portail actif : le joueur peut choisir n'importe quelle case
            trouve = choisir_case_quelconque(jeu, &ligne, &col);
            portail = 0; // Le portail n'est actif qu'un seul coup
        } else {
            // Déplacement normal : case adjacente uniquement
            trouve = choisir_direction(jeu, &ligne, &col);
        }

        if (!trouve) {
            printf("  🚧 Aucune case disponible. Fin du tour.\n");
            resultat = TOUR_BLOQUE;
            break;
        }

        // Étape 3 : Révéler la case et appliquer sa conséquence
        resultat = resoudre_case(jeu, ligne, col, &portail);

        if (resultat == TOUR_CONTINUE) {
            printf("\n  Appuyez sur " GRAS_CYAN "ENTRER" REINIT " pour continuer...   👉  ");
            vider_buffer();
            clrscr();
        }
    }

    // Traitement de la fin du tour
    if (resultat == TOUR_VICTOIRE) {
        // Le joueur a gagné : la partie est terminée
        jeu->partie_finie = 1;
        jeu->gagnant      = jeu->joueur_actuel;

    } else if (resultat == TOUR_ABANDON) {
        // Le joueur abandonne : la partie se termine sans gagnant
        printf(GRAS_BLANC"\n  🏳  %s a abandonné la partie. Fin de la partie !\n"REINIT, j->nom);
        printf("\n  Appuyez sur " GRAS_CYAN "ENTRER" REINIT " pour continuer...   👉  ");
        vider_buffer();
        clrscr();
        jeu->partie_finie = 1;
        jeu->gagnant      = -1;

    } else {
        // Mort, bloqué ou totem : retour à la position de départ, joueur suivant
        printf(GRAS_BLANC"\n  🏁  %s retourne à sa position de départ.\n"REINIT, j->nom);
        printf("\n  Appuyez sur " GRAS_CYAN "ENTRER" REINIT " pour continuer...   👉  ");
        vider_buffer();
        clrscr();
        reinitialiser_plateau(jeu);
        joueur_suivant(jeu);
    }

    return resultat;
}
