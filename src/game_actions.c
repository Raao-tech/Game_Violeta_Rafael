/**
 * @brief It implements the game update through user actions
 *
 * @file game_actions.c
 * @author Profesores PPROG, Violeta, Rafael and Javier
 * @version 4
 * @date 11-04-2026
 * @copyright GNU Public License
 */

#include "game_actions.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <time.h>

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

Status game_actions_update(Game *game, Command *command) {
  CommandCode cmd;

  if (!game || !command) return ERROR;
  if (game_set_last_command(game, command) == ERROR) return ERROR;

  cmd = command_get_code(command);

  switch (cmd) {
    case UNKNOWN: game_actions_unknown(game); break;
    case EXIT:    game_actions_exit(game);    break;
    case MOVE:    game_actions_move(game);    break;
    case TAKE:    game_actions_take(game);    break;
    case DROP:    game_actions_drop(game);    break;
    case ATTACK:  game_actions_attack(game);  break;
    case CHAT:    game_actions_chat(game);    break;
    case INSPECT: game_actions_inspect(game); break;
    default:      break;
  }

  return OK;
}


/* ========================================================================= */
/*                       PRIVATE: ACTION HANDLERS                            */
/* ========================================================================= */

/* ---- UNKNOWN ---- */
static void game_actions_unknown(Game *game) {
  if (!game) return;
  game_set_last_cmd_status(game, ERROR);
}

/* ---- EXIT ---- */
static void game_actions_exit(Game *game) {
  if (!game) return;
  game_set_last_cmd_status(game, OK);
}


/* ========================================================================= */
/*                    MOVE (F8): move <direction>                             */
/* ========================================================================= */


static void game_actions_move(Game *game) {
  Player    *player  = NULL;
  Space     *dest_sp = NULL;
  char      *dir_str = NULL;
  Direction  dir;
  Id         origin, dest;

  if (!game) return;

  player = game_get_player_by_turn(game);
  if (!player) {
    game_set_last_cmd_status(game, ERROR_dir);
    return;
  }

  origin = player_get_location(player);
  if (origin == NO_ID) {
    game_set_last_cmd_status(game, ERROR_dir);
    return;
  }

  dir_str = command_get_obj(game_get_last_command(game));
  if (!dir_str) {
    game_set_last_cmd_status(game, ERROR_dir);
    return;
  }

  dir = ge_parse_direction(dir_str);
  if (dir == U) {
    game_set_last_cmd_status(game, ERROR_dir);
    return;
  }

  dest = game_get_connection(game, origin, dir);
  if (dest == NO_ID) {
    game_set_last_cmd_status(game, ERROR_dir);
    return;
  }

  if (game_connection_is_open(game, origin, dir) == FALSE) {
    game_set_last_cmd_status(game, ERROR_dir);
    return;
  }

  player_set_location(player, dest);

  dest_sp = game_get_space(game, dest);
  if (dest_sp) space_set_discovered(dest_sp, TRUE);

  game_set_last_cmd_status(game, OK);
}


