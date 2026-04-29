#include "display.h"



// Procédure pour afficher l'écran d'accueil du jeu
void afficher_jeu() {

printf(BOLD_GREEN"\n\n\n             ･✧･ﾟ: *✧･ﾟ:* ･ﾟ✧･ﾟ: *✧･ﾟ:*･ﾟ✧･ﾟ: *✧･ﾟ:* ･ﾟ✧･ﾟ: *✧･ﾟ:* ･ﾟ:* ･\n"RESET);
printf(BOLD_GREEN"             ◇"RESET" ╔══════════════════════════════════════════════════════╗ "BOLD_GREEN"◇\n"RESET);
printf(BOLD_GREEN"             ◇"RESET" ║                                                      ║ "BOLD_GREEN"◇\n"RESET);
printf(BOLD_GREEN"             ◇"RESET" ║         " CYAN "🏰    BIENVENUE SUR" RESET " " BOLD_YELLOW " MEMO-RPG " RESET "   🏰          ║ "BOLD_GREEN"◇\n"RESET);
printf(BOLD_GREEN"             ◇"RESET" ║                                                      ║ "BOLD_GREEN"◇\n"RESET);
printf(BOLD_GREEN"             ◇"RESET" ║             " BOLD_MAGENTA "  Le Labyrinthe du Donjon" RESET "                ║ "BOLD_GREEN"◇\n"RESET);
printf(BOLD_GREEN"             ◇"RESET" ║                                                      ║ "BOLD_GREEN"◇\n"RESET);
printf(BOLD_GREEN"             ◇"RESET" ╚══════════════════════════════════════════════════════╝ "BOLD_GREEN"◇\n"RESET);
printf(BOLD_GREEN"             ･✧･ﾟ: *✧･ﾟ:* ･ﾟ✧･ﾟ: *✧･ﾟ:*･ﾟ✧･ﾟ: *✧･ﾟ:* ･ﾟ✧･ﾟ: *✧･ﾟ:* ･ﾟ:* ･\n"RESET);
printf(BOLD_WHITE "\n\n                          🌿  Entrez si vous l'osez...  🌿\n" RESET);
printf(YELLOW "                    ✨ Réalisé par Cyrine, Marame et Shazia ✨\n\n\n\n" RESET);

printf("Appuyez sur " BOLD_CYAN "ENTRER" RESET " pour continuer...   👉  ");
while (getchar() != '\n');

clrscr(); // Efface l'écran
}

// Procédure pour afficher le menu principal
void afficher_menu() {

    printf(BOLD_WHITE "\n\n\n\n                             ┌────────────────────────┐ \n"RESET);
    printf(BOLD_CYAN"                             ✦   📜 MENU PRINCIPAL    ✦\n"RESET);
    printf(BOLD_WHITE"                             └────────────────────────┘ \n\n"RESET);
    printf(BOLD_CYAN "                           1️⃣ . Entrer dans le donjon 🚪\n");
    printf("                           2️⃣ . Fuir le labyrinthe 🔙\n\n"RESET);

    printf(BOLD_CYAN "\nAppuyez sur 1 ou 2 pour choisir :   👉  "RESET);

}
