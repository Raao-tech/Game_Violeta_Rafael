/**
 * @brief It implements the game update through user actions
 *
 * @file game_actions.c
 * @author Profesores PPROG
 * @version 2
 * @date 27-01-2025
 * @copyright GNU Public License
 */

#include "game_actions.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/**
   Private functions
*/
void game_actions_unknown(Game *game);
void game_actions_exit(Game *game);
void game_actions_next(Game *game);
void game_actions_back(Game *game);
void game_actions_left(Game *game);
void game_actions_right(Game *game);
void game_actions_take(Game *game);
void game_actions_drop(Game *game);
void game_actions_attack(Game *game);
void game_actions_chat(Game *game);

/**
   Game actions implementation
*/
Status game_actions_update(Game *game, Command *command){
  CommandCode cmd;

  game_set_last_command(game, command);

  cmd = command_get_code(command);

  switch (cmd){
    case UNKNOWN:
      game_actions_unknown(game);
      break;
    case EXIT:
      game_actions_exit(game);
      break;
    case NEXT:
      game_actions_next(game);
      break;
    case LEFT:
      game_actions_left(game);
      break;
    case RIGHT:
      game_actions_right(game);
      break;
    case BACK:
      game_actions_back(game);
      break;
    case TAKE:
      game_actions_take(game);
      break;
    case DROP:
      game_actions_drop(game);
      break;
    case ATTACK:
      game_actions_attack(game);
      break;
    case CHAT:
      game_actions_chat(game);
      break;
    default:
      break;
  }

  return OK;
}

/**
   Calls implementation for each action (Enfoque B: access Player/Space directly)
*/

void game_actions_unknown(Game *game) {
  game_set_last_cmd_status(game, ERROR);
}

void game_actions_exit(Game *game) {
  game_set_last_cmd_status(game, OK);
}

void game_actions_next(Game *game){
  Player *player = game_get_player(game);
  Id space_id = player_get_location(player);
  Space *space = NULL;
  Id next_id = NO_ID;

  if (space_id == NO_ID){
    game_set_last_cmd_status(game, ERROR_dir);
    return;
  }

  space = game_get_space(game, space_id);
  next_id = space_get_south(space);

  if (next_id != NO_ID){
    player_set_location(player, next_id);
    game_set_last_cmd_status(game, OK);
  } else {
    game_set_last_cmd_status(game, ERROR_dir);
  }
}

void game_actions_back(Game *game){
  Player *player = game_get_player(game);
  Id space_id = player_get_location(player);
  Space *space = NULL;
  Id back_id = NO_ID;

  if (space_id == NO_ID){
    game_set_last_cmd_status(game, ERROR_dir);
    return;
  }

  space = game_get_space(game, space_id);
  back_id = space_get_north(space);

  if (back_id != NO_ID){
    player_set_location(player, back_id);
    game_set_last_cmd_status(game, OK);
  } else {
    game_set_last_cmd_status(game, ERROR_dir);
  }
}

void game_actions_left(Game *game){
  Player *player = game_get_player(game);
  Id space_id = player_get_location(player);
  Space *space = NULL;
  Id left_id = NO_ID;

  if (space_id == NO_ID){
    game_set_last_cmd_status(game, ERROR_dir);
    return;
  }

  space = game_get_space(game, space_id);
  left_id = space_get_east(space);

  if (left_id != NO_ID){
    player_set_location(player, left_id);
    game_set_last_cmd_status(game, OK);
  } else {
    game_set_last_cmd_status(game, ERROR_dir);
  }
}

void game_actions_right(Game *game){
  Player *player = game_get_player(game);
  Id space_id = player_get_location(player);
  Space *space = NULL;
  Id right_id = NO_ID;

  if (space_id == NO_ID){
    game_set_last_cmd_status(game, ERROR_dir);
    return;
  }

  space = game_get_space(game, space_id);
  right_id = space_get_west(space);

  if (right_id != NO_ID){
    player_set_location(player, right_id);
    game_set_last_cmd_status(game, OK);
  } else {
    game_set_last_cmd_status(game, ERROR_dir);
  }
}

/* F12: take <object_name> */
void game_actions_take(Game *game){
  Player *player = game_get_player(game);
  Id space_id = player_get_location(player);
  Space *space = NULL;
  char *obj_name = NULL;
  Object *obj = NULL;
  Id obj_id = NO_ID;

  if (space_id == NO_ID){
    game_set_last_cmd_status(game, ERROR_take);
    return;
  }

  /* Get the object name from the command */
  obj_name = command_get_obj(game_get_last_command(game));
  if (!obj_name){
    game_set_last_cmd_status(game, ERROR_take);
    return;
  }

  /* Find the object by name in the game */
  obj = game_get_object_by_name(game, obj_name);
  if (!obj){
    game_set_last_cmd_status(game, ERROR_take);
    return;
  }

  obj_id = obj_get_id(obj);
  space = game_get_space(game, space_id);

  /* Check that the object is in the current space */
  if (space_contains_object(space, obj_id) == FALSE){
    game_set_last_cmd_status(game, ERROR_take);
    return;
  }

  /* Remove from space, add to player */
  space_remove_object(space, obj_id);
  player_add_object(player, obj_id);

  game_set_last_cmd_status(game, OK);
}

