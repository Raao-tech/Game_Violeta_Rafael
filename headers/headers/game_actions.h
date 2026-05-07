/**
 * @brief It defines the game actions interface
 *
 * This module applies user commands to the game state.
 * Each command (move, take, drop, attack, chat, inspect, exit)
 * has a corresponding private handler that validates the action
 * and updates the game accordingly.
 *
 * @file game_actions.h
 * @author Profesores PPROG, Violeta, Rafael and Javier
 * @version 3
 * @date 11-04-2026
 * @copyright GNU Public License
 */

#ifndef GAME_ACTIONS_H
#define GAME_ACTIONS_H

#include "command.h"
#include "game.h"
#include "types.h"

/**
 * @brief It updates the game state according to the user's command
 * @author Violeta y Rafa
 *
 * Dispatches the command code to the appropriate private handler:
 * MOVE, TAKE, DROP, ATTACK, CHAT, INSPECT, EXIT, or UNKNOWN.
 * The result status is stored in the game via game_set_last_cmd_status.
 *
 * @param game a pointer to the game
 * @param cmd a pointer to the command (already parsed by command_get_user_input)
 * @return OK if the dispatch succeeded, ERROR if game or cmd is NULL
 */
Status game_actions_update (Game* game, Command* cmd);


Direction   ge_parse_direction (const char* str);

#endif