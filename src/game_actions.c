/**
 * @brief It implements the game update through user actions
 *
 * @file game_actions.c
 * @author Profesores PPROG, Violeta, Rafael, Salvador and Javier
 * @version 5
 * @date 23-04-2026
 * @copyright GNU Public License
 */
#include "game_rules.h"
#include "game_management.h"
#include "game_actions.h"
#include "raylib.h" /*esto es una prueba*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <time.h>
#include <math.h>

/**
 * Private function prototypes.
 * Each handles one command type and sets the last_cmd_status in game.
 */
static void game_actions_unknown(Game *game);
static void game_actions_exit(Game *game);
static void game_actions_move(Game *game);
static void game_actions_take(Game *game);
static void game_actions_drop(Game *game);
static void game_actions_attack(Game *game);
static void game_actions_chat(Game *game);
static void game_actions_inspect(Game *game);
static void game_actions_use(Game *game);
static void game_actions_open(Game *game);
static void game_actions_save(Game *game);
static void game_actions_load(Game *game);
static void game_actions_recruit(Game *game);
static void game_actions_kick(Game *game);
/**
 * @brief Converts a direction string to a Direction enum value
 *
 * Accepts "north"/"n", "south"/"s", "east"/"e", "west"/"w"
 * (case-insensitive).  Returns U if unrecognized.
 */
static Direction ge_parse_direction(const char *str);

/* ========================================================================= */
/*                          PUBLIC: DISPATCHER                               */
/* ========================================================================= */

/*
 * La idea seria que ahora no solo seactivaran las caciones ,por medio del teclado. esto se mantendrá, pero
 * el comando central tiene que ser el input del usario por medio del teclado,  player 1 y 2 pueden mverse
 * gracias a "wasd" o a las flechas "Up, Down, Right, Left" y tendrànm posibildiad de hacer otras cosas con otras "KEYs"
 * del teclado. Tal vez lo del Keyboards no es el tipo de dato, eso se pued equitar, era para no olvidar que ese cambio se tienme que hacer
 *  Pero ta,mbien hay que mantender ewl command porque las pruebas automatixadas se hacen atraves de texto, asi que eso se debe de jdjar
 * para los logs, luego veré como hago coo
 *
 *   Punto importante:                         Modo de Juego (SourceInput)
 *       Modo Test                                     Modo Juego
 *  (Textual, no visual)                         (keys de Keyboard, Visual)
 *  (comandos por player, No en paralelo)        (comandos dependientes de "wasd" o flechas, en paralelo)
 *
 *  Una posible propuesta de adaptación es la siguiente:
 *      la estrctura de command tendrá : {}
 *
 *
 *
 */
Status game_actions_update(Game *game, Command *command)
{
  CommandCode cmd;

  if (!game || !command)
    return ERROR;
  if (game_set_last_command(game, command) == ERROR)
    return ERROR;

  cmd = command_get_code(command);

  switch (cmd)
  {
	case UNKNOWN:	game_actions_unknown(game); break;
	case EXIT:		game_actions_exit(game);    break;
	case MOVE:		game_actions_move(game);    break;
	case TAKE:		game_actions_take(game);    break;
	case DROP:		game_actions_drop(game);	break;
	case ATTACK:	game_actions_attack(game);	break;
	case CHAT:		game_actions_chat(game);	break;
	case INSPECT:	game_actions_inspect(game);	break;
	case USE:		game_actions_use(game);		break;
	case OPEN:		game_actions_open(game);	break;
	case SAVE:		game_actions_save(game);	break;
	case LOAD:		game_actions_load(game);	break;
	case RECRUIT:	game_actions_recruit(game);	break;
	case KICK:		game_actions_kick(game);	break;
	default:									break;
  }
  return OK;
}

/* ========================================================================= */
/*                       PRIVATE: ACTION HANDLERS                            */
/* ========================================================================= */

