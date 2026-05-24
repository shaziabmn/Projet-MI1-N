#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structures.h"
#include "affichage.h"
#include "joueur.h"


// Positions de départ des joueurs
//   J1 (haut)   : ligne -1, col  2
//   J2 (bas)    : ligne  5, col  2
//   J3 (gauche) : ligne  2, col -1
//   J4 (droite) : ligne  2, col  5
const int LIGNES_DEPART[4] = {-1,  5,  2,  2};
const int COLS_DEPART[4]   = { 2,  2, -1,  5};


// Couleur attribuée à chaque joueur selon son indice
const char *couleur_joueur(int idx) {
    switch (idx) {
        case 0: return GRAS_VERT;
        case 1: return GRAS_ROUGE;
        case 2: return GRAS_CYAN;
        case 3: return GRAS_JAUNE;
        default: return REINIT;
    }
}


// Emoji de la classe d'un joueur
const char *emoji_classe(int classe) {
    switch (classe) {
        case CLASSE_GUERRIER: return "⚔️  ";
        case CLASSE_RANGER:   return "🏹 ";
        case CLASSE_MAGICIEN: return "🪄 ";
        case CLASSE_VOLEUR:   return "🥷 ";
        default:              return "👤 ";
    }
}


// Nom de la classe d'un joueur
const char *nom_classe(int classe) {
    switch (classe) {
        case CLASSE_GUERRIER: return "Guerrier";
        case CLASSE_RANGER:   return "Ranger";
        case CLASSE_MAGICIEN: return "Magicien";
        case CLASSE_VOLEUR:   return "Voleur";
        default:              return "Inconnu";
    }
}


// Demande le nom d'un joueur
void nom_joueur(Joueur *j, int numero) {

    printf("\n");
    printf(GRAS_BLANC "                             ┌─────────────────────────┐\n" REINIT);
    printf(         "%s                             ✦    👤 AVENTURIER %d      ✦%s\n", couleur_joueur(numero - 1), numero, REINIT);
    printf(GRAS_BLANC "                             └─────────────────────────┘\n\n" REINIT);

    printf("%s  Entrez votre nom (%d caractères max) :   👉  %s",
           couleur_joueur(numero - 1), NOM_MAX - 1, REINIT);

    scanf("%15s", j->nom);
    vider_buffer();
}


// Vide le reste du buffer d'entrée jusqu'au saut de ligne
void vider_buffer(void) {
    int c;
    while ((c = getchar()) != '\n');
}


// Lit un entier dans [min, max]
int lire_entier(int min, int max) {
    int valeur;
    while (1) {
        if (scanf("%d", &valeur) == 1 && valeur >= min && valeur <= max) {
            vider_buffer();
            return valeur;
        }
        vider_buffer();
        printf(ROUGE "  ❓ Choix invalide. Veuillez entrer un nombre entre %d et %d :  👉  " REINIT, min, max);
    }
}


// Demande les noms et assigne classes et positions des joueurs
void init_joueurs(Jeu *jeu) {
    int i;

    clrscr();
    printf("\n");
    printf(GRAS_BLANC   "                             ┌─────────────────────────┐\n" REINIT);
    printf(GRAS_MAGENTA "                             ✦   ⚔️  NOMBRE DE JOUEURS  ✦\n" REINIT);
    printf(GRAS_BLANC   "                             └─────────────────────────┘\n\n" REINIT);
    printf(GRAS_MAGENTA"  Combien d'aventuriers oseront défier le labyrinthe (2 à 4) ?   👉  " REINIT);

    jeu->nb_joueurs = lire_entier(2, 4);

    clrscr();

    printf(GRAS_BLANC "\n                           🌱   Les portes s'ouvrent...  🌱\n" REINIT);
    printf(JAUNE "                    🌙  %d aventuriers entreront dans le donjon ! 🌙\n\n" REINIT, jeu->nb_joueurs);

    for (i = 0; i < jeu->nb_joueurs; i++) {
        nom_joueur(&jeu->joueurs[i], i + 1);

        // La classe est assignée automatiquement selon l'indice du joueur
        jeu->joueurs[i].classe = i;
        printf("  ✦ Classe assignée : %s %s\n", emoji_classe(i), nom_classe(i));

        // Placement à la position de départ
        jeu->joueurs[i].ligne_depart = LIGNES_DEPART[i];
        jeu->joueurs[i].col_depart   = COLS_DEPART[i];
        jeu->joueurs[i].ligne        = LIGNES_DEPART[i];
        jeu->joueurs[i].col          = COLS_DEPART[i];

        jeu->joueurs[i].a_coffre     = 0;
        jeu->joueurs[i].a_arme       = 0;
        jeu->joueurs[i].arme_active  = 0;
    }

    jeu->joueur_actuel = 0;
}


// Remet les joueurs à leur position de départ sans redemander les noms
void reinit_joueurs_pour_rejouer(Jeu *jeu) {
    int i;
    for (i = 0; i < jeu->nb_joueurs; i++) {
        jeu->joueurs[i].ligne_depart = LIGNES_DEPART[i];
        jeu->joueurs[i].col_depart   = COLS_DEPART[i];
        jeu->joueurs[i].ligne        = LIGNES_DEPART[i];
        jeu->joueurs[i].col          = COLS_DEPART[i];
        jeu->joueurs[i].a_coffre     = 0;
        jeu->joueurs[i].a_arme       = 0;
        jeu->joueurs[i].arme_active  = 0;
    }
    jeu->joueur_actuel = 0;
}


// Passe au joueur suivant
void joueur_suivant(Jeu *jeu) {
    jeu->joueur_actuel = jeu->joueur_actuel + 1;
    if (jeu->joueur_actuel >= jeu->nb_joueurs) {
        jeu->joueur_actuel = 0;
    }
}
