/**
 * @brief It implements the game update through user actions
 *
 * @file game_actions.c
 * @author Profesores PPROG, Violeta, Rafael, Salvador and Javier
 * @version 6
 * @date 03-05-2026
 * @copyright GNU Public License
 */
#include "game_actions.h"
#include "game_management.h"
#include "game_rules.h"
#include "raylib.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <time.h>

/* ========================================================================= */
/*                       PRIVATE PROTOTYPES                                   */
/* ========================================================================= */
static void game_actions_unknown (Game* game);
static void game_actions_exit    (Game* game);
static void game_actions_move    (Game* game);
static void game_actions_walk    (Game* game);
static void game_actions_take    (Game* game);
static void game_actions_drop    (Game* game);
static void game_actions_attack  (Game* game);
static void game_actions_chat    (Game* game);
static void game_actions_inspect (Game* game);
static void game_actions_use     (Game* game);
static void game_actions_save    (Game* game);
//static void game_actions_load    (Game* game);
static void game_actions_recruit (Game* game);
static void game_actions_kick    (Game* game);

Status game_actions_apply_effect (Game* game, Object* obj, Effect obj_effect);
static Status _apply_health_plus (Game* game, Object* obj);
static Status _apply_health_less (Game* game, Object* obj);
static Status _apply_open_door   (Game* game, Object* obj);
static Status _apply_speed_plus  (Game* game, Object* obj);
static Status _apply_speed_less  (Game* game, Object* obj);

Direction ge_parse_direction (const char* str);

/* ========================================================================= */
/*                        PUBLIC: DISPATCHER                                  */
/* ========================================================================= */
Status
game_actions_update (Game* game, Command* command)
{
    CommandCode cmd;

    if (!game || !command) return ERROR;
    if (game_set_last_command (game, command) == ERROR) return ERROR;

    cmd = command_get_code (command);

    switch (cmd)
    {
        case UNKNOWN: game_actions_unknown (game); break;
        case EXIT:    game_actions_exit    (game); break;
        case MOVE:    game_actions_move    (game); break;
        case WALK:    game_actions_walk    (game); break;
        case TAKE:    game_actions_take    (game); break;
        case DROP:    game_actions_drop    (game); break;
        case ATTACK:  game_actions_attack  (game); break;
        case CHAT:    game_actions_chat    (game); break;
        case INSPECT: game_actions_inspect (game); break;
        case USE:     game_actions_use     (game); break;
        case SAVE:    game_actions_save    (game); break;
       // case LOAD:    game_actions_load    (game); break;
        case RECRUIT: game_actions_recruit (game); break;
        case KICK:    game_actions_kick    (game); break;
        default: break;
    }
    return OK;
}

/* ========================================================================= */
/*                          UNKNOWN / EXIT                                    */
/* ========================================================================= */
static void
game_actions_unknown (Game* game)
{
    if (!game) return;
    game_set_last_cmd_status (game, ERROR);
}

static void
game_actions_exit (Game* game)
{
    if (!game) return;
    game_set_last_cmd_status (game, OK);
}

/* ========================================================================= */
/*                                MOVE                                        */
/* ========================================================================= */
static void
game_actions_move (Game* game)
{
    if (!game || game_rule_move (game) == ERROR)
    {
        game_set_last_cmd_status (game, ERROR);
        return;
    }
    game_set_last_cmd_status (game, OK);
}