/* ---- UNKNOWN ---- */
static void game_actions_unknown(Game *game)
{
  if (!game) return;
  game_set_last_cmd_status(game, ERROR);
}

/* ---- EXIT ---- */
static void game_actions_exit(Game *game)
{
  if (!game) return;
  game_set_last_cmd_status(game, OK);
}

/* ========================================================================= */
/*                    MOVE (F8): move <direction>                             */
/* ========================================================================= */

static void game_actions_move(Game *game)
{
  if(!game||game_rule_move(game)==ERROR)
  {
    game_set_last_cmd_status(game, ERROR);
    return;
  }
  game_set_last_cmd_status(game, OK);
  return;
}

/* ========================================================================= */
/*                    TAKE (F10): take <object_name>                          */
/* ========================================================================= */
static void game_actions_take(Game *game)
{
  Player *player = NULL;
  Space *space = NULL;
  Object *obj = NULL;
  char *obj_name = NULL;
  Id space_id, obj_id, dependency_id;
  Bool movable;

  if (!game)
    return;

  /*
   * Este tipo de comprobaciones hay que ponerlas en mira, para la solución del multijugador
   * en paralelo, hay que ver que jugador esta haciendo la ccion, pero no es por turnos. Hay una cantida
   * n de jugadores establecida en alguna macro que dependerá de la enumaercaicon de tipos de datos PLY
   */
  player = game_get_player_by_turn(game);
  if (!player)
  {
    game_set_last_cmd_status(game, ERROR_take);
    return;
  }

  space_id = player_get_zone(player);
  if (space_id == NO_ID)
  {
    game_set_last_cmd_status(game, ERROR_take);
    return;
  }

  obj_name = command_get_target(game_get_last_command(game));
  if (!obj_name)
  {
    game_set_last_cmd_status(game, ERROR_take);
    return;
  }

  obj = game_get_object_by_name(game, obj_name);
  if (!obj)
  {
    game_set_last_cmd_status(game, ERROR_take);
    return;
  }

  obj_id = obj_get_id(obj);
  space = game_get_space(game, space_id);

  if (space_contains_object(space, obj_id) == FALSE)
  {
    game_set_last_cmd_status(game, ERROR_take);
    return;
  }
  movable = obj_get_movable(obj);
  if (movable == FALSE)
  {

    dependency_id = obj_get_dependency(obj);
    if (dependency_id != NO_ID && player_contains_object(player, dependency_id) == TRUE)
    {
      /* The object is not movable, but the player has the required dependency in inventory */
      /* Allow taking the object */
    }
    else
    {
      game_set_last_cmd_status(game, ERROR_take);
      return;
    }
  }
  space_remove_object(space, obj_id);

  if (player_add_object(player, obj_id) != OK)
  {
    /* Inventory full — put the object back */
    space_set_object(space, obj_id);
    game_set_last_cmd_status(game, ERROR_take);
    return;
  }

  game_set_last_cmd_status(game, OK);
}

/* ========================================================================= */
/*                    DROP: drop <object_name>                          */
/* ========================================================================= */
static void game_actions_drop(Game *game)
{
  Player *player = NULL;
  Space *space = NULL;
  Object *obj = NULL;
  char *obj_name = NULL;
  Id space_id, obj_id;

  if (!game)
    return;

  player = game_get_player_by_turn(game);
  if (!player)
  {
    game_set_last_cmd_status(game, ERROR_drop);
    return;
  }

  space_id = player_get_zone(player);
  if (space_id == NO_ID)
  {
    game_set_last_cmd_status(game, ERROR_drop);
    return;
  }

  obj_name = command_get_target(game_get_last_command(game));
  if (!obj_name)
  {
    game_set_last_cmd_status(game, ERROR_drop);
    return;
  }

  obj = game_get_object_by_name(game, obj_name);
  if (!obj)
  {
    game_set_last_cmd_status(game, ERROR_drop);
    return;
  }

  obj_id = obj_get_id(obj);

  if (player_contains_object(player, obj_id) == FALSE)
  {
    game_set_last_cmd_status(game, ERROR_drop);
    return;
  }

  player_delete_object(player, obj_id);
  space = game_get_space(game, space_id);
  space_set_object(space, obj_id);

  game_set_last_cmd_status(game, OK);
}

