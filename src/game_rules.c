/**
 * @brief It implements the game rules
 *
 * @file game_rules.c
 * @author Salvador
 * @version 1.5
 * @date 01-05-2026
 * @copyright GNU Public License
 */

#include "game_rules.h"

Status
game_rule_attack_enemy (Game* game, Id id_enemy)
{
    Player* player = NULL;
    Space* space   = NULL;
    Numen *num = NULL, *enemy_num = NULL;
    char* skill_indx_ch = NULL;
    Skills_id skill     = NO_SKILL;
    Id space_id, num_id;
    Set* space_numens = NULL;
    int distance, skill_indx = 0, radio, active_pos_x, active_pos_y, enemy_pos_x, enemy_pos_y, num_enemies, i;

    if (!game || id_enemy == NO_ID) return ERROR_enemy_attack;

    player = game_get_player_at (game, PLAYER);
    if (!player) return ERROR_enemy_attack;

    num_id = player_get_active_numen (player);
    if (num_id == NO_ID) return ERROR_enemy_attack;

    space_id = player_get_zone (player);
    if (space_id == NO_ID) return ERROR_enemy_attack;

    space = game_get_space (game, space_id);
    if (!space) return ERROR_enemy_attack;

    if (space_contains_numen (space, id_enemy) == FALSE) return ERROR_enemy_attack;

    enemy_num = game_get_numen_by_id (game, id_enemy);
    if (!enemy_num || numen_get_health (enemy_num) <= 0) return ERROR_enemy_attack;

    skill_indx = rand () % NUM_SKILLS; /* Enemy randomly selects a skill from its held skills */
    for (i = 0; i < NUM_SKILLS; i++)
        {
            skill = numen_get_skill_by_index (num, skill_indx);

            if (skill == NO_SKILL) skill_indx = (skill_indx + 1) % NUM_SKILLS; /* Try another skill if the selected one is NO_SKILL */
        }
    if (skill == NO_SKILL) return ERROR_enemy_attack; /* Enemy has no valid skills to attack */

    num = game_get_numen_by_id (game, num_id);
    if (!space || !num) return ERROR_enemy_attack;

    active_pos_x = numen_get_pos_x (num);
    active_pos_y = numen_get_pos_y (num);

    enemy_pos_x  = numen_get_pos_x (enemy_num);
    enemy_pos_y  = numen_get_pos_y (enemy_num);

    if (numen_get_health (num) <= 0 && numen_get_health (enemy_num) <= 0) return ERROR_enemy_attack;

    radio    = skill_get_radio (skill); /*por implementar*/

    distance = sqrt (pow (active_pos_x - enemy_pos_x, 2) + pow (active_pos_y - enemy_pos_y, 2));

    if (skill_active (enemy_num, num, skill, distance) == ERROR) /*por implementar status skill_apply_effect*/
        return ERROR_enemy_attack;

    return OK;
}