/* Drop: drops the first object (or specified by name) */
void game_actions_drop(Game *game){
  Player *player = game_get_player(game);
  Id space_id = player_get_location(player);
  Space *space = NULL;
  char *obj_name = NULL;
  Object *obj = NULL;
  Id obj_id = NO_ID;

  if (space_id == NO_ID || player_get_n_objects(player) == 0){
    game_set_last_cmd_status(game, ERROR_drop);
    return;
  }

  space = game_get_space(game, space_id);
  obj_name = command_get_obj(game_get_last_command(game));

  if (obj_name){
    /* Drop a specific object by name */
    obj = game_get_object_by_name(game, obj_name);
    if (!obj){
      game_set_last_cmd_status(game, ERROR_drop);
      return;
    }
    obj_id = obj_get_id(obj);
  } else {
    /* No name specified: error */
    game_set_last_cmd_status(game, ERROR_drop);
    return;
  }

  /* Check player has it */
  if (player_contains_object(player, obj_id) == FALSE){
    game_set_last_cmd_status(game, ERROR_drop);
    return;
  }

  /* Remove from player, add to space */
  player_delete_object(player, obj_id);
  space_set_object(space, obj_id);

  game_set_last_cmd_status(game, OK);
}

/* F10: attack <character_name> */
void game_actions_attack(Game *game){
  Player *player = game_get_player(game);
  Id space_id = player_get_location(player);
  Space *space = NULL;
  char *char_name = NULL;
  Character *ch = NULL;
  Id char_id = NO_ID;
  int roll;

  if (space_id == NO_ID){
    game_set_last_cmd_status(game, ERROR_Attack);
    return;
  }

  /* Get character name from command */
  char_name = command_get_obj(game_get_last_command(game));
  if (!char_name){
    game_set_last_cmd_status(game, ERROR_Attack);
    return;
  }

  /* Find character by name */
  ch = game_get_character_by_name(game, char_name);
  if (!ch){
    game_set_last_cmd_status(game, ERROR_Attack);
    return;
  }

  char_id = character_get_id(ch);
  space = game_get_space(game, space_id);

  /* Character must be in the same space */
  if (space_contains_character(space, char_id) == FALSE){
    game_set_last_cmd_status(game, ERROR_Attack);
    return;
  }

  /* Character must NOT be friendly */
  if (character_get_friendly(ch) == TRUE){
    game_set_last_cmd_status(game, ERROR_Attack);
    return;
  }

  /* Character must be alive */
  if (character_get_health(ch) <= 0){
    game_set_last_cmd_status(game, ERROR_Attack);
    return;
  }

  /* F10: random 0-9, 0-4 enemy wins, 5-9 player wins */
  roll = rand() % 10;

  if (roll < 5){
    /* Enemy wins: player loses 1 HP */
    player_set_health(player, player_get_health(player) - 1);
    if (player_get_health(player) <= 0){
      game_set_finished(game, TRUE);
    }
  } else {
    /* Player wins: enemy loses 1 HP */
    character_set_health(ch, character_get_health(ch) - 1);
    if(character_get_health(ch) <= 0){
      space_remove_character(space, char_id);
    }
  }

  game_set_last_cmd_status(game, OK);
}

/* F11: chat <character_name> */
void game_actions_chat(Game *game){
  Player *player = game_get_player(game);
  Id space_id = player_get_location(player);
  Space *space = NULL;
  char *char_name = NULL;
  Character *ch = NULL;
  Id char_id = NO_ID;

  if (space_id == NO_ID){
    game_set_last_cmd_status(game, ERROR_Chat);
    return;
  }

  /* Get character name from command */
  char_name = command_get_obj(game_get_last_command(game));
  if (!char_name){
    game_set_last_cmd_status(game, ERROR_Chat);
    return;
  }

  /* Find character by name */
  ch = game_get_character_by_name(game, char_name);
  if (!ch){
    game_set_last_cmd_status(game, ERROR_Chat);
    return;
  }

  char_id = character_get_id(ch);
  space = game_get_space(game, space_id);

  /* Character must be in the same space */
  if (space_contains_character(space, char_id) == FALSE){
    game_set_last_cmd_status(game, ERROR_Chat);
    return;
  }

  /* Character must be friendly */
  if (character_get_friendly(ch) == FALSE){
    game_set_last_cmd_status(game, ERROR_Chat);
    return;
  }

  /* The message will be displayed by graphic_engine reading from the character */
  /* We store the character id so graphic_engine knows who spoke last */
  /* For now, the graphic_engine will check the last command and find the character */

  game_set_last_cmd_status(game, OK);
}
