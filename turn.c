/* ============================================================
 * turn.c
 * Implementation of the turn logic module for Memo-RPG.
 *
 * Responsible for:
 *   - Weapon selection each turn
 *   - Cell reveal and consequence resolution
 *     (monster combat, treasure, ancient weapon, portal, totem)
 *   - Victory / death / block / totem detection
 *   - Board reset between turns
 *
 * Author (Person 2): turn logic & game flow
 * ============================================================ */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game_types.h"
#include "turn.h"

/* ============================================================
 * Internal helpers — display utilities
 * ============================================================ */

/* Prints a coloured header line to separate game sections */
static void print_separator(const char *label) {
    printf("\n\033[1;33m══════════════════════════════════════\033[0m\n");
    if (label && label[0] != '\0') {
        printf("\033[1;33m  %s\033[0m\n", label);
        printf("\033[1;33m══════════════════════════════════════\033[0m\n");
    }
}

/* ============================================================
 * Weapon helpers
 * ============================================================ */

const char *weapon_name(WeaponType w) {
    switch (w) {
        case WEAPON_SHIELD: return "Reflecting Shield";
        case WEAPON_TORCH:  return "Torch";
        case WEAPON_AXE:    return "Two-Handed Axe";
        case WEAPON_BOW:    return "Long Bow";
        default:            return "Unknown Weapon";
    }
}

const char *weapon_emoji(WeaponType w) {
    switch (w) {
        case WEAPON_SHIELD: return "🛡️ ";
        case WEAPON_TORCH:  return "🔥";
        case WEAPON_AXE:    return "🪓";
        case WEAPON_BOW:    return "🏹";
        default:            return "❓";
    }
}

/*
 * weapon_defeats()
 * Maps each weapon to the single monster type it can defeat.
 *   Shield  → Basilisk (reflects gaze)
 *   Torch   → Zombie   (fire)
 *   Axe     → Troll    (devastating blow)
 *   Bow     → Harpy    (ranged attack)
 */
int weapon_defeats(WeaponType w, CardType card) {
    switch (w) {
        case WEAPON_SHIELD: return (card == CARD_MONSTER_BASILISK);
        case WEAPON_TORCH:  return (card == CARD_MONSTER_ZOMBIE);
        case WEAPON_AXE:    return (card == CARD_MONSTER_TROLL);
        case WEAPON_BOW:    return (card == CARD_MONSTER_HARPY);
        default:            return 0;
    }
}

/* ============================================================
 * Card helpers
 * ============================================================ */

const char *card_name(CardType c) {
    switch (c) {
        case CARD_MONSTER_BASILISK:   return "Basilisk 🐍";
        case CARD_MONSTER_ZOMBIE:     return "Zombie 🧟";
        case CARD_MONSTER_TROLL:      return "Troll 👹";
        case CARD_MONSTER_HARPY:      return "Harpy 🦅";
        case CARD_TREASURE:           return "Treasure Chest 💰";
        case CARD_WEAPON_FIRE_SWORD:  return "Fire Sword ⚔️ ";
        case CARD_WEAPON_STAFF:       return "Familiar Staff 🪄";
        case CARD_WEAPON_GRIMOIRE:    return "Forbidden Grimoire 📖";
        case CARD_WEAPON_DAGGER:      return "Sleep Dagger 🗡️ ";
        case CARD_PORTAL:             return "Magic Portal 🌀";
        case CARD_TOTEM:              return "Transmutation Totem 🗿";
        case CARD_HIDDEN:             return "Hidden Cell";
        default:                      return "Unknown";
    }
}

int is_monster(CardType c) {
    return (c == CARD_MONSTER_BASILISK ||
            c == CARD_MONSTER_ZOMBIE   ||
            c == CARD_MONSTER_TROLL    ||
            c == CARD_MONSTER_HARPY);
}

int is_ancient_weapon(CardType c) {
    return (c == CARD_WEAPON_FIRE_SWORD ||
            c == CARD_WEAPON_STAFF      ||
            c == CARD_WEAPON_GRIMOIRE   ||
            c == CARD_WEAPON_DAGGER);
}

/*
 * ancient_weapon_for_class()
 * Each adventurer class seeks exactly one ancient weapon.
 */