/* ========================================================================= */
/*                          WALK (estilo Pokemon)                             */
/*                                                                            */
/* Mueve al player UNA celda en la direccion indicada.                        */
/* Si el destino esta fuera del grid, intenta transicion de space             */
/* via game_rule_move (la "puerta" del borde).                                */
/* Si el destino esta dentro del grid pero la celda es 0 (pared),             */
/* el movimiento falla.                                                       */
/* ========================================================================= */
static void
game_actions_walk (Game* game)
{   
    Numen*    nu   = NULL;
    Command*  lst_cmd;
    Player*   player;
    Space*    space;
    char*     dir_str;
    int*      grid_line;
    Direction direction;
    Position  pos_current;
    Position  pos_original;
    Position  cur_orig;
    Position  ply_vision;
    Player*   p     = NULL;  
    Id        nu_id;
    int       cell_x, cell_y;

    if (!game) { game_set_last_cmd_status (game, ERROR_walk); return; }

    /*ultimo comando*/
    lst_cmd = game_get_last_command (game);
    if (!lst_cmd) { game_set_last_cmd_status (game, ERROR_walk); return; }

    /*Dirección en string*/
    dir_str = command_get_target (lst_cmd);
    if (!dir_str) { game_set_last_cmd_status (game, ERROR_walk); return; }

    /*Dirección parseada*/
    direction = ge_parse_direction (dir_str);
    if (direction == U) { game_set_last_cmd_status (game, ERROR_walk); return; }

    /*Obtenemos player*/
    player = game_get_player_at (game, PLAYER);
    if (!player) { game_set_last_cmd_status (game, ERROR_walk); return; }

    pos_current = player_get_position (player);
    pos_original = player_get_position (player);
    ply_vision = pos_original;

    /* Calculamos la posicion DESTINO en pixeles */
    switch (direction)
    {
        case N: pos_current.pos_y -= SCALE; ply_vision.pos_y -= SCALE;  break;
        case S: pos_current.pos_y += SCALE; ply_vision.pos_y += SCALE;  break;
        case W: pos_current.pos_x -= SCALE; ply_vision.pos_x -= SCALE;  break;
        case E: pos_current.pos_x += SCALE; ply_vision.pos_x += SCALE;  break;
        default:
            game_set_last_cmd_status (game, ERROR_walk);
            return;
    }
    player_set_vision (player, ply_vision.pos_x, ply_vision.pos_y);


    /*convertir a celdas y validar contra el grid */
    cell_x = pos_current.pos_x / SCALE;
    cell_y = pos_current.pos_y / SCALE;

    /*si nos salimos del grid -> intento de transicion de space */
    if (cell_x < 0 || cell_x >= WIDHT ||
        cell_y < 0 || cell_y >= HIGHT)
    {
        if (game_rule_move (game) == OK)
        {
            game_set_last_cmd_status (game, OK);
            return;
        }
        game_set_last_cmd_status (game, ERROR_walk);
        return;
    }

    /* Dentro del grid: comprobamos transitabilidad */
    space = game_get_space (game, player_get_zone (player));
    if (!space) { game_set_last_cmd_status (game, ERROR_walk); return; }

    grid_line = space_get_grid_by_line (space, cell_y);
    if (!grid_line) { game_set_last_cmd_status (game, ERROR_walk); return; }

    /* solo 0 = no transitable.*/
    if (grid_line[cell_x] == 0)
        {
            game_set_last_cmd_status (game, ERROR_walk);
            return;
        }

    if (player_set_position (player, pos_current.pos_x, pos_current.pos_y) == ERROR)
        {
            game_set_last_cmd_status (game, ERROR_walk);
            return;
        }
    space_set_grid_by_position(space, pos_original, 1); 
    space_set_grid_by_position(space, pos_current, 0);
    /*Pequeño entorno disitno del resto del código doonde nos aseguramos de actualizar la posicón del numen_active */
    {




        game_rule_walk_active(game);
        /*==== MOVIMIENTO del los Numens Enemigos ====*/
        game_rule_walk_enemy (game);
    }
   


    game_set_last_cmd_status (game, OK);
}

/* ========================================================================= */
/*                                 TAKE                                       */
/* ========================================================================= */
static void
game_actions_take (Game* game)
{
    Player*  player;
    Space*   space;
    Object*  obj;
    Position obj_pos, ply_vision;
    Id       space_id, obj_id, dependency_id;
    Bool     movable;

    if (!game) return;

    obj_pos.pos_x = NO_POS; obj_pos.pos_y = NO_POS;

    player = game_get_player_by_turn (game);
    if (!player) { game_set_last_cmd_status (game, ERROR_take); return; }

    ply_vision = player_get_vision (player);

    space_id = player_get_zone (player);
    if (space_id == NO_ID) { game_set_last_cmd_status (game, ERROR_take); return; }
    space = game_get_space (game, space_id);
    if (!space) { game_set_last_cmd_status (game, ERROR_take); return; }

    obj = game_get_object_by_vision (game, ply_vision);
    if (!obj) { game_set_last_cmd_status (game, ERROR_take); return; }

    obj_id = obj_get_id (obj);
    if (obj_id == NO_ID) { game_set_last_cmd_status (game, ERROR_take); return; }

    if (space_contains_object (space, obj_id) == FALSE)
    { game_set_last_cmd_status (game, ERROR_take); return; }

    movable = obj_get_movable (obj);
    if (movable == FALSE)
    {
        dependency_id = obj_get_dependency (obj);
        if (dependency_id == NO_ID ||
            player_contains_object (player, dependency_id) == FALSE)
        { game_set_last_cmd_status (game, ERROR_take); return; }
    }

    space_remove_object (space, obj_id, obj_pos);
    if (player_add_object (player, obj_id) != OK)
    {
        space_set_object (space, obj_id, obj_pos);
        game_set_last_cmd_status (game, ERROR_take);
        return;
    }
    player_set_active_object (player, obj_id);

    game_set_last_cmd_status (game, OK);
}

