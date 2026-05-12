// player.c
// Gestion des joueurs : couleurs, classes, initialisation, rotation

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "display.h"
#include "player.h"


// Couleur attribuee a chaque joueur selon son indice
//   J1 = vert, J2 = rouge, J3 = cyan, J4 = jaune
const char *couleur_joueur(int idx) {
    switch (idx) {
        case 0: return BOLD_GREEN;
        case 1: return BOLD_RED;
        case 2: return BOLD_CYAN;
        case 3: return BOLD_YELLOW;
        default: return RESET;
    }
}


// Emoji de la classe d'un joueur (affiche dans la case)
const char *emoji_classe(int classe) {
    switch (classe) {
        case CLASSE_GUERRIER: return "⚔️  ";
        case CLASSE_RANGER:   return "🏹 ";
        case CLASSE_MAGICIEN: return "🪄 ";
        case CLASSE_VOLEUR:   return "🥷 ";
        default:              return "👤 ";
    }
}


// Nom de la classe (pour affichage)
const char *nom_classe(int classe) {
    switch (classe) {
        case CLASSE_GUERRIER: return "Guerrier";
        case CLASSE_RANGER:   return "Ranger";
        case CLASSE_MAGICIEN: return "Magicien";
        case CLASSE_VOLEUR:   return "Voleur";
        default:              return "Inconnu";
    }
}


// Demande le nom du joueur, en gerant proprement les saisies trop longues
void nom_joueur(Joueur *j, int numero) {


    printf("\n");
    printf(BOLD_WHITE "                             ┌─────────────────────────┐\n" RESET);
    printf(         "%s                             ✦    👤 AVENTURIER %d      ✦%s\n", couleur_joueur(numero - 1), numero, RESET);
    printf(BOLD_WHITE "                             └─────────────────────────┘\n\n" RESET);

    printf("%s  Entrez votre nom (%d caracteres max) :   👉  %s",
           couleur_joueur(numero - 1), NOM_MAX - 1, RESET);

    fgets(j->nom, NOM_MAX, stdin); // lit au maximum (NOM_MAX - 1) caracteres + \0

    // Supprimer le '\n' si present a la fin
    size_t len = strlen(j->nom);
    if (len > 0 && j->nom[len - 1] == '\n') {
        j->nom[len - 1] = '\0';
        len--;
    } else {
        // Pas de '\n' => l'utilisateur a tape plus que la limite
        // On vide le reste du buffer
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
    }

    // Nom par defaut si vide
    if (j->nom[0] == '\0') {
        snprintf(j->nom, NOM_MAX, "Aventurier_%d", numero);
    }
}


// Lit un entier dans [min, max] de maniere securisee
static int lire_entier_securise(int min, int max) {
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


// Demande noms et assigne classes/positions des joueurs
// Positions de depart : EN DEHORS du plateau, sur les 4 cotes
//   J1 (haut)   : ligne -1, col 2
//   J2 (bas)    : ligne  5, col 2
//   J3 (gauche) : ligne  2, col -1
//   J4 (droite) : ligne  2, col  5
void init_joueurs(Jeu *jeu) {

    int i;
    int lignes_depart[4] = {-1,  5,  2,  2};
    int cols_depart[4]   = { 2,  2, -1,  5};

    clrscr(); // Efface l'écran
    printf("\n");
    printf(BOLD_WHITE   "                             ┌─────────────────────────┐\n" RESET);
    printf(BOLD_MAGENTA "                             ✦   ⚔️  NOMBRE DE JOUEURS  ✦\n" RESET);
    printf(BOLD_WHITE   "                             └─────────────────────────┘\n\n" RESET);
    printf(BOLD_MAGENTA"  Combien d'aventuriers oseront defier le labyrinthe (2 à 4) ?   👉  " RESET);

    jeu->nb_joueurs = lire_entier_securise(2, 4);
    
    clrscr(); // Efface l'écran

    printf(BOLD_WHITE "\n                           🌱   Les portes s'ouvrent...  🌱\n" RESET);
    printf(YELLOW "                    🌙  %d aventuriers entreront dans le donjon ! 🌙\n\n" RESET, jeu->nb_joueurs);

    for (i = 0; i < jeu->nb_joueurs; i++) {

        nom_joueur(&jeu->joueurs[i], i + 1);

        jeu->joueurs[i].classe = i;
        printf("  ✦ Classe assignée : %s %s\n", emoji_classe(i), nom_classe(i));

        jeu->joueurs[i].ligne_depart = lignes_depart[i];
        jeu->joueurs[i].col_depart   = cols_depart[i];
        jeu->joueurs[i].ligne        = lignes_depart[i];
        jeu->joueurs[i].col          = cols_depart[i];
        jeu->joueurs[i].a_coffre     = 0;
        jeu->joueurs[i].a_arme       = 0;
        jeu->joueurs[i].actif        = 1;
        jeu->joueurs[i].arme_active  = 0;
    }

    jeu->joueur_actuel = 0;
}


// Remet les joueurs a leur depart sans redemander les noms
void reinit_joueurs_pour_rejouer(Jeu *jeu) {
    int i;
    int lignes_depart[4] = {-1,  5,  2,  2};
    int cols_depart[4]   = { 2,  2, -1,  5};

    for (i = 0; i < jeu->nb_joueurs; i++) {
        jeu->joueurs[i].ligne_depart = lignes_depart[i];
        jeu->joueurs[i].col_depart   = cols_depart[i];
        jeu->joueurs[i].ligne        = lignes_depart[i];
        jeu->joueurs[i].col          = cols_depart[i];
        jeu->joueurs[i].a_coffre     = 0;
        jeu->joueurs[i].a_arme       = 0;
        jeu->joueurs[i].actif        = 1;
        jeu->joueurs[i].arme_active  = 0;
    }
    jeu->joueur_actuel = 0;
}


// Passe au joueur actif suivant (tourne en boucle)
void joueur_suivant(Jeu *jeu) {
    int depart = jeu->joueur_actuel;
    do {
        jeu->joueur_actuel = (jeu->joueur_actuel + 1) % jeu->nb_joueurs;
        if (jeu->joueur_actuel == depart) break;
    } while (!jeu->joueurs[jeu->joueur_actuel].actif);
}