Status
game_rule_walk_enemy (Game* game)
{
    Player* player      = NULL;
    Numen* enemy_num    = NULL;
    Direction direction = NULL;
    Space* space        = NULL;
    Position pos_current, pos_player;
    Id id_enemy        = NO_ID;
    Set* space_num_ids = NULL;
    int *grid[HIGHT] = NULL, i, num_enemies = 0, loop;
    Status ret=OK;

    if (!game)
        {
            return ERROR;
        }
    player = game_get_player_at (game, PLAYER);
    if (!player)
        {
            return ERROR;
        }
    space = game_get_space (game, player_get_zone (player));
    if (!space)
        {
            return ERROR;
        }
    num_enemies = space_get_n_numens (space);
    if (num_enemies == 0)
        {
            return OK;
        }
    space_num_ids = space_get_numens (space);

    for (loop = 0; loop < num_enemies; loop++)
        {
            id_enemy = set_get_id_at (space_num_ids, loop);
            if (id_enemy == NO_ID || id_enemy == player_get_active_numen (player))
                {
                    continue; /* Skip if the ID is NO_ID or if it's the player's active numen */
                }
            enemy_num = game_get_numen_by_id (game, id_enemy);

            if (!enemy_num || numen_get_health (enemy_num) <= 0)
                {
                    continue; /*skip dead numens or when *enemy_num=NULL */
                }

            pos_current.pos_x = numen_get_pos_x (enemy_num);
            pos_current.pos_y = numen_get_pos_y (enemy_num);

            pos_player        = player_get_position (player);

            if (pos_current.pos_x < pos_player.pos_x)
                {
                    direction = E;
                }
            else if (pos_current.pos_x > pos_player.pos_x)
                {
                    direction = W;
                }
            else if (pos_current.pos_y < pos_player.pos_y)
                {
                    direction = S;
                }
            else if (pos_current.pos_y > pos_player.pos_y)
                {
                    direction = N;
                }
            else
                {
                    continue; /* Enemy is already on the player's position */
                }

            if (numen_get_health (enemy_num) == 1)
                {
                    switch (direction)
                        {
                        case N:
                            return S;
                        case S:
                            return N;
                        case E:
                            return W;
                        case W:
                            return E;
                        default:
                            return U;
                        }
                }

            switch (direction) /*Falta definir cuánto se mueve*/
                {
                case N:
                    pos_current.pos_y -= SCALE;
                    break;
                case S:
                    pos_current.pos_y += SCALE;
                    break;
                case W:
                    pos_current.pos_x -= SCALE;
                    break;
                case E:
                    pos_current.pos_x += SCALE;
                    break;
                default:
                    break;
                }

            for (i = 0; i < HIGHT; i++)
                {
                    grid[i] = space_get_grid_by_line (game_get_space (game, game_get_numen_location (game, id_enemy)), i);
                }
            if (grid[pos_current.pos_x][pos_current.pos_y] != 0)
                {
                    if (numen_set_pos_x (enemy_num, pos_current.pos_x) == ERROR || numen_set_pos_y (enemy_num, pos_current.pos_y) == ERROR)
                        {
                            ret=ERROR; /*Unable to move a enemy_num*/
                         }
                        continue;
                }
            switch (direction) /*checks whether the direction that failed is on the x coordinate and restores it. If it isn't, then it must've been on
                                            the y coordinate and there's nothing left to check*/
                {
                case W:
                    pos_current.pos_x += SCALE;
                    break;
                case E:
                    pos_current.pos_x -= SCALE;
                    break;
                default:
                    continue;
                }
            /*If the error was on the x coordinate, try the y coordinate*/
            if (pos_current.pos_y < pos_player.pos_y)
                {
                    direction = S;
                }
            else if (pos_current.pos_y > pos_player.pos_y)
                {
                    direction = N;
                }

            switch (direction) /*Falta definir cuánto se mueve*/
                {
                case N:
                    pos_current.pos_y -= SCALE;
                    break;
                case S:
                    pos_current.pos_y += SCALE;
                    break;

                default:
                    break;
                }
            if (grid[pos_current.pos_x][pos_current.pos_y] != 0)
                {
                    if (numen_set_pos_x (enemy_num, pos_current.pos_x) == ERROR || numen_set_pos_y (enemy_num, pos_current.pos_y) == ERROR)
                        {
                            ret=ERROR; /*Unable to move a enemy_num*/
                        }
                   
                }
            
        }
    return ret;
}