/* ========================================================================= */
/*                                 DROP                                       */
/* ========================================================================= */
static void
game_actions_drop (Game* game)
{
    Player*  player         = NULL;
    Space*   space          = NULL;
    Object*  obj            = NULL;
    Command* last_command   = NULL;
    char*    obj_char       = NULL;
    Id       space_id, obj_id;
    Position ply_vision;

    if (!game) return;

    player = game_get_player_by_turn (game);
    if (!player) { game_set_last_cmd_status (game, ERROR_drop); return; }

    ply_vision = player_get_vision (player);
    if (ply_vision.pos_x == NO_POS || ply_vision.pos_y == NO_POS)
    { game_set_last_cmd_status (game, ERROR_drop); return; }

    last_command = game_get_last_command (game);
    if (!last_command) { game_set_last_cmd_status (game, ERROR_drop); return; }

    /* Si el comando lleva nombre de objeto: drop por nombre.
     * Si no: drop del active_object actual. */
    obj_char = command_get_target (last_command);
    if (obj_char && obj_char[0] != '\0')
    {
        obj = game_get_object_by_name (game, obj_char);
        if (!obj) { game_set_last_cmd_status (game, ERROR_drop); return; }
        obj_id = obj_get_id (obj);
    }
    else
    {
        obj_id = player_get_active_object (player);
        if (obj_id == NO_ID) { game_set_last_cmd_status (game, ERROR_drop); return; }
        obj = game_get_object_by_id (game, obj_id);
        if (!obj) { game_set_last_cmd_status (game, ERROR_drop); return; }
    }

    space_id = player_get_zone (player);
    if (space_id == NO_ID) { game_set_last_cmd_status (game, ERROR_drop); return; }
    space = game_get_space (game, space_id);
    if (!space) { game_set_last_cmd_status (game, ERROR_drop); return; }

    if (player_contains_object (player, obj_id) == FALSE)
    { game_set_last_cmd_status (game, ERROR_drop); return; }

    /* Mecanica del drop */
    player_delete_object (player, obj_id);
    space_set_object     (space, obj_id, ply_vision);
    obj_set_position     (obj, ply_vision.pos_x, ply_vision.pos_y);





    game_set_last_cmd_status (game, OK);
}

