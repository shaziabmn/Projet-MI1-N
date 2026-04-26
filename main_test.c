/* ============================================================
 * main_test.c
 * Minimal test harness for the turn logic module (Person 2).
 *
 * This file stubs the board initialisation (Person 1) and the
 * stats persistence (Person 3) so that Person 2's code can be
 * compiled and tested independently.
 *
 * TO COMPILE:
 *   gcc -Wall -Wextra -o memo_rpg_test main_test.c turn.c
 *
 * TO RUN:
 *   ./memo_rpg_test
 * ============================================================ */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "game_types.h"
#include "turn.h"

/* ============================================================
 * Stub: board initialisation (normally done by Person 1)
 * ============================================================ */

/*
 * init_board_stub()
 * Fills the 5x5 board with the correct number of each card type,
 * shuffles them using Fisher-Yates, then hides all cells.
 */
static void init_board_stub(GameState *gs) {
    /* Build a flat list of all 25 card types */
    CardType deck[TOTAL_CELLS];
    int idx = 0;

    /* 4 monsters of each type (4×4 = 16 total) */
    for (int i = 0; i < NB_BASILISKS; i++) deck[idx++] = CARD_MONSTER_BASILISK;
    for (int i = 0; i < NB_ZOMBIES;   i++) deck[idx++] = CARD_MONSTER_ZOMBIE;
    for (int i = 0; i < NB_TROLLS;    i++) deck[idx++] = CARD_MONSTER_TROLL;
    for (int i = 0; i < NB_HARPIES;   i++) deck[idx++] = CARD_MONSTER_HARPY;

    /* 2 treasure chests */
    for (int i = 0; i < NB_TREASURES; i++) deck[idx++] = CARD_TREASURE;

    /* 4 ancient weapons (1 per class) */
    deck[idx++] = CARD_WEAPON_FIRE_SWORD;
    deck[idx++] = CARD_WEAPON_STAFF;
    deck[idx++] = CARD_WEAPON_GRIMOIRE;
    deck[idx++] = CARD_WEAPON_DAGGER;

    /* 1 portal, 2 totems */
    deck[idx++] = CARD_PORTAL;
    for (int i = 0; i < NB_TOTEMS; i++) deck[idx++] = CARD_TOTEM;

    /* Sanity check */
    if (idx != TOTAL_CELLS) {
        fprintf(stderr, "ERROR: deck size mismatch (%d vs %d)\n", idx, TOTAL_CELLS);
        exit(EXIT_FAILURE);
    }

    /* Fisher-Yates shuffle */
    srand((unsigned int)time(NULL));
    for (int i = TOTAL_CELLS - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        CardType tmp = deck[i];
        deck[i]      = deck[j];
        deck[j]      = tmp;
    }

    /* Place shuffled deck onto the 5x5 board, all face-down */
    for (int r = 0; r < BOARD_SIZE; r++) {
        for (int c = 0; c < BOARD_SIZE; c++) {
            gs->board[r][c].type     = deck[r * BOARD_SIZE + c];
            gs->board[r][c].revealed = 0;
        }
    }
}

/*
 * init_players_stub()
 * Sets up 2 players for a quick test game.
 * Each player starts on a different edge of the board at the middle cell.
 * (Person 1 will handle the real player-setup menu.)
 */
static void init_players_stub(GameState *gs) {
    gs->nb_players     = 2;
    gs->current_player = 0;
    gs->game_over      = 0;
    gs->winner         = -1;

    /* Player 0: Warrior, starts top-centre (row 0, col 2) */
    strncpy(gs->players[0].name, "Arthur", sizeof(gs->players[0].name) - 1);
    gs->players[0].class       = CLASS_WARRIOR;
    gs->players[0].start_row   = 0;
    gs->players[0].start_col   = 2;
    gs->players[0].row         = 0;
    gs->players[0].col         = 2;
    gs->players[0].has_treasure = 0;
    gs->players[0].has_weapon  = 0;
    gs->players[0].is_active   = 1;

    /* Player 1: Mage, starts bottom-centre (row 4, col 2) */
    strncpy(gs->players[1].name, "Merlin", sizeof(gs->players[1].name) - 1);
    gs->players[1].class       = CLASS_MAGE;
    gs->players[1].start_row   = 4;
    gs->players[1].start_col   = 2;
    gs->players[1].row         = 4;
    gs->players[1].col         = 2;
    gs->players[1].has_treasure = 0;
    gs->players[1].has_weapon  = 0;
    gs->players[1].is_active   = 1;
}

