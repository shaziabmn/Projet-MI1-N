#include "display.h"
#include "player.h"


//Fonction pour choisir le nombre de joueurs
int choix_nb_joueur(){

    int nb_joueurs = 0;
    
    do {
        printf(BOLD_WHITE "\n\n\n                             ┌────────────────────────┐ \n"RESET);
        printf(BOLD_MAGENTA"                             ✦  ⚔️  NOMBRE DE JOUEURS  ✦\n"RESET);
        printf(BOLD_WHITE"                             └────────────────────────┘ \n\n"RESET);
        printf(BOLD_MAGENTA"Combien d'aventuriers oseront défier le labyrinthe (2 à 4) ?   👉  "RESET);
        

        // Lecture du choix de l'utilisateur et vérification de sa validité
        if (scanf("%d", &nb_joueurs) != 1 || nb_joueurs < 2 || nb_joueurs > 4){
            printf(RED"❌ Choix invalide. Veuillez entrer un chiffre entre 2 et 4.\n\n"RESET);
            while (getchar() != '\n'); // Vider le buffer
            continue;
        } 
        while (getchar() != '\n');  // Vider le buffer d'entrée
        break;
        
    } while (1); // Continuer à demander tant que l'entrée n'est pas valide
    
    return nb_joueurs;
}


// Fonction pour choisir le nom du joueur
void nom_joueur(Joueur *j, int numero) {


    printf(BOLD_WHITE"\n\n\n                             ┌────────────────────────┐ \n"RESET);
    printf(BOLD_CYAN"                             ✦    👤 AVENTURIER %d     ✦\n"RESET, numero);
    printf(BOLD_WHITE"                             └────────────────────────┘ \n\n"RESET);
    
    printf(BOLD_CYAN"Entrez votre nom (%d caractères max) :   👉  "RESET, TAILLE_NOM - 1);
    
    fgets(j->nom, TAILLE_NOM, stdin); // lit au maximum (TAILLE_NOM - 1) caractères + \0

    // Supprimer le '\n' si présent à la fin
    size_t len = strlen(j->nom);
    if (len > 0 && j->nom[len - 1] == '\n') {
        j->nom[len - 1] = '\0'; // Remplacer le '\n' par un '\0'
        len--;  
    } else {
        // Si pas de '\n', l'utilisateur a tapé plus de 15 caractères
        // Vider le reste du buffer
        while (getchar() != '\n');
    }

    // Limiter strictement à 15 caractères (si TAILLE_NOM = 16)
    if (len > 15) {
        j->nom[15] = '\0';
        len = 15;
    }
    
    // Nom par défaut si vide
    if (j->nom[0] == '\0') {
        sprintf(j->nom, "Aventurier_%d", numero);
    }
}

// Fonction pour afficher la liste des joueurs
void afficher_joueurs(Joueur *liste, int n) {
    printf(BOLD_WHITE "\n\n\n                             ┌────────────────────────┐ \n"RESET);
    printf(BOLD_MAGENTA"                             ✦     ⚔️  AVENTURIERS     ✦\n"RESET);
    printf(BOLD_WHITE"                             └────────────────────────┘ \n\n"RESET);
    
    for (int i = 0; i < n; i++) {
        printf(BOLD_MAGENTA"                                %d. %s\n"RESET, i + 1, liste[i].nom);
    }

    printf(BOLD_WHITE "\n\n\n     🗝  Les portes se referment derrière vous... Il n'y a plus de retour possible. 🗝\n" RESET);
    printf(YELLOW "                           ⚠️  Le labyrinthe vous attend ! ⚠️\n\n" RESET);

}