/* ========================================================================= */
/*                                ATTACK                                      */
/* ========================================================================= */
static void
game_actions_attack (Game* game)
{
    Player*   player;
    Space*    space;
    Numen     *num = NULL, *enemy_num = NULL;
    char*     skill_indx_ch;
    Skills_id skill = NO_SKILL;
    Id        space_id, num_id;
    Set*      space_numens;
    int       distance, skill_indx, radio;
    int       active_pos_x, active_pos_y, enemy_pos_x, enemy_pos_y;
    int       num_enemies, i;

    if (!game) return;

    player = game_get_player_at (game, PLAYER);
    if (!player) { game_set_last_cmd_status (game, ERROR_Attack); return; }

    num_id = player_get_active_numen (player);
    if (num_id == NO_ID) { game_set_last_cmd_status (game, ERROR_Attack); return; }

    space_id = player_get_zone (player);
    if (space_id == NO_ID) { game_set_last_cmd_status (game, ERROR_Attack); return; }

    skill_indx_ch = command_get_target (game_get_last_command (game));
    if (!skill_indx_ch) { game_set_last_cmd_status (game, ERROR_Attack); return; }

    skill_indx = atoi (skill_indx_ch);
    if (skill_indx < 0 || skill_indx >= N_SKILLS)
    { game_set_last_cmd_status (game, ERROR_Attack); return; }

    space = game_get_space      (game, space_id);
    num   = game_get_numen_by_id (game, num_id);
    if (!space || !num) { game_set_last_cmd_status (game, ERROR_Attack); return; }

    if (numen_get_health (num) <= 0)
    { game_set_last_cmd_status (game, ERROR_Attack); return; }

    skill = numen_get_skill_by_index (num, skill_indx);
    if (skill == NO_SKILL) { game_set_last_cmd_status (game, ERROR_Attack); return; }

    active_pos_x = numen_get_pos_x (num);
    active_pos_y = numen_get_pos_y (num);
    radio        = skill_get_radio (skill);

    space_numens = space_get_numens (space);
    if (!space_numens) { game_set_last_cmd_status (game, ERROR_Attack); return; }

    num_enemies = space_get_n_numens (space);

    /* Aplicamos el skill a TODOS los enemigos validos en el radio */
    for (i = 0; i < num_enemies; i++)
    {
        enemy_num = game_get_numen_by_id (game, set_get_id_at (space_numens, i));
        if (!enemy_num) continue;
        if (numen_get_id (enemy_num) == num_id) continue;
        if (numen_get_corrupt (enemy_num) == FALSE) continue;
        if (numen_get_health (enemy_num) <= 0) continue;

        enemy_pos_x = numen_get_pos_x (enemy_num);
        enemy_pos_y = numen_get_pos_y (enemy_num);
        distance    = (int)sqrt (pow (active_pos_x - enemy_pos_x, 2) +
                                 pow (active_pos_y - enemy_pos_y, 2));

        if ((int)(distance/SCALE) > (int)(radio/SCALE)) continue;

        if (skill_active (num, enemy_num, skill, (int)(distance/SCALE)) == ERROR)
        { game_set_last_cmd_status (game, ERROR_Attack); return; }

        /* Propagacion de FIRE_BALL: salpicadura a vecinos del impactado */
        if (skill == FIRE_BALL)
        {
            int radio_a = skill_get_radio (FIRE_BALL_A);
            int j, dx, dy, sec_dist;
            Numen* secondary;

            for (j = 0; j < num_enemies; j++)
            {
                secondary = game_get_numen_by_id (game, set_get_id_at (space_numens, j));
                if (!secondary || secondary == enemy_num) continue;
                if (numen_get_id (secondary) == num_id) continue;
                if (numen_get_corrupt (secondary) == FALSE) continue;
                if (numen_get_health (secondary) <= 0) continue;

                dx = numen_get_pos_x (secondary) - numen_get_pos_x (enemy_num);
                dy = numen_get_pos_y (secondary) - numen_get_pos_y (enemy_num);
                sec_dist = (int)sqrt (pow (dx, 2) + pow (dy, 2));

                if (sec_dist <= radio_a)
                    skill_active (enemy_num, secondary, FIRE_BALL_A, sec_dist);
            }
        }
    }

    game_set_last_cmd_status (game, OK);
}

/* ========================================================================= */
/*                                 CHAT                                       */
/* ========================================================================= */
static void
game_actions_chat (Game* game)
{
    Player* player;
    Space*  space;
    Numen*  ch;
    char*   char_name;
    Id      char_id, space_id;

    if (!game) return;

    player = game_get_player_by_turn (game);
    if (!player) { game_set_last_cmd_status (game, ERROR_Chat); return; }

    space_id = player_get_zone (player);
    if (space_id == NO_ID) { game_set_last_cmd_status (game, ERROR_Chat); return; }

    char_name = command_get_target (game_get_last_command (game));
    if (!char_name) { game_set_last_cmd_status (game, ERROR_Chat); return; }

    ch = game_get_numen_by_name (game, char_name);
    if (!ch) { game_set_last_cmd_status (game, ERROR_Chat); return; }

    char_id = numen_get_id (ch);
    space   = game_get_space (game, space_id);
    if (!space) { game_set_last_cmd_status (game, ERROR_Chat); return; }

    if (space_contains_character (space, char_id) == FALSE)
    { game_set_last_cmd_status (game, ERROR_Chat); return; }

    if (numen_get_following (ch) == NO_ID)
    { game_set_last_cmd_status (game, ERROR_Chat); return; }

    game_set_last_cmd_status (game, OK);
}