CardType ancient_weapon_for_class(AdventurerClass cls) {
    switch (cls) {
        case CLASS_WARRIOR: return CARD_WEAPON_FIRE_SWORD;
        case CLASS_RANGER:  return CARD_WEAPON_STAFF;
        case CLASS_MAGE:    return CARD_WEAPON_GRIMOIRE;
        case CLASS_THIEF:   return CARD_WEAPON_DAGGER;
        default:            return CARD_HIDDEN; /* Should never happen */
    }
}

/* Returns the name of an adventurer class */
static const char *class_name(AdventurerClass cls) {
    switch (cls) {
        case CLASS_WARRIOR: return "Warrior ⚔️ ";
        case CLASS_RANGER:  return "Ranger 🏹";
        case CLASS_MAGE:    return "Mage 🧙";
        case CLASS_THIEF:   return "Thief 🗡️ ";
        default:            return "Unknown";
    }
}

/* ============================================================
 * Adjacency / movement helpers
 * ============================================================ */

/*
 * get_adjacent_hidden()
 * Scans the 4 orthogonal neighbours of (r, c).
 * Returns only those that are within bounds AND still face-down.
 */
int get_adjacent_hidden(const GameState *gs, int r, int c,
                        int rows[4], int cols[4]) {
    /* Direction deltas: up, down, left, right */
    int dr[4] = {-1,  1, 0, 0};
    int dc[4] = { 0,  0,-1, 1};
    int count = 0;

    for (int i = 0; i < 4; i++) {
        int nr = r + dr[i];
        int nc = c + dc[i];
        /* Check bounds */
        if (nr < 0 || nr >= BOARD_SIZE) continue;
        if (nc < 0 || nc >= BOARD_SIZE) continue;
        /* Check that the cell is still hidden */
        if (!gs->board[nr][nc].revealed) {
            rows[count] = nr;
            cols[count] = nc;
            count++;
        }
    }
    return count;
}

/*
 * get_all_hidden()
 * Returns all cells on the board that are still face-down.
 * Used for portal jumps and totem swaps.
 */
int get_all_hidden(const GameState *gs, int rows[25], int cols[25]) {
    int count = 0;
    for (int r = 0; r < BOARD_SIZE; r++) {
        for (int c = 0; c < BOARD_SIZE; c++) {
            if (!gs->board[r][c].revealed) {
                rows[count] = r;
                cols[count] = c;
                count++;
            }
        }
    }
    return count;
}

/* ============================================================
 * Input helpers — safe integer input
 * ============================================================ */

/*
 * read_int_in_range()
 * Reads an integer from stdin in [min, max].
 * Loops until a valid value is entered, preventing crashes on bad input.
 */
static int read_int_in_range(int min, int max) {
    int value;
    char buffer[64];

    while (1) {
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            /* EOF or read error — treat as minimum valid choice */
            return min;
        }
        if (sscanf(buffer, "%d", &value) == 1) {
            if (value >= min && value <= max) {
                return value;
            }
        }
        printf("  ⚠️  Please enter a number between %d and %d: ", min, max);
    }
}

/* ============================================================
 * Weapon selection
 * ============================================================ */

/*
 * choose_weapon()
 * Displays the 4 weapon options and stores the player's choice.
 * This is STEP #1 of every turn.
 */
void choose_weapon(GameState *gs) {
    Player *p = &gs->players[gs->current_player];

    print_separator("CHOOSE YOUR WEAPON");
    printf("  Adventurer: \033[1;36m%s\033[0m (%s)\n\n", p->name, class_name(p->class));

    for (int i = 0; i < NB_WEAPONS_CHOICE; i++) {
        printf("  [%d] %s %s\n", i + 1, weapon_emoji((WeaponType)i),
               weapon_name((WeaponType)i));
    }

    printf("\n  Your choice (1-%d): ", NB_WEAPONS_CHOICE);
    int choice = read_int_in_range(1, NB_WEAPONS_CHOICE);

    p->active_weapon = (WeaponType)(choice - 1);
    printf("\n  ✅  You equip the \033[1;32m%s\033[0m.\n",
           weapon_name(p->active_weapon));
}

/* ============================================================
 * Cell selection
 * ============================================================ */

/*
 * choose_cell_adjacent()
 * STEP #2 (normal move): player picks one of the hidden cells
 * adjacent to their current position.
 * Returns 0 if no adjacent hidden cell exists (player is blocked).
 */
