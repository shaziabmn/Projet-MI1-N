# Memo-RPG — Le Labyrinthe du Donjon

Jeu de plateau en C jouable au terminal (2 à 4 joueurs), mélangeant mémoire et
choix tactique. Chaque joueur incarne un aventurier d'une classe (Guerrier,
Ranger, Magicien, Voleur) et doit retrouver SON arme antique ainsi qu'un coffre
avant les autres, en révélant les cases d'un plateau 5×5.

Réalisé par Cyrine, Marame et Shazia.

## Compilation

Le projet utilise un `Makefile` et `gcc` (avec `-Wall -Wextra`).

```sh
make            # Compile le binaire memorpg
make clean      # Supprime les .o, le binaire et le fichier de statistiques
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

Les règles complètes sont dans [`regles.txt`](regles.txt) (accessibles aussi
depuis le menu du jeu).

## Fichiers

| Fichier            | Rôle                                                  |
|--------------------|-------------------------------------------------------|
| `main.c`           | Boucle principale, menu, enchaînement des parties     |
| `affichage.c/.h`   | Écrans d'accueil, menu, règles, codes couleurs ANSI   |
| `joueur.c/.h`      | Joueurs : noms, classes, positions, saisie sécurisée  |
| `plateau.c/.h`     | Plateau 5×5 : initialisation, affichage, mélange      |
| `tour.c/.h`        | Logique d'un tour (arme, déplacement, résolution)     |
| `statistiques.c/.h`| Statistiques dans `joueurs.dat`, chronomètre          |
| `structures.h`     | Types, constantes, énumérations                       |
| `regles.txt`       | Texte des règles affiché en jeu                       |
