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

Status game_rule_attack_enemy (Game* game, Id id_enemy);

Status game_rule_walk_enemy (Game* game);

Status game_rule_walk_active (Game* game);

Status game_rule_move (Game* game);

Status game_rules_regen (Game* game);

Bool game_rules_win_condition (Game* game);

Bool game_rules_loose_condition (Game* game);

void game_rules_death_numen(Game* game, Numen* num);

#endif