int choose_cell_adjacent(GameState *gs, int *out_row, int *out_col) {
    Player *p = &gs->players[gs->current_player];
    int adj_rows[4], adj_cols[4];
    int count = get_adjacent_hidden(gs, p->row, p->col, adj_rows, adj_cols);

    if (count == 0) {
        return 0; /* Blocked — no moves available */
    }

    print_separator("REVEAL A CELL");
    printf("  Current position: row %d, col %d\n", p->row + 1, p->col + 1);
    printf("  Adjacent hidden cells:\n\n");

    for (int i = 0; i < count; i++) {
        printf("  [%d] Row %d, Col %d\n", i + 1,
               adj_rows[i] + 1, adj_cols[i] + 1);
    }

    printf("\n  Your choice (1-%d): ", count);
    int choice = read_int_in_range(1, count);

    *out_row = adj_rows[choice - 1];
    *out_col = adj_cols[choice - 1];
    return 1;
}

/*
 * choose_cell_any_hidden()
 * Used after a portal is activated: the player may jump to ANY
 * face-down cell, not just adjacent ones.
 * Returns 0 if no hidden cells remain on the board.
 */
int choose_cell_any_hidden(GameState *gs, int *out_row, int *out_col) {
    int all_rows[25], all_cols[25];
    int count = get_all_hidden(gs, all_rows, all_cols);

    if (count == 0) {
        return 0;
    }

    print_separator("PORTAL — CHOOSE ANY HIDDEN CELL");
    printf("  The portal lets you jump anywhere!\n");
    printf("  Available hidden cells:\n\n");

    for (int i = 0; i < count; i++) {
        printf("  [%2d] Row %d, Col %d\n", i + 1,
               all_rows[i] + 1, all_cols[i] + 1);
    }

    printf("\n  Your choice (1-%d): ", count);
    int choice = read_int_in_range(1, count);

    *out_row = all_rows[choice - 1];
    *out_col = all_cols[choice - 1];
    return 1;
}

/*
 * choose_cell_for_totem_swap()
 * Internal helper: for the totem effect, the player picks any hidden
 * cell to swap the totem with (without revealing the target cell).
 * Returns 0 if no other hidden cell exists.
 */
static int choose_cell_for_totem_swap(GameState *gs, int totem_row, int totem_col,
                                       int *swap_row, int *swap_col) {
    int all_rows[25], all_cols[25];
    int count = 0;

    /* Collect all hidden cells, excluding the totem's own position */
    for (int r = 0; r < BOARD_SIZE; r++) {
        for (int c = 0; c < BOARD_SIZE; c++) {
            if (!gs->board[r][c].revealed &&
                !(r == totem_row && c == totem_col)) {
                all_rows[count] = r;
                all_cols[count] = c;
                count++;
            }
        }
    }

    if (count == 0) {
        return 0;
    }

    print_separator("TOTEM — CHOOSE A CELL TO SWAP WITH");
    printf("  The totem will be exchanged with a hidden cell.\n");
    printf("  The chosen cell stays face-down after the swap.\n\n");

    for (int i = 0; i < count; i++) {
        printf("  [%2d] Row %d, Col %d\n", i + 1,
               all_rows[i] + 1, all_cols[i] + 1);
    }

    printf("\n  Your choice (1-%d): ", count);
    int choice = read_int_in_range(1, count);

    *swap_row = all_rows[choice - 1];
    *swap_col = all_cols[choice - 1];
    return 1;
}

/* ============================================================
 * Cell consequence resolution
 * ============================================================ */

/*
 * resolve_cell()
 * The heart of the turn logic.
 * Reveals (row, col), then applies the appropriate consequence:
 *
 *   Monster     → weapon check → survive or die
 *   Treasure    → set has_treasure flag
 *   Anc. weapon → set has_weapon if it's the player's own
 *   Portal      → set portal_active flag for next move
 *   Totem       → swap + end turn
 *   Other       → continue
 *
 * After every resolution, checks whether the player is blocked
 * (all adjacent cells revealed), and whether victory is achieved.
 */