/* ========================================================================= */
/*            ATTACK: attack <name>  (NPC or PvP)                            */
/* ========================================================================= */
/*
 * The attack command now supports TWO modes:
 *
 *  1. NPC combat (original):
 *     "attack Skeleton" → find Character by name → must be in
 *     same spacee, hostile, alive → random roll.
 *
 *  2. PvP combat (NEW):
 *     "attack witch" -> if no Character found with that name,
 *     search for a Player with that name → must be in same space,
 *     cannot be yourself -> random roll anddd game_turn_update can whit all
 *     trust sjsj
 */
static void game_actions_attack(Game *game)
{
  Player *player = NULL;
  Space *space = NULL;
  Numen *num = NULL, *enemy_num = NULL;
  char *skill_indx_ch = NULL;
  Skills_id skill = TAKLE;
  Id space_id, num_id;
  Set *space_numens = NULL;
  int roll, distance, skill_indx, radio, active_pos_x, active_pos_y, enemy_pos_x, enemy_pos_y, num_enemies, i;

  if (!game)
    return;

  player = game_get_player_at(game, PLAYER);
  if (!player)
  {
    game_set_last_cmd_status(game, ERROR_Attack);
    return;
  }
  num_id = player_get_active_numen(player);
  if (num_id == NO_ID)
  {
    game_set_last_cmd_status(game, ERROR_Attack);
    return;
  }
  space_id = player_get_zone(player); 
  if (space_id == NO_ID)
  {
    game_set_last_cmd_status(game, ERROR_Attack);
    return;
  }

  skill_indx_ch = command_get_target(game_get_last_command(game));
  if (!skill_indx)
  {
    game_set_last_cmd_status(game, ERROR_Attack);
    return;
  }

  skill_indx = atoi(skill_indx_ch);
  if (skill_indx < 0 || skill_indx >= MAX_HELD_SKILLS)
  {
    game_set_last_cmd_status(game, ERROR_Attack);
    return;
  }

  space = game_get_space(game, space_id);
  num = game_get_numen_by_id(game, num_id); 
  if (!space || !num)
  {
    game_set_last_cmd_status(game, ERROR_Attack);
    return;
  }
  active_pos_x = numen_get_pos_x(num); 
  active_pos_y = numen_get_pos_y(num); 

  if (numen_get_health(num) <= 0)
  {
    game_set_last_cmd_status(game, ERROR_Attack);
    return;
  }

  skill = numen_get_skill_by_index(num, skill_indx); 
  if (skill == NO_SKILL)
  {
    game_set_last_cmd_status(game, ERROR_Attack);
    return;
  }

  radio = skill_get_radio(skill); /*por implementar*/

  space_numens = space_get_numens(space); 
  if (!space_numens)
  {
    game_set_last_cmd_status(game, ERROR_Attack);
    return;
  }

  num_enemies = space_get_n_numens(space); 

 /* For simplicity, we apply the skill effect to all valid targets in range. */
  for (i = 0; i < num_enemies; i++)
  {
    enemy_num = game_get_numen_by_id(game, set_get_id_at(space_numens, i)); 
    if (enemy_num && enemy_num && numen_get_id(enemy_num) != num_id && numen_get_corrupt(enemy_num) == FALSE)
    {
      enemy_pos_x = numen_get_pos_x(enemy_num);
      enemy_pos_y = numen_get_pos_y(enemy_num);
      distance = sqrt(pow(active_pos_x - enemy_pos_x, 2) + pow(active_pos_y - enemy_pos_y, 2));
      if (distance <= radio)
      {
        if(skill_active(num, enemy_num, skill) == ERROR)  /*por implementar status skill_apply_effect*/
        {
          game_set_last_cmd_status(game, ERROR_Attack);
          return;
        }
      }
    }
  }

  game_set_last_cmd_status(game, OK);
  return;


}

