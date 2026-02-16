/**
 * @brief       It registers the user's input
 *
 * @file        game_actions.h
 * @author      Profesores PPROG
 * @version     0
 * @date        27-01-2025
 * @copyright   GNU Public License
 */

#ifndef GAME_ACTIONS_H
#define GAME_ACTIONS_H

#include "command.h"
#include "game.h"
#include "types.h"

/**
 * @brief It updates the game according to the user's input
 * @author Violeta y Rafa
 *
 * @param game a pointer to the game
 * @param cmd a pointer to the command
 * @return OK, if everything goes well, or ERROR if there was some mistake
 */
Status game_actions_update(Game *game, Command *cmd);

#endif