/* ========================================================================= */
/*                               INSPECT                                      */
/* ========================================================================= */
static void
game_actions_inspect (Game* game)
{
    Player* player;
    Space*  space;
    Object* obj;
    char*   obj_name;
    Id      obj_id, space_id;
    Bool    in_space, in_inventory;

    if (!game) return;

    player = game_get_player_by_turn (game);
    if (!player) { game_set_last_cmd_status (game, ERROR_inspect); return; }

    obj_name = command_get_target (game_get_last_command (game));
    if (!obj_name) { game_set_last_cmd_status (game, ERROR_inspect); return; }

    obj = game_get_object_by_name (game, obj_name);
    if (!obj) { game_set_last_cmd_status (game, ERROR_inspect); return; }

    obj_id   = obj_get_id (obj);
    space_id = player_get_zone (player);
    space    = game_get_space (game, space_id);

    in_space     = (space && space_contains_object (space, obj_id) == TRUE);
    in_inventory = player_contains_object (player, obj_id);

    if (in_space == FALSE && in_inventory == FALSE)
    { game_set_last_cmd_status (game, ERROR_inspect); return; }

    if (obj_get_description (obj) == NULL || obj_get_description (obj)[0] == '\0')
    { game_set_last_cmd_status (game, ERROR_inspect); return; }

    game_set_last_cmd_status (game, OK);
}

/* ========================================================================= */
/*                                  USE                                       */
/* ========================================================================= */
static void
game_actions_use (Game* game)
{
    Player* player;
    Object* obj;
    char*   obj_name;
    Id      obj_id;
    Effect  obj_effect;
    Status  result_effect;

    if (!game) return;

    player = game_get_player_by_turn (game);
    if (!player) { game_set_last_cmd_status (game, ERROR_use); return; }

    obj_name = command_get_target (game_get_last_command (game));
    if (!obj_name) { game_set_last_cmd_status (game, ERROR_use); return; }

    obj = game_get_object_by_name (game, obj_name);
    if (!obj) { game_set_last_cmd_status (game, ERROR_use); return; }

    obj_id = obj_get_id (obj);
    if (player_contains_object (player, obj_id) == FALSE)
    { game_set_last_cmd_status (game, ERROR_use); return; }

    obj_effect    = obj_get_effect (obj);
    result_effect = game_actions_apply_effect (game, obj, obj_effect);

    game_set_last_cmd_status (game, result_effect);
}

/* ========================================================================= */
/*                       SAVE / LOAD RECRUIT / KICK                         */
/*                                                          */
/* ========================================================================= */
static void
game_actions_save (Game* game)
{
    if (!game) { game_set_last_cmd_status (game, ERROR_save); return; }
    if (game_management_save_file (&game) == OK)
        game_set_last_cmd_status (game, OK);
    else
        game_set_last_cmd_status (game, ERROR_save);
}

/*
static void game_actions_load (Game *game)
{
    if(!game) { game_set_last_cmd_status (game, ERROR_load); return; }
}
    */