TurnResult resolve_cell(GameState *gs, int row, int col, int *portal_active) {
    Player *p        = &gs->players[gs->current_player];
    Card   *cell     = &gs->board[row][col];
    CardType type    = cell->type;

    /* Reveal the cell */
    cell->revealed = 1;

    /* Move player onto this cell */
    p->row = row;
    p->col = col;

    printf("\n  🎴  You reveal: \033[1;35m%s\033[0m\n", card_name(type));

    /* ---- MONSTER ---- */
    if (is_monster(type)) {
        if (weapon_defeats(p->active_weapon, type)) {
            printf("\n  ⚔️   Your \033[1;32m%s\033[0m defeats the %s!\n",
                   weapon_name(p->active_weapon), card_name(type));
            /* Player survives — check for block or victory below */
        } else {
            printf("\n  💀  Your \033[1;31m%s\033[0m is ineffective against the %s!\n",
                   weapon_name(p->active_weapon), card_name(type));
            printf("      \033[1;31m%s is defeated and sent back to start!\033[0m\n",
                   p->name);
            return TURN_END_DEAD;
        }
    }

    /* ---- TREASURE CHEST ---- */
    else if (type == CARD_TREASURE) {
        if (!p->has_treasure) {
            p->has_treasure = 1;
            printf("  💰  You found a Treasure Chest! You still need your ancient weapon.\n");
        } else {
            printf("  💰  Another chest — you already have one.\n");
        }
    }

    /* ---- ANCIENT WEAPON ---- */
    else if (is_ancient_weapon(type)) {
        CardType sought = ancient_weapon_for_class(p->class);
        if (type == sought) {
            if (!p->has_weapon) {
                p->has_weapon = 1;
                printf("  🏆  You found \033[1;33mYOUR\033[0m ancient weapon: %s!\n",
                       card_name(type));
            } else {
                printf("  🏆  Your ancient weapon again — already collected.\n");
            }
        } else {
            /* Not the player's weapon — just keep moving */
            printf("  🔍  An ancient weapon, but not yours. Keep searching!\n");
        }
    }

    /* ---- PORTAL ---- */
    else if (type == CARD_PORTAL) {
        printf("  🌀  Magic Portal! Your next move can target ANY hidden cell.\n");
        *portal_active = 1;
        /* Portal itself: player stays here, next choose_cell will use any-hidden */
    }

    /* ---- TOTEM ---- */
    else if (type == CARD_TOTEM) {
        printf("  🗿  Transmutation Totem! You must swap it with another hidden cell.\n");

        int swap_row, swap_col;
        int found = choose_cell_for_totem_swap(gs, row, col, &swap_row, &swap_col);

        if (found) {
            /* Swap the totem card with the chosen hidden card */
            Card tmp               = gs->board[swap_row][swap_col];
            gs->board[swap_row][swap_col] = gs->board[row][col];
            gs->board[row][col]           = tmp;

            /* Both cells go back face-down after the swap */
            gs->board[swap_row][swap_col].revealed = 0;
            gs->board[row][col].revealed            = 0;

            printf("  🔄  Totem swapped with row %d, col %d.\n",
                   swap_row + 1, swap_col + 1);
        } else {
            printf("  (No other hidden cells — nothing to swap.)\n");
        }

        /* Totem always ends the turn */
        printf("  ⏹️   Turn ends. %s returns to start.\n", p->name);
        return TURN_END_TOTEM;
    }

    /* ---- VICTORY CHECK ---- */
    if (p->has_treasure && p->has_weapon) {
        printf("\n  🎉🏆🎉  \033[1;33m%s wins!\033[0m They have the treasure AND their ancient weapon!\n",
               p->name);
        return TURN_VICTORY;
    }

    /* ---- BLOCK CHECK ---- */
    int adj_rows[4], adj_cols[4];
    int adj_count = get_adjacent_hidden(gs, p->row, p->col, adj_rows, adj_cols);

    if (adj_count == 0 && !(*portal_active)) {
        printf("\n  🚧  No hidden adjacent cells — %s is blocked! Turn ends.\n", p->name);
        return TURN_END_BLOCKED;
    }

    return TURN_CONTINUE;
}

/* ============================================================
 * Board and player reset
 * ============================================================ */

/*
 * reset_board_and_player()
 * Flips all cards face-down and returns the current player to their
 * starting position. Called whenever a turn ends without victory.
 */
void reset_board_and_player(GameState *gs) {
    /* Hide all cards */
    for (int r = 0; r < BOARD_SIZE; r++) {
        for (int c = 0; c < BOARD_SIZE; c++) {
            gs->board[r][c].revealed = 0;
        }
    }

    /* Send player back to start */
    Player *p = &gs->players[gs->current_player];
    p->row    = p->start_row;
    p->col    = p->start_col;
}

