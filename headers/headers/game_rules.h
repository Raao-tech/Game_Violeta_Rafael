/**
 * @brief It defines the game rules
 *
 * This module manages the logic of the game the player doesn't interact with directly.
 *
 * @file game_rules.h
 * @author Salvador
 * @version 1.0
 * @date 26-04-2026
 * @copyright GNU Public License
 */

#ifndef GAME_RULES_H
#define GAME_RULES_H

#include "game.h"
#include "numen.h"
#include <time.h>

/**
 * @brief It updates the attack of the enemy
 * @author Rafael, Javier, Salvador and Violeta
 * 
 * @note The enemy only attack just having acount the distance
 *
 * @param game a pointer to the game
 * @param id_enemy the new id of the enemy value to set
 * @return OK, if everything goes well, or ERROR if there was some mistake
 */
Status game_rule_attack_enemy (Game* game, Id id_enemy);

/**
 * @brief It updates the position of the enemy in funtion of the numen active
 * @author Rafael, Javier, Salvador and Violeta
 *
 * @param game a pointer to the game
 * @return OK, if everything goes well, or ERROR if there was some mistake
 */
Status game_rule_walk_enemy (Game* game);

/**
 * @brief It updates the position of the numen active in funtion of the player
 * @author Rafael, Javier, Salvador and Violeta
 *
 * @param game a pointer to the game
 * @return OK, if everything goes well, or ERROR if there was some mistake
 */
Status game_rule_walk_active (Game* game);

/**
 * @brief It updates the position of the player and the numen active of the space
 * @author Rafael, Javier, Salvador and Violeta
 *
 * @param game a pointer to the game
 * @return OK, if everything goes well, or ERROR if there was some mistake
 */
Status game_rule_move (Game* game);

#endif