#include "board.h"


// Initialisation du labyrinthe 
void creer_labyrinthe(Case grille[TAILLE][TAILLE]) {
    // Tableau avec les 25 types de cases
    int types[25] = {
        // 4 Basilics (0)
        0, 0, 0, 0,
        // 4 Zombies (1)
        1, 1, 1, 1,
        // 4 Trolls (2)
        2, 2, 2, 2,
        // 4 Harpies (3)
        3, 3, 3, 3,
        // 2 Coffres (4)
        4, 4,
        // 4 Armes antiques (5, 6, 7, 8)
        5, 6, 7, 8,
        // 1 Portail (9)
        9,
        // 2 Totems (10)
        10, 10
    };
    
    // Mélange 
    for (int i = 24; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = types[i];
        types[i] = types[j];
        types[j] = temp;
    }
    
    // Remplir la grille
    int k = 0;
    for (int i = 0; i < TAILLE; i++) {
        for (int j = 0; j < TAILLE; j++) {
            grille[i][j].type = types[k++];
            grille[i][j].visible = 0;  // Tout caché au début
        }
    }
}

void afficher_symbole(int type) {
    switch (type) {
        case 0: printf(BOLD_RED"🐍 "RESET); break;      // Basilic
        case 1: printf(BOLD_GREEN"🧟 "RESET); break;    // Zombie
        case 2: printf(BOLD_YELLOW"🧌  "RESET); break;  // Troll
        case 3: printf(BOLD_MAGENTA"🦅 "RESET); break;  // Harpie
        case 4: printf(BOLD_YELLOW"💰 "RESET); break;   // Coffre
        case 5: printf(BOLD_RED"🗡  "RESET); break;     // Épée
        case 6: printf(BOLD_GREEN"🪃  "RESET); break;   // Bâton
        case 7: printf(BOLD_BLUE"📖 "RESET); break;     // Grimoire
        case 8: printf(BOLD_CYAN"🔪️ "RESET); break;     // Dague
        case 9: printf(BOLD_CYAN"🌌 "RESET); break;     // Portail
        case 10: printf(BOLD_MAGENTA"🗿 "RESET); break; // Totem
        default: printf(" ? "); break;
    }
}

// Affichage de la légende
void afficher_legende() {
    printf(BOLD_YELLOW"\n                           ✦ ───────── LÉGENDE ───────── ✦\n\n"RESET);
    
    printf("                  🐍 = Basilic  🧟 = Zombie  🧌 = Troll  🦅= Harpie\n");
    printf("                  🗡  = Épée  🪃  = Bâton  📖 = Grimoire  🔪 = Dague\n");
    printf("                  🌌 = Portail  🗿 = Totem  💰 = Coffre  ◈ = Caché\n\n");
    
    printf(BOLD_YELLOW"                           ✦ ─────────────────────────── ✦\n\n"RESET);
}

// Affichage du labyrinthe
void afficher_labyrinthe(Case grille[TAILLE][TAILLE]) {
    printf(BOLD_WHITE"\n\n                             ┌────────────────────────┐ \n"RESET);
    printf(BOLD_GREEN"                             ✦    🍃  LABYRINTHE      ✦\n"RESET);
    printf(BOLD_WHITE"                             └────────────────────────┘ \n\n\n"RESET);
    
    // Numéros de colonnes
    printf("                                  ");  
    for (int j = 0; j < TAILLE; j++) {
        printf(BOLD_GREEN"%d   " RESET, j);
    }
    printf("\n");
    
    // Bordure supérieure
    printf("                               ┌");  
    for (int j = 0; j < TAILLE; j++) {
        printf("───");
        if (j < TAILLE - 1) printf("┬");
    }
    printf("┐\n");
    
    // Afficher chaque ligne
    for (int i = 0; i < TAILLE; i++) {
        printf("                            "BOLD_GREEN"%d  "RESET"│", i);  
        
        for (int j = 0; j < TAILLE; j++) {
            if (grille[i][j].visible) {
                // Case révélée : afficher le symbole
                afficher_symbole(grille[i][j].type);
            } else {
                // Case cachée : afficher un symbole
                printf(" ◈ "); 
            }
            if (j < TAILLE - 1) printf("│");
        }
        printf("│\n");
        
        // Bordure intermédiaire (sauf dernière ligne)
        if (i < TAILLE - 1) {
            printf("                               ├");  
            for (int j = 0; j < TAILLE; j++) {
                printf("───");
                if (j < TAILLE - 1) printf("┼");
            }
            printf("┤\n");
        }
    }
    
    // Bordure inférieure
    printf("                               └");  // 31 espaces
    for (int j = 0; j < TAILLE; j++) {
        printf("───");
        if (j < TAILLE - 1) printf("┴");
    }
    printf("┘\n\n\n");
    
    // Légende
    afficher_legende();
}
