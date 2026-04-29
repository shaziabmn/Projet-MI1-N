#include "display.h"
#include "player.h"
#include "board.h"

int main() { 

    Joueur j[4];  // Tableau de 4 joueurs
    Case labyrinthe[TAILLE][TAILLE];
    srand(time(NULL));

    afficher_jeu();

    int choix_menu = 0; // Variable pour vérifier si l'entrée de l'utilisateur est valide

    do {

        // Affiche le menu principal
        afficher_menu();

        // Lecture du choix de l'utilisateur et vérification de sa validité
        if (scanf("%d", &choix_menu) != 1 || (choix_menu != 1 && choix_menu != 2)) {
            printf(RED "❌ Choix invalide. Veuillez entrer '1' pour lancer le jeu ou '2' pour quitter\n\n" RESET);
            while (getchar() != '\n');  // Vider le buffer
            continue;
        }
         while (getchar() != '\n');  // Vider le buffer 

        
        // Si l'utilisateur choisit de lancer le jeu
        if (choix_menu == 1) {
            
            int nb_joueurs = choix_nb_joueur();

            printf(BOLD_WHITE "\n\n\n                           🌱   Les portes s'ouvrent...  🌱\n" RESET);
            printf(YELLOW "                    🌙  %d aventuriers entreront dans le donjon ! 🌙\n" RESET, nb_joueurs);
            
            

            // Demander le nom de chaque joueur
            for (int i = 0; i < nb_joueurs; i++) {
                nom_joueur(&j[i], i+1);
            }

            afficher_joueurs(j, nb_joueurs);

            // Créer et afficher le labyrinthe

            creer_labyrinthe(labyrinthe);
            afficher_labyrinthe(labyrinthe);
        
            //appeler une fonction qui gère la logique du jeu (Personne 2)
            break;

            
        }

    } while (choix_menu != 2); // Continuer à afficher le menu tant que l'utilisateur ne choisit pas de quitter

    printf("\n\n\nMerci d'avoir joué à " BOLD_YELLOW "MEMO-RPG" RESET " ! À bientôt... ⚰️\n\n");
   
    


    return 0;
}
