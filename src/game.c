/**
 * @brief It implements the game structure
 *
 * @file game.c
 * @author Profesores PPROG
 * @version 2
 * @date 27-01-2025
 * @copyright GNU Public License
 */

#include "game.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Game
 *
 * This struct stores all the global state of the game.
 */
struct _Game {
  Player    *player;
  Object    *objects[MAX_OBJECTS];
  Space     *spaces[MAX_SPACES];
  Character *characters[MAX_CHARACTERS];
  Command   *last_cmd;
  LOG       *logs[MAX_LOGS];


  int       n_spaces;
  int       n_objects;
  int       n_characters;
  int       n_LOG;
  Bool      finished;
  Status    last_cmd_status;
};

struct _LOG
{
  Command* command;
  Status result;
};


/* ========== Create / Destroy ========== */

Game *game_create(){
  int i;
  Game *game = NULL;

  game = (Game *)malloc(sizeof(Game));
  if(!game) return NULL;

  for(i = 0; i < MAX_SPACES; i++)     game->spaces[i] = NULL;
  for(i = 0; i < MAX_OBJECTS; i++)     game->objects[i] = NULL;
  for(i = 0; i < MAX_CHARACTERS; i++)  game->characters[i] = NULL;

  game->n_spaces = 0;
  game->n_objects = 0;
  game->n_characters = 0;
  game->finished = FALSE;
  game->last_cmd_status = OK;
  game->n_LOG = 0;
  game->logs[0] = NULL;

  game->player = player_create();
  if(!game->player){
    free(game);
    return NULL;
  }

  game->last_cmd = command_create();
  if(!game->last_cmd){
    player_destroy(game->player);
    free(game);
    return NULL;
  }

  return game;
}

Status game_destroy(Game *game){
  int i;
  if(!game) return ERROR;

  for(i = 0; i < game->n_spaces; i++)
    space_destroy(game->spaces[i]);

  for(i = 0; i < game->n_objects; i++)
    obj_destroy(game->objects[i]);

  for(i = 0; i < game->n_characters; i++)
    character_destroy(game->characters[i]);

  player_destroy(game->player);
  command_destroy(game->last_cmd);

  free(game);
  return OK;
}


/* ========== Access: Enfoque B (expose pointers) ========== */

Player *game_get_player(Game *game){
  if(!game) return NULL;
  return game->player;
}

Command *game_get_last_command(Game *game){
  if(!game) return NULL;
  return game->last_cmd;
}

Space *game_get_space(Game *game, Id id){
  int i;

  if(!game || id == NO_ID) return NULL;

  for(i = 0; i < game->n_spaces; i++){
    if(space_get_id(game->spaces[i]) == id)
      return game->spaces[i];
  }

  return NULL;
}


/* ========== Add elements (used by game_reader) ========== */

Status game_add_space(Game *game, Space *space){
  if(!game || !space || game->n_spaces >= MAX_SPACES) return ERROR;

  game->spaces[game->n_spaces] = space;
  game->n_spaces++;
  return OK;
}

Status game_add_object(Game *game, Object *obj){
  if(!game || !obj || game->n_objects >= MAX_OBJECTS) return ERROR;

  game->objects[game->n_objects] = obj;
  game->n_objects++;
  return OK;
}

Status game_add_character(Game *game, Character *character){
  if(!game || !character || game->n_characters >= MAX_CHARACTERS) return ERROR;

  game->characters[game->n_characters] = character;
  game->n_characters++;
  return OK;
}


/* ========== Search: Objects ========== */

Object *game_get_object_by_id(Game *game, Id id){
  int i;

  if(!game || id == NO_ID) return NULL;

  for(i = 0; i < game->n_objects; i++){
    if(obj_get_id(game->objects[i]) == id)
      return game->objects[i];
  }

  return NULL;
}

Object *game_get_object_by_name(Game *game, char *name){
  int i;

  if(!game || !name) return NULL;

  for(i = 0; i < game->n_objects; i++){
    if(obj_has_name(game->objects[i], name) == TRUE)
      return game->objects[i];
  }

  return NULL;
}

/**
 * @brief Given an object id, finds in which space it is located
 *
 * Iterates through all spaces checking if any contains the given object id.
 *
 * @param game a pointer to game
 * @param obj_id the id of the object to locate
 * @return the Id of the space containing the object, or NO_ID if not found
 */
Id game_get_object_location(Game *game, Id obj_id){
  int i;
  if(!game || obj_id == NO_ID) return NO_ID;

  for(i = 0; i < game->n_spaces; i++){
    if(space_contains_object(game->spaces[i], obj_id) == TRUE)
      return space_get_id(game->spaces[i]);
  }

  /* Object might be in player's inventory, not in any space */
  return NO_ID;
}