static void
game_actions_recruit (Game* game)
{
    Player*  player;
    Space*   space;
    Numen*   active_numen = NULL;
    Numen*   candidate;
    Numen*   best         = NULL;
    Set*     set_ids;
    Id       active_id, cand_id, space_id;
    int      n_numens, i, dx, dy, dist_sq, best_dist_sq;
    int      ax, ay;
    int      hp;
    Position pos_cand;

    if (!game) { game_set_last_cmd_status (game, ERROR_recruit); return; }

    player = game_get_player_at (game, PLAYER);
    if (!player) { game_set_last_cmd_status (game, ERROR_recruit); return; }

    if (player_get_n_numens (player) >= player_get_max_numens (player))
    { game_set_last_cmd_status (game, ERROR_recruit); return; }

    space_id = player_get_zone (player);
    space    = game_get_space (game, space_id);
    if (!space) { game_set_last_cmd_status (game, ERROR_recruit); return; }

    /* Centro de captura: el numen activo. Sin activo no se puede capturar. */
    active_id = player_get_active_numen (player);
    if (active_id == NO_ID)
    { game_set_last_cmd_status (game, ERROR_recruit); return; }

    active_numen = game_get_numen_by_id (game, active_id);
    if (!active_numen) { game_set_last_cmd_status (game, ERROR_recruit); return; }

    ax = numen_get_pos_x (active_numen);
    ay = numen_get_pos_y (active_numen);

    /* Buscar al mas cercano que cumpla con las condiciones, es más sencillo que lo del visual*/
    set_ids  = space_get_numens (space);
    n_numens = space_get_n_numens (space);

    best_dist_sq = (SCALE * 4) * (SCALE * 4);

    for (i = 0; i < n_numens; i++)
    {
        cand_id = set_get_id_at (set_ids, i);
        if (cand_id == NO_ID || cand_id == active_id) continue;

        candidate = game_get_numen_by_id (game, cand_id);
        if (!candidate) continue;

        /*FILTROS para capturar*/
        if (numen_get_corrupt (candidate) == TRUE) continue;
        if (numen_is_errant   (candidate) == FALSE) continue;
        if (player_contains_numen (player, cand_id) == TRUE) continue;

        hp = numen_get_health (candidate);
        if (hp <= 0 || hp > MAX_LIFE * 30 / 100) continue;

        pos_cand = numen_get_position (candidate);
        if (pos_cand.pos_x == NO_POS) continue;

        dx = pos_cand.pos_x - ax;
        dy = pos_cand.pos_y - ay;
        dist_sq = dx*dx + dy*dy;

        if (dist_sq <= best_dist_sq)
        {
            best_dist_sq = dist_sq;
            best         = candidate;
        }
    }

    if (!best) { game_set_last_cmd_status (game, ERROR_recruit); return; }

    /* CAPTURAR: pasa al backpack y deja de ser errante */
    if (player_add_numen (player, numen_get_id (best)) == ERROR)
    { game_set_last_cmd_status (game, ERROR_recruit); return; }

    if (numen_set_following (best, player_get_id (player)) == ERROR)
    { game_set_last_cmd_status (game, ERROR_recruit); return; }

    /* Lo sacamos del set del space, ya no esta en el mundo, esta en la bolsa*/
    space_remove_numen (space, numen_get_id (best),
                        numen_get_position (best));

    game_set_last_cmd_status (game, OK);
}

static void
game_actions_kick (Game* game)
{
  Player* player        = NULL;
    Numen* numen          = NULL;
    Command* last_command = NULL;
    char* numen_char      = NULL;
	Id numen_id;


    if (!game) return;

    /*Obtenemos el puntero a player*/
    player = game_get_player_at (game, PLAYER);
    if (!player)
        {
            game_set_last_cmd_status (game, ERROR_kick);
            return;
        }

    /*Obtenemos el utlimo comando del jgador*/
    last_command = game_get_last_command (game);
    if (!last_command)
        {
            game_set_last_cmd_status (game, ERROR_kick);
            return;
        }

    /*Obtenemos el numen que tiene activo el jugador*/
    numen_char = command_get_target (last_command);
    if (numen_char)
        {
            numen = game_get_numen_by_name (game, numen_char);
            if (!numen)
                {
                    game_set_last_cmd_status (game, ERROR_kick);
                    return;
                }
            numen_id = numen_get_id (numen);
        }
    else
        {
           numen_id = player_get_active_numen (player);
            numen    = game_get_numen_by_id (game, numen_id);
            if (!numen)
                {
                   game_set_last_cmd_status (game, ERROR_kick);
                    return;
                }
        }

    /*Volvemos a comprobar que el jugador tenga el numen en su invetario y que no es el último que le queda*/
    if (player_contains_numen (player, numen_id) == FALSE || player_get_n_numens(player)==1)
        {
            game_set_last_cmd_status (game, ERROR_kick);
            return;
        }

    player_delete_numen (player, numen_id);

    numen_destroy(numen);

    game_set_last_cmd_status (game, OK);
}

