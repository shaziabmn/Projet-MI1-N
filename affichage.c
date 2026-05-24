#include "affichage.h"
#include "joueur.h"


// Affiche l'écran d'accueil
void afficher_jeu() {

    clrscr();
    printf("\n");
    printf(GRAS_VERT"             ･✧･ﾟ: *✧･ﾟ:* ･ﾟ✧･ﾟ: *✧･ﾟ:*･ﾟ✧･ﾟ: *✧･ﾟ:* ･ﾟ✧･ﾟ: *✧･ﾟ:* ･ﾟ:* ･\n"REINIT);
    printf(GRAS_VERT"             ✦"REINIT" ╔══════════════════════════════════════════════════════╗ "GRAS_VERT"✦\n"REINIT);
    printf(GRAS_VERT"             ✦"REINIT" ║                                                      ║ "GRAS_VERT"✦\n"REINIT);
    printf(GRAS_VERT"             ✦"REINIT" ║         " CYAN "🏰    BIENVENUE SUR" REINIT " " GRAS_JAUNE " MEMO-RPG " REINIT "   🏰          ║ "GRAS_VERT"✦\n"REINIT);
    printf(GRAS_VERT"             ✦"REINIT" ║                                                      ║ "GRAS_VERT"✦\n"REINIT);
    printf(GRAS_VERT"             ✦"REINIT" ║             " GRAS_MAGENTA "  Le Labyrinthe du Donjon" REINIT "                ║ "GRAS_VERT"✦\n"REINIT);
    printf(GRAS_VERT"             ✦"REINIT" ║                                                      ║ "GRAS_VERT"✦\n"REINIT);
    printf(GRAS_VERT"             ✦"REINIT" ╚══════════════════════════════════════════════════════╝ "GRAS_VERT"✦\n"REINIT);
    printf(GRAS_VERT"             ･✧･ﾟ: *✧･ﾟ:* ･ﾟ✧･ﾟ: *✧･ﾟ:*･ﾟ✧･ﾟ: *✧･ﾟ:* ･ﾟ✧･ﾟ: *✧･ﾟ:* ･ﾟ:* ･\n"REINIT);
    printf(GRAS_BLANC"\n\n                          🌿  Entrez si vous l'osez...  🌿\n" REINIT);
    printf(JAUNE    "                    ✨ Réalisé par Cyrine, Marame et Shazia ✨\n\n\n\n" REINIT);

    printf("  Appuyez sur " GRAS_CYAN "ENTRER" REINIT " pour continuer...   👉  ");
    vider_buffer();
}


// Affiche le menu principal
void afficher_menu() {

    clrscr();
    printf("\n");
    printf(GRAS_BLANC "                             ┌─────────────────────────┐\n" REINIT);
    printf(GRAS_CYAN  "                             ✦    📜 MENU PRINCIPAL    ✦\n" REINIT);
    printf(GRAS_BLANC "                             └─────────────────────────┘\n\n" REINIT);
    printf(GRAS_CYAN  "                           1️⃣ . Entrer dans le donjon 🚪\n");
    printf(           "                           2️⃣ . Voir le classement 🏆\n");
    printf(           "                           3️⃣ . Règles du jeu 📖\n");
    printf(           "                           4️⃣ . Fuir le labyrinthe 🔙\n\n"REINIT);

    printf(GRAS_CYAN "\n  Appuyez sur 1, 2, 3 ou 4 pour choisir :   👉  "REINIT);

}


// Lit et affiche les règles du jeu depuis le fichier regles.txt
void afficher_regles() {
    clrscr();
    printf("\n");
    printf(GRAS_BLANC "                                 ┌─────────────────────────┐\n" REINIT);
    printf(GRAS_JAUNE "                                 ✦     📚 RÈGLES DU JEU    ✦\n" REINIT);
    printf(GRAS_BLANC "                                 └─────────────────────────┘\n\n" REINIT);

    FILE *f = fopen("regles.txt", "r");
    if (f == NULL) {
        printf(ROUGE "\n  ❓ Fichier regles.txt introuvable.\n" REINIT);
    } else {
        char ligne[256];
        while (fgets(ligne, sizeof(ligne), f) != NULL) {
            printf("%s", ligne);
        }
        fclose(f);
    }

    printf("\n\n  Appuyez sur " GRAS_CYAN "ENTRER" REINIT " pour revenir au menu...   👉  ");
    vider_buffer();
}