/* ========================================================================= */
/*                    CHAT: chat <character_name>                             */
/* ========================================================================= */
static void game_actions_chat(Game *game)
{
  Player *player = NULL;
  Space *space = NULL;
  Numen *ch = NULL;
  char *char_name = NULL;
  Id char_id, space_id;

  if (!game)
    return;

  player = game_get_player_by_turn(game);
  if (!player)
  {
    game_set_last_cmd_status(game, ERROR_Chat);
    return;
  }

  space_id = player_get_zone(player);
  if (space_id == NO_ID)
  {
    game_set_last_cmd_status(game, ERROR_Chat);
    return;
  }

  char_name = command_get_target(game_get_last_command(game));
  if (!char_name)
  {
    game_set_last_cmd_status(game, ERROR_Chat);
    return;
  }

  ch = game_get_numen_by_name(game, char_name);
  if (!ch)
  {
    game_set_last_cmd_status(game, ERROR_Chat);
    return;
  }

  char_id = numen_get_id(ch);
  space = game_get_space(game, space_id);

  if (space_contains_character(space, char_id) == FALSE)
  {
    game_set_last_cmd_status(game, ERROR_Chat);
    return;
  }

  if (numen_get_following(ch) == FALSE)
  {
    game_set_last_cmd_status(game, ERROR_Chat);
    return;
  }

  game_set_last_cmd_status(game, OK);
}

/* ========================================================================= */
/*           INSPECT (F9): inspect <object_name>                             */
/* ========================================================================= */
/*
 */
static void game_actions_inspect(Game *game)
{
  Player *player = NULL;
  Space *space = NULL;
  Object *obj = NULL;
  char *obj_name = NULL;
  Id obj_id, space_id;
  Bool in_space, in_inventory;

  if (!game)
    return;

  player = game_get_player_by_turn(game);
  if (!player)
  {
    game_set_last_cmd_status(game, ERROR_inspect);
    return;
  }

  obj_name = command_get_target(game_get_last_command(game));
  if (!obj_name)
  {
    game_set_last_cmd_status(game, ERROR_inspect);
    return;
  }

  obj = game_get_object_by_name(game, obj_name);
  if (!obj)
  {
    game_set_last_cmd_status(game, ERROR_inspect);
    return;
  }

  obj_id = obj_get_id(obj);
  space_id = player_get_zone(player);
  space = game_get_space(game, space_id);

  /* Object must be accessible: in current space OR in inventory */
  in_space = (space && space_contains_object(space, obj_id));
  in_inventory = player_contains_object(player, obj_id);

  if (in_space == FALSE && in_inventory == FALSE)
  {
    game_set_last_cmd_status(game, ERROR_inspect);
    return;
  }

  /* Object must have a description */
  if (obj_get_description(obj) == NULL ||
      obj_get_description(obj)[0] == '\0')
  {
    game_set_last_cmd_status(game, ERROR_inspect);
    return;
  }

  game_set_last_cmd_status(game, OK);
}

