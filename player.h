#ifndef PLAYER_H
#define PLAYER_H


#define TAILLE_NOM 16


typedef struct {

    char nom[TAILLE_NOM];
    //autres éléments à ajouter pour le joueur 

} Joueur;



// Déclaration des fonctions
void nom_joueur(Joueur *j, int numero);
int choix_nb_joueur();
void afficher_joueurs(Joueur *liste, int n);

#endif