/* ========== Search: Characters ========== */

Character *game_get_character_by_id(Game *game, Id id){
  int i;

  if(!game || id == NO_ID) return NULL;

  for(i = 0; i < game->n_characters; i++){
    if(character_get_id(game->characters[i]) == id)
      return game->characters[i];
  }

  return NULL;
}

Character *game_get_character_by_name(Game *game, char *name){
  int i;
  if(!game || !name) return NULL;

  for(i = 0; i < game->n_characters; i++){
    if(character_has_name(game->characters[i], name) == TRUE)
      return game->characters[i];
  }

  return NULL;
}

/**
 * @brief Given a character id, finds in which space it is located
 *
 * Iterates through all spaces checking if any contains the given character id.
 *
 * @param game a pointer to game
 * @param char_id the id of the character to locate
 * @return the Id of the space containing the character, or NO_ID if not found
 */
Id game_get_character_location(Game *game, Id char_id){
  int i;

  if(!game || char_id == NO_ID) return NO_ID;

  for(i = 0; i < game->n_spaces; i++){
    if(space_contains_character(game->spaces[i], char_id) == TRUE)
      return space_get_id(game->spaces[i]);
  }

  return NO_ID;
}


/* ========== Access by index (for graphic_engine iteration) ========== */

Object *game_get_object_at(Game *game, int position){
  if(!game || position < 0 || position >= game->n_objects) return NULL;
  return game->objects[position];
}

Character *game_get_character_at(Game *game, int position){
  if(!game || position < 0 || position >= game->n_characters) return NULL;
  return game->characters[position];
}

Id game_get_space_id_at(Game *game, int position){
  if(!game || position < 0 || position >= game->n_spaces) return NO_ID;
  return space_get_id(game->spaces[position]);
}

int game_get_n_spaces(Game *game){
  if(!game) return -1;
  return game->n_spaces;
}

int game_get_n_objects(Game *game){
  if(!game) return -1;
  return game->n_objects;
}

int game_get_n_characters(Game *game){
  if(!game) return -1;
  return game->n_characters;
}


/* ========== Game state ========== */

Status game_set_finished(Game *game, Bool finished){
  if(!game) return ERROR;
  game->finished = finished;
  return OK;
}

Bool game_get_finished(Game *game){
  if(!game) return TRUE;
  return game->finished;
}

Status game_set_last_command(Game *game, Command *command){
  if(!game || !command || game->n_LOG >= MAX_LOGS) return ERROR;
  game->last_cmd = command;
  game->n_LOG++;
  game->logs[game->n_LOG]->command = command;
  return OK;
}

Status game_set_last_cmd_status(Game *game, Status status){
  if(!game) return ERROR;
  game->last_cmd_status = status;
  game->logs[game->n_LOG]->result = status;
  return OK;
}

Status game_get_last_cmd_status(Game *game){
  if(!game) return ERROR;
  return game->last_cmd_status;
}

LOG *game_get_log(Game* game, int n)
{
  if(!game || !n || n < 0 || n > game->n_LOG) return NULL;
  return game->logs[n - 1];
}

/* ========== Print (debugging) ========== */

void game_print(Game *game){
  int i;

  if(!game) return;

  printf("\n============ GAME STATE ============\n");

  /* Player info */
  printf("=> Player:\n");
  player_print(game->player);

  /* Spaces */
  printf("\n=> Spaces (%d):\n", game->n_spaces);
  for(i = 0; i < game->n_spaces; i++){
    space_print(game->spaces[i]);
  }

  /* Objects with locations */
  printf("\n=> Objects (%d):\n", game->n_objects);
  for(i = 0; i < game->n_objects; i++){
    Id loc = game_get_object_location(game, obj_get_id(game->objects[i]));
    obj_print(game->objects[i]);
    if(loc != NO_ID)
      printf("   Located in space: %ld\n", loc);
    else
      printf("   In player inventory\n");
  }

  /* Characters with locations */
  printf("\n=> Characters (%d):\n", game->n_characters);
  for(i = 0; i < game->n_characters; i++){
    Id loc = game_get_character_location(game, character_get_id(game->characters[i]));
    character_print(game->characters[i]);
    if(loc != NO_ID)
      printf("   Located in space: %ld\n", loc);
  }

  printf("\n=> Finished: %s\n", game->finished == TRUE ? "YES" : "NO");
  printf("====================================\n");
}