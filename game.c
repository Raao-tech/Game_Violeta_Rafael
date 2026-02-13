/**
 * @brief It implements the game structure
 *
 * @file game.c
 * @author Profesores PPROG
 * @version 0
 * @date 27-01-2025
 * @copyright GNU Public License
 */

#include "game.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/**
   Game interface implementation
*/

Status game_create(Game *game) {
  int i;

  if(game == NULL ){
    return  ERROR;
  }

  for (i = 0; i < MAX_SPACES; i++) {
    game->spaces[i] = NULL;
  }

  game->n_spaces = 0;
  game->player = player_create(NO_ID);
  game->objects = obj_create(NO_ID, NULL);
  game->last_cmd = command_create();
  game->finished = FALSE;

  return OK;
}
Status game_destroy(Game *game) {
  int i = 0;
  if(game == NULL)  {
    return NULL;
  }

  for (i = 0; i < game->n_spaces; i++) {
    space_destroy(game->spaces[i]);
  }

  if((player_destroy(game->player) == ERROR)  ||  (obj_destroy(game->objects) == ERROR)){
    return ERROR;
  }

  if(command_destroy(game->last_cmd) == ERROR){
    return ERROR;
  }

  return OK;
}


/* Functions to sets properties of the game */
Status game_set_object_location(Game *game, Id id) {
  if (id == NO_ID) {
    return ERROR;
  }

  return space_set_object(game->spaces, id);
}
Status game_set_player_location(Game *game, Id id) {
  if (id == NO_ID) {
    return ERROR;
  }

  return player_set_location(game->player, id);

}
Status game_set_last_command(Game *game, Command *command) {
  game->last_cmd = command;

  return OK;
}
Status game_set_finished(Game *game, Bool finished) {
  game->finished = finished;

  return OK;
}


/* Functions to gets properties of the game */
Space *game_get_space(Game *game, Id id) {
  int i = 0;

  if (id == NO_ID) {
    return NULL;
  }

  for (i = 0; i < game->n_spaces; i++) {
    if (id == space_get_id(game->spaces[i])) {
      return game->spaces[i];
    }
  }

  return NULL;
}
const Id game_get_player_location(Game *game) { 
  if(!game){
    return  NO_ID;
  } 

  return  player_get_location(game->player);
}
const Id game_get_object_location(Game *game) { 
  if(game == NULL){
    return NO_ID;
  }

  return space_get_object(game->spaces); 
}
Command* game_get_last_command(Game *game) { 
  if(game == NULL){
    return NO_CMD;
  }

  return game->last_cmd; 
}
Bool game_get_finished(Game *game) { 
  if (game == NULL){
    return TRUE;
  }
  
  return game->finished; 
}

void game_print(Game *game) {
  int i = 0;

  printf("\n\n-------------\n\n");

  printf("=> Spaces: \n");
  for (i = 0; i < game->n_spaces; i++) {
    space_print(game->spaces[i]);
  }

  printf("=> Object location: %d\n", (int)space_get_object(game->spaces));
  printf("=> Player location: %d\n", (int)player_get_location(game->player));
}


/*
   Implementation of private functions
*/

Status game_add_space(Game *game, Space *space) {
  if ((space == NULL) || (game->n_spaces >= MAX_SPACES)) {
    return ERROR;
  }

  game->spaces[game->n_spaces] = space;
  game->n_spaces++;

  return OK;
}

Id game_get_space_id_at(Game *game, int position) {
  if (position < 0 || position >= game->n_spaces) {
    return NO_ID;
  }

  return space_get_id(game->spaces[position]);
}
