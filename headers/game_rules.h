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
#include "game_actions.h"
#include <time.h>

Status game_rule_attack_enemigo(Game *game, Id id_enemy);

Status game_rule_walk_enemigo(Game *game, Id id_enemy);


#endif