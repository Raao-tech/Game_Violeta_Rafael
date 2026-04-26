/**
 * @brief It implements the game rules
 *
 * @file game_rules.c
 * @author Salvador
 * @version 1.0
 * @date 26-04-2026
 * @copyright GNU Public License
 */

#include "game_rules.h"

Status game_rule_attack_enemigo(Game *game, Id id_enemy)
{
    Player *player = NULL;
    Space *space = NULL;
    Numen *num = NULL, *enemy_num = NULL;
    char *skill_indx_ch = NULL;
    Skills_id skill = NO_SKILL;
    Id space_id, num_id;
    Set *space_numens = NULL;
    int roll, distance, skill_indx, radio, active_pos_x, active_pos_y, enemy_pos_x, enemy_pos_y, num_enemies, i;

    srand(time(NULL));

    if (!game || id_enemy == NO_ID)
        return ERROR_enemy_attack;

    player = game_get_player_at(game, PLAYER);
    if (!player)
    {
        return ERROR_enemy_attack;
    }
    num_id = player_get_active_numen(player);
    if (num_id == NO_ID)
    {
        return ERROR_enemy_attack;
    }
    space_id = player_get_zone(player);
    if (space_id == NO_ID)
    {
        return ERROR_enemy_attack;
    }
    space = game_get_space(game, space_id);
    if (!space)
    {
        return ERROR_enemy_attack;
    }

    if (space_contains_numen(space, id_enemy) == FALSE)
    {
        return ERROR_enemy_attack;
    }

    enemy_num = game_get_numen_by_id(game, id_enemy);
    if (!enemy_num)
    {
        return ERROR_enemy_attack;
    }

    skill_indx = rand() % MAX_HELD_SKILLS; /* Enemy randomly selects a skill from its held skills */
    for (i = 0; i < MAX_HELD_SKILLS; i++)
    {
        skill = numen_get_skill_by_index(num, skill_indx);
        if (skill == NO_SKILL)
        {
            skill_indx = (skill_indx + 1) % MAX_HELD_SKILLS; /* Try another skill if the selected one is NO_SKILL */
        }
    }
    if (skill == NO_SKILL)
    {
        return ERROR_enemy_attack; /* Enemy has no valid skills to attack */
    }
    num = game_get_numen_by_id(game, num_id);
    if (!space || !num)
    {
        return ERROR_enemy_attack;
    }

    active_pos_x = numen_get_pos_x(num);
    active_pos_y = numen_get_pos_y(num);

    enemy_pos_x = numen_get_pos_x(enemy_num);
    enemy_pos_y = numen_get_pos_y(enemy_num);

    if (numen_get_health(num) <= 0 && numen_get_health(enemy_num) <= 0)
    {
        return ERROR_enemy_attack;
    }

    radio = skill_get_radio(skill); /*por implementar*/

    distance = sqrt(pow(active_pos_x - enemy_pos_x, 2) + pow(active_pos_y - enemy_pos_y, 2));
    if (distance <= radio)
    {
        if (skill_active(enemy_num, num, skill) == ERROR) /*por implementar status skill_apply_effect*/
        {
            return ERROR_enemy_attack;
        }
    }
    return OK;
}

Status game_rule_walk_enemigo(Game *game, Id id_enemy)
{
    if (!game || !id_enemy)
        return ERROR;
}
