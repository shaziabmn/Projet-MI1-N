# Memo-RPG — Le Labyrinthe du Donjon

Memo-RPG est un jeu de mémoire au tour par tour codé en C, jouable de 2 à 4 joueurs. 
Chaque aventurier doit explorer un labyrinthe de 25 cases mélangées aléatoirement 
pour trouver un coffre au trésor et son arme antique, tout en évitant les monstres.

Réalisé par Cyrine, Marame et Shazia.

## Compilation

Le projet utilise un `Makefile` 

```sh
make            # Compile le binaire memorpg
make clean      # Supprime les .o et le binaire
```

## Lancement

```sh
./memorpg
```

Depuis le menu principal :
1. **Entrer dans le donjon** — démarre une nouvelle partie
2. **Voir le classement** — affiche le tableau des victoires
3. **Règles du jeu** — affiche le contenu de `regles.txt`
4. **Fuir le labyrinthe** — quitte le programme

## Règles

Les règles complètes sont dans [`regles.txt`](regles.txt) 

## Fichiers

| Fichier            | Rôle                                                  |
|--------------------|-------------------------------------------------------|
| `main.c`           | Boucle principale, menu, enchaînement des parties     |
| `affichage.c/.h`   | Écrans d'accueil, menu, règles, codes couleurs ANSI   |
| `joueur.c/.h`      | Joueurs : noms, classes, positions, saisie sécurisée  |
| `plateau.c/.h`     | Plateau 5×5 : initialisation, affichage, mélange      |
| `tour.c/.h`        | Logique d'un tour (arme, déplacement, résolution)     |
| `statistiques.c/.h`| Persistance des stats dans `joueurs.dat`, chronomètre |
| `structures.h`     | Types, constantes, énumérations                       |
| `regles.txt`       | Texte des règles affiché en jeu                       |