/* ========================================================================= */
/*                    TAKE (F10): take <object_name>                          */
/* ========================================================================= */
static void game_actions_take(Game *game) {
  Player *player   = NULL;
  Space  *space    = NULL;
  Object *obj      = NULL;
  char   *obj_name = NULL;
  Id      space_id, obj_id;

  if (!game) return;

  player = game_get_player_by_turn(game);
  if (!player) {
    game_set_last_cmd_status(game, ERROR_take);
    return;
  }

  space_id = player_get_location(player);
  if (space_id == NO_ID) {
    game_set_last_cmd_status(game, ERROR_take);
    return;
  }

  obj_name = command_get_obj(game_get_last_command(game));
  if (!obj_name) {
    game_set_last_cmd_status(game, ERROR_take);
    return;
  }

  obj = game_get_object_by_name(game, obj_name);
  if (!obj) {
    game_set_last_cmd_status(game, ERROR_take);
    return;
  }

  obj_id = obj_get_id(obj);
  space  = game_get_space(game, space_id);

  if (space_contains_object(space, obj_id) == FALSE) {
    game_set_last_cmd_status(game, ERROR_take);
    return;
  }

  space_remove_object(space, obj_id);

  if (player_add_object(player, obj_id) != OK) {
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
static void game_actions_drop(Game *game) {
  Player *player   = NULL;
  Space  *space    = NULL;
  Object *obj      = NULL;
  char   *obj_name = NULL;
  Id      space_id, obj_id;

  if (!game) return;

  player = game_get_player_by_turn(game);
  if (!player) {
    game_set_last_cmd_status(game, ERROR_drop);
    return;
  }

  space_id = player_get_location(player);
  if (space_id == NO_ID) {
    game_set_last_cmd_status(game, ERROR_drop);
    return;
  }

  obj_name = command_get_obj(game_get_last_command(game));
  if (!obj_name) {
    game_set_last_cmd_status(game, ERROR_drop);
    return;
  }

  obj = game_get_object_by_name(game, obj_name);
  if (!obj) {
    game_set_last_cmd_status(game, ERROR_drop);
    return;
  }

  obj_id = obj_get_id(obj);

  if (player_contains_object(player, obj_id) == FALSE) {
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
 *     same space, hostile, alive → random roll.
 *
 *  2. PvP combat (NEW):
 *     "attack witch" → if no Character found with that name,
 *     search for a Player with that name → must be in same space,
 *     cannot be yourself → random roll.
 */
static void game_actions_attack(Game *game) {
  Player    *player    = NULL;
  Space     *space     = NULL;
  Character *ch        = NULL;
  char      *name      = NULL;
  Id         space_id;
  int        roll;

  if (!game) return;

  player = game_get_player_by_turn(game);
  if (!player) {
    game_set_last_cmd_status(game, ERROR_Attack);
    return;
  }

  space_id = player_get_location(player);
  if (space_id == NO_ID) {
    game_set_last_cmd_status(game, ERROR_Attack);
    return;
  }

  name = command_get_obj(game_get_last_command(game));
  if (!name) {
    game_set_last_cmd_status(game, ERROR_Attack);
    return;
  }

  space = game_get_space(game, space_id);

  /* ========== PHASE 1: Try as NPC (Character) ========== */
  ch = game_get_character_by_name(game, name);

  if (ch) {
    Id char_id = character_get_id(ch);

    /* Must be in the same space */
    if (space_contains_character(space, char_id) == FALSE) {
      game_set_last_cmd_status(game, ERROR_Attack);
      return;
    }

    /* Must NOT be friendly */
    if (character_get_friendly(ch) == TRUE) {
      game_set_last_cmd_status(game, ERROR_Attack);
      return;
    }

    /* Must be alive */
    if (character_get_health(ch) <= 0) {
      game_set_last_cmd_status(game, ERROR_Attack);
      return;
    }

    /* NPC Combat roll */
    roll = rand() % 10;

    if (roll < 5) {
      /* Enemy wins: player loses 1 HP */
      player_set_health(player, player_get_health(player) - 1);
      if (player_get_health(player) <= 0) {
        game_set_finished(game, TRUE);
      }
    } else {
      /* Player wins: enemy loses 1 HP */
      character_set_health(ch, character_get_health(ch) - 1);
      if (character_get_health(ch) <= 0) {
        space_remove_character(space, char_id);
      }
    }

    game_set_last_cmd_status(game, OK);
    return;
  }

  /*Inciiamos nuestro ambioto local. Toda variable declarada dentri de los {}  sólo existe ahí*/
  /*
    Se suele usar para evitar tener que declarar target arriba de todo.  
    -Wependatic es muyyyy pedante y obliga a declarar todas las variables
    según el estandar c99.   Lamentando lo mucho, no lo tenemos así, y me da pereza
    cambiar todo desde cero, (incluyendo lógica), pero también es muy útil para evitar errores de
    scope, por lo que iré a reescribir partes del código para usar este estilo.
    Se les quiere!!! besitosss ;D
  */
  /* ========== PHASE 2: Try as Player (PvP) ========== */
   {
    Player *target = game_get_player_by_name(game, name);

    if (!target) {
      game_set_last_cmd_status(game, ERROR_Attack);
      return;
    }

    /* Cannot attack yourself */
    if (player_get_id(target) == player_get_id(player)) {
      game_set_last_cmd_status(game, ERROR_Attack);
      return;
    }

    /* Target must be in the same space */
    if (player_get_location(target) != space_id) {
      game_set_last_cmd_status(game, ERROR_Attack);
      return;
    }

    /* Target must be alive */
    if (player_get_health(target) <= 0) {
      game_set_last_cmd_status(game, ERROR_Attack);
      return;
    }

    /* PvP Combat roll */
    roll = rand() % 20;

    if (roll < 5) {
      player_set_health(player, player_get_health(player) - 1);
      /*game_turn_update saltará al muerto */
    } else {
      player_set_health(target, player_get_health(target) - 1);
      /*game_turn_update saltará al muerto */
    }

    game_set_last_cmd_status(game, OK);
    return;
  }
}


/* ========================================================================= */
/*                    CHAT: chat <character_name>                             */
/* ========================================================================= */
static void game_actions_chat(Game *game) {
  Player    *player    = NULL;
  Space     *space     = NULL;
  Character *ch        = NULL;
  char      *char_name = NULL;
  Id         char_id, space_id;

  if (!game) return;

  player = game_get_player_by_turn(game);
  if (!player) {
    game_set_last_cmd_status(game, ERROR_Chat);
    return;
  }

  space_id = player_get_location(player);
  if (space_id == NO_ID) {
    game_set_last_cmd_status(game, ERROR_Chat);
    return;
  }

  char_name = command_get_obj(game_get_last_command(game));
  if (!char_name) {
    game_set_last_cmd_status(game, ERROR_Chat);
    return;
  }

  ch = game_get_character_by_name(game, char_name);
  if (!ch) {
    game_set_last_cmd_status(game, ERROR_Chat);
    return;
  }

  char_id = character_get_id(ch);
  space   = game_get_space(game, space_id);

  if (space_contains_character(space, char_id) == FALSE) {
    game_set_last_cmd_status(game, ERROR_Chat);
    return;
  }

  if (character_get_friendly(ch) == FALSE) {
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
static void game_actions_inspect(Game *game) {
  Player *player   = NULL;
  Space  *space    = NULL;
  Object *obj      = NULL;
  char   *obj_name = NULL;
  Id      obj_id, space_id;
  Bool    in_space, in_inventory;

  if (!game) return;

  player = game_get_player_by_turn(game);
  if (!player) {
    game_set_last_cmd_status(game, ERROR_inspect);
    return;
  }

  obj_name = command_get_obj(game_get_last_command(game));
  if (!obj_name) {
    game_set_last_cmd_status(game, ERROR_inspect);
    return;
  }

  obj = game_get_object_by_name(game, obj_name);
  if (!obj) {
    game_set_last_cmd_status(game, ERROR_inspect);
    return;
  }

  obj_id   = obj_get_id(obj);
  space_id = player_get_location(player);
  space    = game_get_space(game, space_id);

  /* Object must be accessible: in current space OR in inventory */
  in_space     = (space && space_contains_object(space, obj_id));
  in_inventory = player_contains_object(player, obj_id);

  if (in_space == FALSE && in_inventory == FALSE) {
    game_set_last_cmd_status(game, ERROR_inspect);
    return;
  }

  /* Object must have a description */
  if (obj_get_description(obj) == NULL ||
      obj_get_description(obj)[0] == '\0') {
    game_set_last_cmd_status(game, ERROR_inspect);
    return;
  }

  game_set_last_cmd_status(game, OK);
}


/* ========================================================================= */
/*                      HELPER: PARSE DIRECTION                              */
/* ========================================================================= */

static Direction ge_parse_direction(const char *str) {
  if (!str) return U;

  if (strcasecmp(str, "north") == 0 || strcasecmp(str, "n") == 0)
    return N;
  if (strcasecmp(str, "south") == 0 || strcasecmp(str, "s") == 0)
    return S;
  if (strcasecmp(str, "east") == 0  || strcasecmp(str, "e") == 0)
    return E;
  if (strcasecmp(str, "west") == 0  || strcasecmp(str, "w") == 0)
    return W;

  return U;
}