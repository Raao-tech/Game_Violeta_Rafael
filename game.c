/**
 * @brief It implements the game structure
 *
 * @file game.c
 * @author Profesores PROG, Violeta, Rafael and Salvador
 * @version 3.0
 * @date 23-4-2026
 * @copyright GNU Public License
 */

#include "game.h"
#include "links.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Game
 *
 * This struct stores all the global state of the game.
 */
struct _Game
{
  Player *players[MAX_PLAYERS];
  Object *objects[MAX_OBJECTS];
  Space *spaces[MAX_SPACES];
  Character *characters[MAX_CHARACTERS];
  Command *last_cmd;
  Links *links[MAX_LINKS];

  int n_players;
  int n_spaces;
  int n_objects;
  int n_characters;
  int n_links;
  Bool finished;
  Status last_cmd_status;
};

/* ========== Create / Destroy ========== */

Game *game_create()
{
  int i;
  Game *game = NULL;

  game = (Game *)malloc(sizeof(Game));
  if (!game)
    return NULL;

  for (i = 0; i < MAX_SPACES; i++)
    game->players[i] = NULL;
  for (i = 0; i < MAX_SPACES; i++)
    game->spaces[i] = NULL;
  for (i = 0; i < MAX_OBJECTS; i++)
    game->objects[i] = NULL;
  for (i = 0; i < MAX_CHARACTERS; i++)
    game->characters[i] = NULL;
  for (i = 0; i < MAX_LINKS; i++)
    game->links[i] = NULL;

  game->n_spaces = 0;
  game->n_objects = 0;
  game->n_characters = 0;
  game->n_links = 0;
  game->finished = FALSE;
  game->last_cmd_status = OK;

  game->last_cmd = command_create();
  if (!game->last_cmd)
  {
    free(game);
    return NULL;
  }

  return game;
}

Status game_destroy(Game *game)
{
  int i;
  if (!game)
    return ERROR;

  for (i = 0; i < game->n_spaces; i++)
    space_destroy(game->spaces[i]);

  for (i = 0; i < game->n_objects; i++)
    obj_destroy(game->objects[i]);

  for (i = 0; i < game->n_characters; i++)
    character_destroy(game->characters[i]);

  for (i = 0; i < game->n_players; i++)
    player_destroy(game->players[i]);

  command_destroy(game->last_cmd);

  free(game);
  return OK;
}

/* ========== Access: Enfoque B (expose pointers) ========== */

Player **game_get_players(Game *game)
{
  if (!game)
    return NULL;

  return game->players;
}

Command *game_get_last_command(Game *game)
{
  if (!game)
    return NULL;
  return game->last_cmd;
}

Space *game_get_space(Game *game, Id id)
{
  int i;

  if (!game || id == NO_ID)
    return NULL;

  for (i = 0; i < game->n_spaces; i++)
  {
    if (space_get_id(game->spaces[i]) == id)
      return game->spaces[i];
  }

  return NULL;
}

Id game_get_connection(Game *game, Id space, Direction dir)
{
  int i;
  if (!game || space == NO_ID || dir == U)
  {
    return NO_ID;
  }
  for (i = 0; i < game->n_links; i++)
  {
    if (link_get_origin_id(game->links[i]) == space && link_get_direction(game->links[i]) == dir)
    {
      return link_get_destiny_id(game->links[i]);
    }
    else if (link_get_destiny_id(game->links[i]) == space && link_get_opposite_direction(game->links[i]) == dir)
    {
      return link_get_origin_id(game->links[i]);
    }
  }
  return NO_ID;
}

Bool game_connection_is_open(Game *game, Id space, Direction dir)
{
  int i;
  if (!game || space == NO_ID || dir == U)
  {
    return FALSE;
  }
  for (i = 0; i < game->n_links; i++)
  {
    if (link_get_origin_id(game->links[i]) == space && link_get_direction(game->links[i]) == dir)
    {
      return link_get_open_orig_to_dest(game->links[i]);
    }
    else if (link_get_destiny_id(game->links[i]) == space && link_get_opposite_direction(game->links[i]) == dir)
    {
      return link_get_open_dest_to_orig(game->links[i]);
    }
  }
  return FALSE;
}