/* ============================================================
 * Stub: display board (normally handled by Person 1)
 * ============================================================ */

/*
 * display_board_stub()
 * Prints a simple ASCII representation of the board.
 * Hidden cells show "[ ? ]", revealed cells show a short tag.
 */
static void display_board_stub(const GameState *gs) {
    /* All tags exactly 3 chars → each cell = "[XXX]" = 5 chars */
    /* 5 cells * 5 chars + 4 spaces between + 2 border spaces = 31 inner chars */
    const char *short_name[12] = {
        "BSK", "ZMB", "TRL", "HRP",
        "TRS", "SWD", "STF", "GRM",
        "DAG", "PRT", "TTM", "???"
    };

    printf("\n  ┌─────────────────────────────────┐\n");
    printf("  │        LABYRINTH BOARD           │\n");
    printf("  ├─────────────────────────────────┤\n");

    for (int r = 0; r < BOARD_SIZE; r++) {
        printf("  │ ");
        for (int c = 0; c < BOARD_SIZE; c++) {
            if (!gs->board[r][c].revealed) {
                printf("[???]");
            } else {
                int t = (int)gs->board[r][c].type;
                if (t < 0 || t > 11) t = 11;
                printf("[%s]", short_name[t]);
            }
            if (c < BOARD_SIZE - 1) printf(" ");
        }
        printf(" │\n");
    }

    printf("  └─────────────────────────────────┘\n");

    /* Show player positions */
    for (int i = 0; i < gs->nb_players; i++) {
        const Player *p = &gs->players[i];
        if (p->is_active) {
            printf("  %s → row %d, col %d\n", p->name, p->row + 1, p->col + 1);
        }
    }
    printf("\n");
}

/* ============================================================
 * Main game loop (stub — Person 1 handles the real menu)
 * ============================================================ */

int main(void) {
    GameState gs;
    memset(&gs, 0, sizeof(GameState));

    printf("\033[1;33m");
    printf("╔══════════════════════════════════════╗\n");
    printf("║         MEMO-RPG  —  TEST RUN        ║\n");
    printf("║     Person 2 : Turn Logic Module     ║\n");
    printf("╚══════════════════════════════════════╝\n");
    printf("\033[0m\n");

    /* Initialise board and players (stubs for Person 1's work) */
    init_board_stub(&gs);
    init_players_stub(&gs);

    /* ---- Main game loop ---- */
    while (!gs.game_over) {
        /* Show board before each turn (Person 1 will do this properly) */
        display_board_stub(&gs);

        /* Run one full turn for the current player */
        TurnResult result = play_turn(&gs);

        if (result == TURN_VICTORY) {
            printf("\n\033[1;33m");
            printf("╔══════════════════════════════════════╗\n");
            printf("║             GAME OVER!               ║\n");
            printf("║  Winner: %-28s ║\n", gs.players[gs.winner].name);
            printf("╚══════════════════════════════════════╝\n");
            printf("\033[0m\n");

            /* Reveal the full board (spec requirement) */
            for (int r = 0; r < BOARD_SIZE; r++)
                for (int c = 0; c < BOARD_SIZE; c++)
                    gs.board[r][c].revealed = 1;

            display_board_stub(&gs);
        }
    }

    /* Person 3 would save stats here */
    printf("  (Stats persistence handled by Person 3's module.)\n\n");

    return EXIT_SUCCESS;
}