/* ========================================================================= */
/*           USE: use <object_name>                                          */
/* ========================================================================= */
static void game_actions_use(Game *game)
{
  Player *player = NULL;
  Object *obj = NULL;
  char *obj_name = NULL;
  Id obj_id;
  Bool in_inventory = FALSE, consumable;
  int obj_health = 0;
  if (!game)
    return;

  player = game_get_player_by_turn(game);
  if (!player)
  {
    game_set_last_cmd_status(game, ERROR_use);
    return;
  }

  obj_name = command_get_target(game_get_last_command(game));
  if (!obj_name)
  {
    game_set_last_cmd_status(game, ERROR_use);
    return;
  }

  obj = game_get_object_by_name(game, obj_name);
  if (!obj)
  {
    game_set_last_cmd_status(game, ERROR_use);
    return;
  }

  obj_id = obj_get_id(obj);

  /* Object must be accessible: in inventory */

  in_inventory = player_contains_object(player, obj_id);

  if (in_inventory == FALSE)
  {
    game_set_last_cmd_status(game, ERROR_use);
    return;
  }

  consumable = obj_get_consumable(obj);

  obj_health = obj_get_health(obj);

  /*Object of health or damage*/
  if (obj_health != 0)
  {
    player_set_health(player, player_get_health(player) + obj_health);
    if (consumable == TRUE)
    {
      player_delete_object(player, obj_id);
    }

    game_set_last_cmd_status(game, OK);
    return;
  }
  /*****************************/

  game_set_last_cmd_status(game, OK);
  return;
}
/* ========================================================================= */
/*                      OPEN: open with <object_name>                        */
/* ========================================================================= */
static void game_actions_open(Game *game)
{
  Player *player = NULL;
  Object *key = NULL;
  char *key_name = NULL;
  Id key_id, space_id, link_id = NO_ID;
  Bool in_inventory = FALSE;
  int obj_health = 0;
  Links *link = NULL;

  if (!game)
    return;

  player = game_get_player_by_turn(game);
  if (!player)
  {
    game_set_last_cmd_status(game, ERROR_open);
    return;
  }

  key_name = command_get_target(game_get_last_command(game));
  if (!key_name)
  {
    game_set_last_cmd_status(game, ERROR_open);
    return;
  }

  key = game_get_object_by_name(game, key_name);
  if (!key)
  {
    game_set_last_cmd_status(game, ERROR_open);
    return;
  }

  key_id = obj_get_id(key);

  /* Object must be accessible: in inventory */

  in_inventory = player_contains_object(player, key_id);

  if (in_inventory == FALSE)
  {
    game_set_last_cmd_status(game, ERROR_open);
    return;
  }

  space_id = player_get_zone(player);

  link_id = obj_get_open(key);
  link = game_get_link_by_id(game, link_id);
  if (!link)
  {
    game_set_last_cmd_status(game, ERROR_open);
    return;
  }

  if (game_connection_is_open(game, space_id, link_get_direction(link)) == TRUE)
  {
    game_set_last_cmd_status(game, ERROR_use);
    return;
  }

  if (link_get_origin_id(link) != space_id && link_get_destination_id(link) != space_id)
  {
    game_set_last_cmd_status(game, ERROR_use);
    return;
  }

  if (link_get_destination_id(link) == space_id)
  {
    link_set_open_dest_to_origin(link, TRUE);
  }
  else if (link_get_origin_id(link) == space_id)
  {
    link_set_open_origin_to_dest(link, TRUE);
  }
  game_set_last_cmd_status(game, OK);
  return;
}

/* ========================================================================= */
/*                      SAVE: save                                           */
/* ========================================================================= */
static void game_actions_save(Game *game)
{
  if (!game)
  {
    game_set_last_cmd_status(game, ERROR_save);
    return;
  }

  if (game_save_file(&game) == OK)
    game_set_last_cmd_status(game, OK);
  else
  {
    game_set_last_cmd_status(game, ERROR_save);
  }
  return;
}

/* ========================================================================= */
/*                      HELPER: PARSE DIRECTION                              */
/* ========================================================================= */

static Direction ge_parse_direction(const char *str)
{
  if (!str)
    return U;

  if (strcasecmp(str, "north") == 0 || strcasecmp(str, "n") == 0)
    return N;
  if (strcasecmp(str, "south") == 0 || strcasecmp(str, "s") == 0)
    return S;
  if (strcasecmp(str, "east") == 0 || strcasecmp(str, "e") == 0)
    return E;
  if (strcasecmp(str, "west") == 0 || strcasecmp(str, "w") == 0)
    return W;

  return U;
}