/* ========== Add elements (used by game_reader) ========== */

Status game_add_space(Game *game, Space *space)
{
  if (!game || !space || game->n_spaces >= MAX_SPACES)
    return ERROR;

  game->spaces[game->n_spaces] = space;
  game->n_spaces++;
  return OK;
}

Status game_add_object(Game *game, Object *obj)
{
  if (!game || !obj || game->n_objects >= MAX_OBJECTS)
    return ERROR;

  game->objects[game->n_objects] = obj;
  game->n_objects++;
  return OK;
}

Status game_add_character(Game *game, Character *character)
{
  if (!game || !character || game->n_characters >= MAX_CHARACTERS)
    return ERROR;

  game->characters[game->n_characters] = character;
  game->n_characters++;
  return OK;
}

Status game_add_player(Game *game, Player *player)
{
  if (!game || !player || game->n_players >= MAX_PLAYERS)
    return ERROR;

  game->players[game->n_players] = player;
  game->n_players++;
  return OK;
}

/* ========== Search: Objects ========== */

Object *game_get_object_by_id(Game *game, Id id)
{
  int i;

  if (!game || id == NO_ID)
    return NULL;

  for (i = 0; i < game->n_objects; i++)
  {
    if (obj_get_id(game->objects[i]) == id)
      return game->objects[i];
  }

  return NULL;
}

Object *game_get_object_by_name(Game *game, char *name)
{
  int i;

  if (!game || !name)
    return NULL;

  for (i = 0; i < game->n_objects; i++)
  {
    if (obj_has_name(game->objects[i], name) == TRUE)
      return game->objects[i];
  }

  return NULL;
}

Id game_get_object_location(Game *game, Id obj_id)
{
  int i;
  if (!game || obj_id == NO_ID)
    return NO_ID;

  for (i = 0; i < game->n_spaces; i++)
  {
    if (space_contains_object(game->spaces[i], obj_id) == TRUE)
      return space_get_id(game->spaces[i]);
  }

  /* Object might be in player's inventory, not in any space */
  return NO_ID;
}

/* ========== Search: Characters ========== */

Character *game_get_character_by_id(Game *game, Id id)
{
  int i;

  if (!game || id == NO_ID)
    return NULL;

  for (i = 0; i < game->n_characters; i++)
  {
    if (character_get_id(game->characters[i]) == id)
      return game->characters[i];
  }

  return NULL;
}

Character *game_get_character_by_name(Game *game, char *name)
{
  int i;
  if (!game || !name)
    return NULL;

  for (i = 0; i < game->n_characters; i++)
  {
    if (character_has_name(game->characters[i], name) == TRUE)
      return game->characters[i];
  }

  return NULL;
}

Id game_get_character_location(Game *game, Id char_id)
{
  int i;

  if (!game || char_id == NO_ID)
    return NO_ID;

  for (i = 0; i < game->n_spaces; i++)
  {
    if (space_contains_character(game->spaces[i], char_id) == TRUE)
      return space_get_id(game->spaces[i]);
  }

  return NO_ID;
}

/* ========== Search: Players ========== */

Player *game_get_player_by_id(Game *game, Id id)
{
  int i;

  if (!game || id == NO_ID)
    return NULL;

  for (i = 0; i < game->n_players; i++)
  {
    if (player_get_id(game->players[i]) == id)
      return game->players[i];
  }

  return NULL;
}

Player *game_get_player_by_name(Game *game, char *name)
{
  int i;
  if (!game || !name)
    return NULL;

  for (i = 0; i < game->n_players; i++)
  {
    if (player_has_name(game->players[i], name) == TRUE)
      return game->players[i];
  }

  return NULL;
}

