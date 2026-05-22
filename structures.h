#ifndef STRUCTURES_H
#define STRUCTURES_H


// Taille du plateau (5x5 = 25 cases)
#define TAILLE_PLATEAU  5
#define NB_CASES        25

// Nombre maximum de joueurs par partie
#define NB_JOUEURS_MAX  4

// Nombre maximum de joueurs enregistrés dans le fichier de statistiques
#define MAX_JOUEURS_FICHIER  100

// Taille maximale d'un nom
#define NOM_MAX         16

// Nom du fichier de sauvegarde des statistiques
#define FICHIER_STATS   "joueurs.dat"

// Types de cases du plateau
#define CASE_BASILIC    0   
#define CASE_ZOMBIE     1  
#define CASE_TROLL      2   
#define CASE_HARPIE     3   
#define CASE_COFFRE     4   
#define CASE_EPEE_FEU   5   
#define CASE_BATON      6   
#define CASE_GRIMOIRE   7   
#define CASE_DAGUE      8   
#define CASE_PORTAIL    9   
#define CASE_TOTEM      10  


// Armes choisies à chaque tour 
#define ARME_BOUCLIER   0   
#define ARME_TORCHE     1   
#define ARME_HACHE      2   
#define ARME_ARC        3  


// Classes des aventuriers
#define CLASSE_GUERRIER 0   
#define CLASSE_RANGER   1  
#define CLASSE_MAGICIEN 2   
#define CLASSE_VOLEUR   3   


// Résultats possibles d'un tour
#define TOUR_CONTINUE   0   // Le tour continue
#define TOUR_MORT       1   // Le joueur a rencontré un monstre avec la mauvaise arme
#define TOUR_BLOQUE     2   // Le joueur n'a plus de direction possible
#define TOUR_TOTEM      3   // Le joueur a trouvé un totem
#define TOUR_VICTOIRE   4   // Le joueur a trouvé son arme ET le coffre
#define TOUR_ABANDON    5   // Le joueur a choisi d'abandonner la partie


// Structure d'une case du plateau
typedef struct {
    int type;      // Type de la case (CASE_BASILIC, CASE_COFFRE...)
    int revelee;   // 1 = face visible, 0 = face cachée
} Case;


// Structure d'un joueur
typedef struct {
    char nom[NOM_MAX];   // Nom du joueur 
    int  classe;         // Classe du joueur 
    int  ligne_depart;   // Ligne de départ (hors plateau)
    int  col_depart;     // Colonne de départ (hors plateau)
    int  ligne;          // Ligne actuelle du joueur 
    int  col;            // Colonne actuelle du joueur 
    int  a_coffre;       // 1 = le joueur a trouvé le coffre, 0 sinon
    int  a_arme;         // 1 = le joueur a trouvé son arme antique, 0 sinon
    int  arme_active;    // Arme choisie
} Joueur;


// Structure de l'état du jeu 
typedef struct {
    Case   plateau[TAILLE_PLATEAU][TAILLE_PLATEAU]; // Grille 5x5 de cases
    Joueur joueurs[NB_JOUEURS_MAX];                 // Tableau des joueurs
    int    nb_joueurs;    // Nombre de joueurs dans la partie (2 à 4)
    int    joueur_actuel; // Indice du joueur dont c'est le tour 
    int    partie_finie;  // 1 = la partie est terminée, 0 = en cours
    int    gagnant;       // Indice du gagnant, ou -1 si pas de gagnant
} Jeu;


// Structure pour les statistiques d'un joueur 
typedef struct {
    char nom[NOM_MAX]; // Nom du joueur
    int  parties;      // Nombre total de parties jouées
    int  victoires;    // Nombre de victoires
} StatJoueur;


#endif