Status
game_rule_walk_active (Game* game)
{
    Player* player      = NULL;
    Numen* active_num   = NULL;
    Direction direction = NULL;
    Space* space        = NULL;
    Position pos_current, pos_player;
    Id id_active     = NO_ID;
    int *grid[HIGHT] = NULL, i, radio = SCALE * 4, distance;

    if (!game)
        {
            return ERROR;
        }
    player = game_get_player_at (game, PLAYER);
    if (!player)
        {
            return ERROR;
        }
    space = game_get_space (game, player_get_zone (player));
    if (!space)
        {
            return ERROR;
        }
    for (i = 0; i < HIGHT; i++)
        {
            grid[i] = space_get_grid_by_line (game_get_space (game, player_get_zone (player)), i);
        }

    id_active = player_get_active_numen (player);
    if (id_active == NO_ID)
        {
            return ERROR; /* Skip if the ID is NO_ID*/
        }
    active_num = game_get_numen_by_id (game, id_active);

    if (!active_num || numen_get_health (active_num) <= 0)
        {
            /*¿debería cambiar al active numen en este caso?*/
            return ERROR;
        }

    pos_current.pos_x = numen_get_pos_x (active_num);
    pos_current.pos_y = numen_get_pos_y (active_num);

    pos_player        = player_get_position (player);

    distance          = sqrt (pow (pos_current.pos_x - pos_player.pos_x, 2) + pow (pos_current.pos_y - pos_player.pos_y, 2));

    if (distance > radio) /*The active numen is too far from player, it tries to get back to the area*/
        {
            if (pos_current.pos_x < pos_player.pos_x)
                {
                    direction = E;
                }
            else if (pos_current.pos_x > pos_player.pos_x)
                {
                    direction = W;
                }
            else if (pos_current.pos_y < pos_player.pos_y)
                {
                    direction = S;
                }
            else if (pos_current.pos_y > pos_player.pos_y)
                {
                    direction = N;
                }

            /////////////////////////////////////////////////////////
            switch (direction) /*Falta definir cuánto se mueve*/
                {
                case N:
                    pos_current.pos_y -= SCALE;
                    break;
                case S:
                    pos_current.pos_y += SCALE;
                    break;
                case W:
                    pos_current.pos_x -= SCALE;
                    break;
                case E:
                    pos_current.pos_x += SCALE;
                    break;
                default:
                    break;
                }

            /////////////////////////////////////////////////////////
            if (grid[pos_current.pos_x][pos_current.pos_y] != 0)
                {
                    if (numen_set_pos_x (active_num, pos_current.pos_x) == ERROR || numen_set_pos_y (active_num, pos_current.pos_y) == ERROR)
                        {
                            return ERROR;
                        }
                    return OK;
                }
            switch (direction) /*checks whether the direction that failed is on the x coordinate and restores it. If it isn't, then it must've been on
                                                        the y coordinate and there's nothing left to check*/
                {
                case W:
                    pos_current.pos_x += SCALE;
                    break;
                case E:
                    pos_current.pos_x -= SCALE;
                    break;
                default:
                    return ERROR;
                }
            /*If the error was on the x coordinate, try the y coordinate*/
            if (pos_current.pos_y < pos_player.pos_y)
                {
                    direction = S;
                }
            else if (pos_current.pos_y > pos_player.pos_y)
                {
                    direction = N;
                }

            switch (direction) /*Falta definir cuánto se mueve*/
                {
                case N:
                    pos_current.pos_y -= SCALE;
                    break;
                case S:
                    pos_current.pos_y += SCALE;
                    break;

                default:
                    break;
                }
            if (grid[pos_current.pos_x][pos_current.pos_y] != 0)
                {
                    if (numen_set_pos_x (active_num, pos_current.pos_x) == ERROR || numen_set_pos_y (active_num, pos_current.pos_y) == ERROR)
                        {
                            return ERROR;
                        }
                    return OK;
                }
            return ERROR; /*Unable to move the active numen any closer to the player*/
        }
    else /*The numen is close enough to the player, for now, it moves at any random position inside the area or 1 square out of it, it decides it
            randomly*/
        {
            direction = (Direction)rand () % 4; /*The 4 possible directions besides unknown*/

            switch (direction) /*Falta definir cuánto se mueve*/
                {
                case N:
                    pos_current.pos_y -= SCALE;
                    break;
                case S:
                    pos_current.pos_y += SCALE;
                    break;
                case W:
                    pos_current.pos_x -= SCALE;
                    break;
                case E:
                    pos_current.pos_x += SCALE;
                    break;
                default:
                    break;
                }

            if (grid[pos_current.pos_x][pos_current.pos_y] == 0)
                {
                    for (direction, i = 0; grid[pos_current.pos_x][pos_current.pos_y] == 0, i < 3; direction = (direction + 1) % 4, i++)
                        {

                            switch ((direction + 3) % 4) /*restores the previous coordinates*/
                                {
                                case N:
                                    pos_current.pos_y += SCALE;
                                    break;
                                case S:
                                    pos_current.pos_y -= SCALE;
                                    break;
                                case W:
                                    pos_current.pos_x += SCALE;
                                    break;
                                case E:
                                    pos_current.pos_x -= SCALE;
                                    break;
                                default:
                                    break;
                                }
                            switch (direction) /*gets the new coordinates to move to*/
                                {
                                case N:
                                    pos_current.pos_y -= SCALE;
                                    break;
                                case S:
                                    pos_current.pos_y += SCALE;
                                    break;
                                case W:
                                    pos_current.pos_x -= SCALE;
                                    break;
                                case E:
                                    pos_current.pos_x += SCALE;
                                    break;
                                default:
                                    break;
                                }
                        }
                }
            if (grid[pos_current.pos_x][pos_current.pos_y] == 0) return ERROR; /*The active Numen can't move*/
            if (numen_set_pos_x (active_num, pos_current.pos_x) == ERROR || numen_set_pos_y (active_num, pos_current.pos_y) == ERROR)
                {
                    return ERROR;
                }
            return OK;
        }
}
Status
game_rule_move (Game* game)
{
    Player* player = NULL;
    Space* dest_sp = NULL;
    char* dir_str  = NULL;
    Direction dir;
    Id origin, dest;
    int pos_x, pos_y;

    if (!game) return;
    player = game_get_player_at (game, PLAYER);
    if (!player)
        {
            return ERROR;
        }

    origin = player_get_zone (player);
    if (origin == NO_ID)
        {
            return ERROR;
        }

    dir_str = command_get_target (game_get_last_command (game));
    if (!dir_str)
        {
            return ERROR;
        }

    dir = ge_parse_direction (dir_str);
    if (dir == U)
        {
            return ERROR;
        }

    dest = game_get_connection (game, origin, dir);
    if (dest == NO_ID)
        {
            return ERROR;
        }

    if (game_connection_is_open (game, origin, dir) == FALSE)
        {
            return ERROR;
        }
    pos_x = player_get_pos_x (player);
    pos_y = player_get_pos_y (player);

    switch (dir)
        {
        case S:
            player_set_position (player, pos_x, (int)((float)(HIGHT_SCREEN / SCALE)));
            break;

        case N:
            player_set_position (player, pos_x, (int)(HIGHT_SCREEN - (float)(HIGHT_SCREEN / SCALE)));
            break;

        case E:
            player_set_position (player, (int)((float)(WIDHT_SCREEN / SCALE)), pos_y);
            break;

        case W:
            player_set_position (player, (int)(WIDHT_SCREEN - (float)(WIDHT_SCREEN / SCALE)), pos_y);
            break;
        }
    player_set_zone (player, dest);

    dest_sp = game_get_space (game, dest);
    if (dest_sp) space_set_discovered (dest_sp, TRUE);

    return OK;
}
