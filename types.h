#ifndef TYPES_H
#define TYPES_H


// Definitions des types, constantes et structures du jeu
#define TAILLE_PLATEAU  5
#define NB_CASES        25
#define NB_JOUEURS_MAX  4
#define MAX_JOUEURS_DB  64
#define NOM_MAX         16
#define FICHIER_STATS   "joueurs.dat"

// Types de cases 
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

// Armes choisies a chaque tour 
#define ARME_BOUCLIER   0   // Vainc le Basilic
#define ARME_TORCHE     1   // Vainc le Zombie
#define ARME_HACHE      2   // Vainc le Troll
#define ARME_ARC        3   // Vainc la Harpie

// Classes des aventuriers 
#define CLASSE_GUERRIER 0   // Cherche : CASE_EPEE_FEU
#define CLASSE_RANGER   1   // Cherche : CASE_BATON
#define CLASSE_MAGICIEN 2   // Cherche : CASE_GRIMOIRE
#define CLASSE_VOLEUR   3   // Cherche : CASE_DAGUE

// Resultats d'un tour 
#define TOUR_CONTINUE   0
#define TOUR_MORT       1
#define TOUR_BLOQUE     2
#define TOUR_TOTEM      3
#define TOUR_VICTOIRE   4
#define TOUR_ABANDON    5  // Le joueur abandonne la partie

// Structure representant une case du plateau

typedef struct {
    int type;      // Type de la case (CASE_BASILIC, CASE_COFFRE...)
    int revelee;   // 1 = face visible, 0 = face cachee
} Case;

// Structure representant un joueur
typedef struct {
    char nom[NOM_MAX];
    int  classe;
    int  ligne_depart;
    int  col_depart;
    int  ligne;
    int  col;
    int  a_coffre;
    int  a_arme;
    int  actif;
    int  arme_active;
} Joueur;

// Structure representant l'etat complet du jeu
typedef struct {
    Case   plateau[TAILLE_PLATEAU][TAILLE_PLATEAU];
    Joueur joueurs[NB_JOUEURS_MAX];
    int    nb_joueurs;
    int    joueur_actuel;
    int    partie_finie;
    int    gagnant;
} Jeu;

// Structure pour les stats d'un joueur (fichier)
typedef struct {
    char nom[NOM_MAX];
    int  parties;
    int  victoires;
} StatJoueur;

#endif 