/* ============================================================
 * Full turn orchestration
 * ============================================================ */

/*
 * next_active_player()
 * Advances gs->current_player to the next player who is still active.
 * Uses modular arithmetic to wrap around.
 */
static void next_active_player(GameState *gs) {
    int start = gs->current_player;
    do {
        gs->current_player = (gs->current_player + 1) % gs->nb_players;
        if (gs->current_player == start) {
            /* All players cycled — no active player found (edge case) */
            break;
        }
    } while (!gs->players[gs->current_player].is_active);
}

/*
 * print_player_status()
 * Shows a brief status for the current player before their turn.
 */
static void print_player_status(const GameState *gs) {
    const Player *p = &gs->players[gs->current_player];
    print_separator("PLAYER STATUS");
    printf("  Name     : \033[1;36m%s\033[0m\n", p->name);
    printf("  Class    : %s\n", class_name(p->class));
    printf("  Seeks    : %s\n", card_name(ancient_weapon_for_class(p->class)));
    printf("  Treasure : %s\n", p->has_treasure ? "\033[1;32mFound ✅\033[0m"
                                                 : "\033[1;31mNot yet ❌\033[0m");
    printf("  Weapon   : %s\n", p->has_weapon   ? "\033[1;32mFound ✅\033[0m"
                                                 : "\033[1;31mNot yet ❌\033[0m");
    printf("  Position : row %d, col %d\n\n", p->row + 1, p->col + 1);
}

/*
 * play_turn()
 * Runs one complete turn for the current player.
 *
 * Loop:
 *   Step 1 — choose_weapon()
 *   Step 2 — choose a cell (adjacent, or any if portal active)
 *   Step 3 — resolve_cell() → check result
 *
 * The loop continues while TurnResult == TURN_CONTINUE.
 * When the turn ends (any other result), the board is reset (unless
 * it is a TURN_VICTORY, where the board stays revealed for analysis).
 *
 * Advances to the next player except on victory.
 */
TurnResult play_turn(GameState *gs) {
    Player     *p            = &gs->players[gs->current_player];
    TurnResult  result       = TURN_CONTINUE;
    int         portal_active = 0;  /* Set to 1 when player stepped on a portal */

    print_player_status(gs);
    printf("  Press ENTER to begin your turn...");
    getchar(); /* Wait for player to be ready */

    /* ---- Main turn loop ---- */
    while (result == TURN_CONTINUE) {

        /* Step 1: choose a weapon */
        choose_weapon(gs);

        /* Step 2: choose a cell to reveal */
        int row, col;
        int cell_found;

        if (portal_active) {
            /* Portal effect: may jump to any hidden cell */
            cell_found    = choose_cell_any_hidden(gs, &row, &col);
            portal_active = 0; /* Portal is consumed after one use */
        } else {
            cell_found = choose_cell_adjacent(gs, &row, &col);
        }

        if (!cell_found) {
            /* No cells available anywhere — player is blocked */
            printf("\n  🚧  No hidden cells available. Turn ends.\n");
            result = TURN_END_BLOCKED;
            break;
        }

        /* Step 3: reveal the cell and apply consequences */
        result = resolve_cell(gs, row, col, &portal_active);

        /* Small pause so the player can read the outcome */
        printf("\n  Press ENTER to continue...");
        getchar();
    }

    /* ---- Handle turn end ---- */
    if (result == TURN_VICTORY) {
        /* Winner found — update game state */
        gs->game_over = 1;
        gs->winner    = gs->current_player;
        p->is_active  = 0;
        /* Board stays revealed so everyone can see the layout */
    } else {
        /* Non-victory end: reset board and move to next player */
        switch (result) {
            case TURN_END_DEAD:
                printf("\n  ⚰️   %s is sent back to their starting position.\n", p->name);
                break;
            case TURN_END_BLOCKED:
                printf("\n  🚧  %s is stuck and retreats to their starting position.\n", p->name);
                break;
            case TURN_END_TOTEM:
                /* Message already printed in resolve_cell */
                break;
            default:
                break;
        }

        reset_board_and_player(gs);
        next_active_player(gs);
    }

    return result;
}