Id game_get_player_location(Game *game, Id player_id)
{
  int i;

  if (!game || player_id == NO_ID)
    return NO_ID;

  for (i = 0; i < game->n_players; i++)
  {
    if (player_get_id(game->players[i])==player_id)
      return player_get_location(game->players[i]);
  }

  return NO_ID;
}

/* ========== Access by index (for graphic_engine iteration) ========== */

Object *game_get_object_at(Game *game, int position)
{
  if (!game || position < 0 || position >= game->n_objects)
    return NULL;
  return game->objects[position];
}

Character *game_get_character_at(Game *game, int position)
{
  if (!game || position < 0 || position >= game->n_characters)
    return NULL;
  return game->characters[position];
}

Player *game_get_player_at(Game *game, int position)
{
  if (!game || position < 0 || position >= game->n_players)
    return NULL;
  return game->players[position];
}

Id game_get_space_id_at(Game *game, int position)
{
  if (!game || position < 0 || position >= game->n_spaces)
    return NO_ID;
  return space_get_id(game->spaces[position]);
}

int game_get_n_spaces(Game *game)
{
  if (!game)
    return -1;
  return game->n_spaces;
}

int game_get_n_objects(Game *game)
{
  if (!game)
    return -1;
  return game->n_objects;
}

int game_get_n_characters(Game *game)
{
  if (!game)
    return -1;
  return game->n_characters;
}

int game_get_n_players(Game *game)
{
  if (!game)
    return -1;
  return game->n_players;
}
/* ========== Game state ========== */

Status game_set_finished(Game *game, Bool finished)
{
  if (!game)
    return ERROR;
  game->finished = finished;
  return OK;
}

Bool game_get_finished(Game *game)
{
  if (!game)
    return TRUE;
  return game->finished;
}

Status game_set_last_command(Game *game, Command *command)
{
  if (!game || !command)
    return ERROR;
  game->last_cmd = command;
  return OK;
}

Status game_set_last_cmd_status(Game *game, Status status)
{
  if (!game)
    return ERROR;
  game->last_cmd_status = status;
  return OK;
}

Status game_get_last_cmd_status(Game *game)
{
  if (!game)
    return ERROR;
  return game->last_cmd_status;
}

/* ========== Print (debugging) ========== */

void game_print(Game *game)
{
  int i;
  Id loc;
  if (!game)
    return;

  printf("\n============ GAME STATE ============\n");

  /* Player info */
  printf("\n=> Players (%d):\n", game->n_players);
  for (i = 0; i < game->n_players; i++)
  {
     loc = game_get_player_location(game, player_get_id(game->players[i]));
    player_print(game->players[i]);
    if (loc != NO_ID)
      printf("   Located in space: %ld\n", loc);
  }

  /* Spaces */
  printf("\n=> Spaces (%d):\n", game->n_spaces);
  for (i = 0; i < game->n_spaces; i++)
  {
    space_print(game->spaces[i]);
  }

  /* Objects with locations */
  printf("\n=> Objects (%d):\n", game->n_objects);
  for (i = 0; i < game->n_objects; i++)
  {
     loc = game_get_object_location(game, obj_get_id(game->objects[i]));
    obj_print(game->objects[i]);
    if (loc != NO_ID)
      printf("   Located in space: %ld\n", loc);
    else
      printf("   In player inventory\n");
  }

  /* Characters with locations */
  printf("\n=> Characters (%d):\n", game->n_characters);
  for (i = 0; i < game->n_characters; i++)
  {
     loc = game_get_character_location(game, character_get_id(game->characters[i]));
    character_print(game->characters[i]);
    if (loc != NO_ID)
      printf("   Located in space: %ld\n", loc);
  }

  printf("\n=> Finished: %s\n", game->finished == TRUE ? "YES" : "NO");
  printf("====================================\n");
}