/* ========================================================================= */
/*                       PARSE DIRECTION                                      */
/* ========================================================================= */
Direction
ge_parse_direction (const char* str)
{
    if (!str) return U;
    if (strcasecmp (str, "north") == 0 || strcasecmp (str, "n") == 0) return N;
    if (strcasecmp (str, "south") == 0 || strcasecmp (str, "s") == 0) return S;
    if (strcasecmp (str, "east")  == 0 || strcasecmp (str, "e") == 0) return E;
    if (strcasecmp (str, "west")  == 0 || strcasecmp (str, "w") == 0) return W;
    return U;
}

/* ========================================================================= */
/*                           EFFECTS DISPATCHER                               */
/* ========================================================================= */
Status
game_actions_apply_effect (Game* game, Object* obj, Effect obj_effect)
{
    Status result = OK;

    if (!game || !obj) return ERROR;
    if (obj_effect == NO_EFECT) return OK;

    switch (obj_effect)
    {
        case HEALTH_PLUS: result = _apply_health_plus (game, obj); break;
        case HEALTH_LESS: result = _apply_health_less (game, obj); break;
        case SPEED_PLUS:  result = _apply_speed_plus  (game, obj); break;
        case SPEED_LESS:  result = _apply_speed_less  (game, obj); break;
        case OPEN:        result = _apply_open_door   (game, obj); break;
        default:          result = ERROR_use;                       break;
    }

    if (obj_get_consumable (obj) == TRUE) obj_set_id (obj, NO_ID);
    return result;
}

static Status
_apply_speed_plus (Game* game, Object* obj)
{
    if (!game || !obj) return ERROR_use;
    /* TODO */
    return OK;
}

static Status
_apply_speed_less (Game* game, Object* obj)
{
    if (!game || !obj) return ERROR_use;
    /* TODO */
    return OK;
}

static Status
_apply_health_plus (Game* game, Object* obj)
{
    Numen*  numen_active;
    Player* player;
    Id      active_id;
    int     life_update;

    if (!game || !obj) return ERROR_use;

    player = game_get_player (game);
    if (!player) return ERROR_use;

    active_id = player_get_active_numen (player);
    if (active_id == NO_ID) return ERROR_use;

    numen_active = game_get_numen_by_id (game, active_id);
    if (!numen_active) return ERROR_use;

    life_update = numen_get_health (numen_active) + obj_get_health (obj);
    if (numen_set_health (numen_active, life_update) == ERROR) return ERROR_use;

    return OK;
}

static Status
_apply_health_less (Game* game, Object* obj)
{
    Numen*  numen_active;
    Player* player;
    Id      active_id;
    int     life_update;

    if (!game || !obj) return ERROR_use;

    player = game_get_player (game);
    if (!player) return ERROR_use;

    /* MEJORA: la version vieja comparaba `numen_active == NO_ID` antes
     * de asignarse; era basura del stack. Comparamos active_id (que es
     * Id) contra NO_ID, que es lo correcto. */
    active_id = player_get_active_numen (player);
    if (active_id == NO_ID) return ERROR_use;

    numen_active = game_get_numen_by_id (game, active_id);
    if (!numen_active) return ERROR_use;

    life_update = numen_get_health (numen_active) - obj_get_health (obj);
    if (numen_set_health (numen_active, life_update) == ERROR) return ERROR_use;

    return OK;
}

static Status
_apply_open_door (Game* game, Object* obj)
{
    Player* player;
    Space*  space;
    Links*  link;
    Id      space_id;
    Id      link_id;

    if (!game || !obj) return ERROR_use;

    player = game_get_player (game);
    if (!player) return ERROR_use;

    space_id = player_get_zone (player);
    space    = game_get_space (game, space_id);
    if (!space) return ERROR_use;

    link_id = obj_get_open (obj);
    link    = game_get_link_by_id (game, link_id);
    if (!link) return ERROR_open;

    if (game_connection_is_open (game, space_id, link_get_direction (link)) == TRUE)
        return ERROR_open;

    if (link_get_origin_id (link) != space_id &&
        link_get_destination_id (link) != space_id)
        return ERROR_open;

    if (link_get_destination_id (link) == space_id)
    {
        if (link_set_open_dest_to_origin (link, TRUE) == ERROR) return ERROR_open;
    }
    else if (link_get_origin_id (link) == space_id)
    {
        if (link_set_open_origin_to_dest (link, TRUE) == ERROR) return ERROR_open;
    }

    return OK;
}
