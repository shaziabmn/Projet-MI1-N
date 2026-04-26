#ifndef GAME_TYPES_H
#define GAME_TYPES_H

/* ============================================================
 * game_types.h
 * Shared types, constants, and enumerations for Memo-RPG.
 * Used by all modules (board, player, turn logic, etc.)
 * ============================================================ */

/* ---------- Board dimensions ---------- */
#define BOARD_SIZE   5          /* The labyrinth is a 5x5 grid */
#define TOTAL_CELLS  25         /* Total number of cards on the board */

/* ---------- Card types ---------- */
typedef enum {
    CARD_MONSTER_BASILISK = 0,  /* Defeated by: WEAPON_SHIELD  */
    CARD_MONSTER_ZOMBIE,        /* Defeated by: WEAPON_TORCH   */
    CARD_MONSTER_TROLL,         /* Defeated by: WEAPON_AXE     */
    CARD_MONSTER_HARPY,         /* Defeated by: WEAPON_BOW     */
    CARD_TREASURE,              /* Chest — adventurer needs at least 1 */
    CARD_WEAPON_FIRE_SWORD,     /* Warrior's ancient weapon     */
    CARD_WEAPON_STAFF,          /* Ranger's ancient weapon      */
    CARD_WEAPON_GRIMOIRE,       /* Mage's ancient weapon        */
    CARD_WEAPON_DAGGER,         /* Thief's ancient weapon       */
    CARD_PORTAL,                /* Teleportation portal         */
    CARD_TOTEM,                 /* Transmutation totem          */
    CARD_HIDDEN                 /* Placeholder: card is face-down */
} CardType;

/* ---------- Number of each card type on the board ---------- */
#define NB_BASILISKS  4
#define NB_ZOMBIES    4
#define NB_TROLLS     4
#define NB_HARPIES    4
#define NB_TREASURES  2
#define NB_WEAPONS    4         /* 1 per adventurer class       */
#define NB_PORTALS    1
#define NB_TOTEMS     2

/* ---------- Weapons (chosen each turn by the player) ---------- */
typedef enum {
    WEAPON_SHIELD = 0,  /* Defeats Basilisks (reflecting gaze) */
    WEAPON_TORCH,       /* Defeats Zombies (fire)              */
    WEAPON_AXE,         /* Defeats Trolls (two-handed axe)     */
    WEAPON_BOW,         /* Defeats Harpies (ranged)            */
    NB_WEAPONS_CHOICE   /* Sentinel — number of weapon choices */
} WeaponType;

/* ---------- Adventurer classes ---------- */
typedef enum {
    CLASS_WARRIOR = 0,  /* Seeks: CARD_WEAPON_FIRE_SWORD */
    CLASS_RANGER,       /* Seeks: CARD_WEAPON_STAFF      */
    CLASS_MAGE,         /* Seeks: CARD_WEAPON_GRIMOIRE   */
    CLASS_THIEF,        /* Seeks: CARD_WEAPON_DAGGER     */
    NB_CLASSES          /* Sentinel                      */
} AdventurerClass;

/* ---------- Turn result codes ---------- */
typedef enum {
    TURN_CONTINUE,      /* Player is still alive and moving     */
    TURN_END_DEAD,      /* Player lost to a monster             */
    TURN_END_BLOCKED,   /* No hidden adjacent cells remain      */
    TURN_END_TOTEM,     /* Player hit a totem — turn resets     */
    TURN_VICTORY        /* Player has treasure + ancient weapon */
} TurnResult;

/* ---------- A single card on the board ---------- */
typedef struct {
    CardType type;      /* What is hidden under this card */
    int      revealed;  /* 1 = face-up, 0 = face-down     */
} Card;

/* ---------- Player state ---------- */
typedef struct {
    char             name[64];      /* Player name                            */
    AdventurerClass  class;         /* Warrior / Ranger / Mage / Thief        */
    int              start_row;     /* Starting row on the board              */
    int              start_col;     /* Starting column (border position)      */
    int              row;           /* Current row                            */
    int              col;           /* Current column                         */
    int              has_treasure;  /* 1 if at least one chest has been found */
    int              has_weapon;    /* 1 if the ancient weapon has been found */
    int              is_active;     /* 1 if still in the game                 */
    WeaponType       active_weapon; /* Weapon chosen this turn                */
} Player;

/* ---------- Full game state (shared across modules) ---------- */
typedef struct {
    Card   board[BOARD_SIZE][BOARD_SIZE]; /* The 5x5 labyrinth        */
    Player players[4];                    /* Up to 4 adventurers      */
    int    nb_players;                    /* Actual number of players */
    int    current_player;                /* Index of the active player */
    int    game_over;                     /* 1 when a winner is found */
    int    winner;                        /* Index of the winner, or -1 */
} GameState;

#endif /* GAME_TYPES_H */
