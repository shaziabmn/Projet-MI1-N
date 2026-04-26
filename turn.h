#ifndef TURN_H
#define TURN_H

/* ============================================================
 * turn.h
 * Public interface for the turn logic module (Person 2).
 *
 * This module handles everything that happens during ONE full
 * turn for the current player:
 *   1. Weapon selection
 *   2. Cell reveal & consequence resolution
 *   3. Victory / death / block / totem detection
 *   4. Board reset when the turn ends
 * ============================================================ */

#include "game_types.h"

/* ----- Weapon helpers ----- */

/**
 * weapon_name()
 * Returns a human-readable string for the given weapon type.
 */
const char *weapon_name(WeaponType w);

/**
 * weapon_emoji()
 * Returns an emoji/symbol representing the weapon (for terminal display).
 */
const char *weapon_emoji(WeaponType w);

/**
 * weapon_defeats()
 * Returns 1 if weapon `w` defeats the monster on card of type `card`,
 * 0 otherwise. Non-monster cards always return 0.
 */
int weapon_defeats(WeaponType w, CardType card);

/* ----- Cell helpers ----- */

/**
 * card_name()
 * Returns a human-readable string for a card type.
 */
const char *card_name(CardType c);

/**
 * is_monster()
 * Returns 1 if the card type represents a monster.
 */
int is_monster(CardType c);

/**
 * is_ancient_weapon()
 * Returns 1 if the card type is any of the 4 ancient weapons.
 */
int is_ancient_weapon(CardType c);

/**
 * ancient_weapon_for_class()
 * Returns the CardType of the ancient weapon that belongs to `class`.
 */
CardType ancient_weapon_for_class(AdventurerClass class);

/* ----- Adjacency / movement helpers ----- */

/**
 * get_adjacent_hidden()
 * Fills `rows[]` and `cols[]` with the coordinates of all hidden cells
 * adjacent (up/down/left/right) to position (r, c) on the board.
 * Returns the number of such cells found (0 means the player is blocked).
 */
int get_adjacent_hidden(const GameState *gs, int r, int c,
                        int rows[4], int cols[4]);

/**
 * get_all_hidden()
 * Fills `rows[]` and `cols[]` with ALL hidden cells on the board.
 * Returns the count. Used for portal and totem effects.
 */
int get_all_hidden(const GameState *gs, int rows[25], int cols[25]);

/* ----- Core turn functions ----- */

/**
 * choose_weapon()
 * Interactively asks the current player to choose one of the 4 weapons.
 * Updates gs->players[gs->current_player].active_weapon.
 */
void choose_weapon(GameState *gs);

/**
 * choose_cell_adjacent()
 * Asks the player to pick an adjacent hidden cell. Used for normal movement.
 * Fills *out_row and *out_col with the chosen coordinates.
 * Returns 0 if no adjacent hidden cell exists (player is blocked).
 */
int choose_cell_adjacent(GameState *gs, int *out_row, int *out_col);

/**
 * choose_cell_any_hidden()
 * Asks the player to pick ANY hidden cell on the board.
 * Used after the portal effect. Fills *out_row and *out_col.
 * Returns 0 if no hidden cell exists at all.
 */
int choose_cell_any_hidden(GameState *gs, int *out_row, int *out_col);

/**
 * resolve_cell()
 * Applies the consequence of revealing the cell at (row, col) for the
 * current player. Handles monsters, treasures, ancient weapons, portals,
 * and totems.
 *
 * Returns a TurnResult:
 *   TURN_CONTINUE   — player survives and may keep going
 *   TURN_END_DEAD   — player died (wrong weapon vs monster)
 *   TURN_END_BLOCKED— no more adjacent hidden cells
 *   TURN_END_TOTEM  — totem triggered, turn resets
 *   TURN_VICTORY    — player has both treasure and ancient weapon
 *
 * `portal_active` is an in/out flag: set to 1 when a portal is found,
 * and the caller should use choose_cell_any_hidden() for the next move.
 */
TurnResult resolve_cell(GameState *gs, int row, int col, int *portal_active);

/**
 * reset_board_and_player()
 * Resets all cells to face-down and moves the current player back to
 * their starting position. Called when a turn ends (death, block, totem).
 */
void reset_board_and_player(GameState *gs);

/**
 * play_turn()
 * Runs a complete turn for gs->players[gs->current_player].
 * Loops through weapon-choice → cell-reveal → consequence until the
 * turn ends (death / block / totem / victory).
 *
 * Returns the TurnResult that ended the turn (TURN_VICTORY if won).
 * Advances gs->current_player to the next active player when appropriate.
 */
TurnResult play_turn(GameState *gs);

#endif /* TURN_H */
