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


/* Functions to sets properties of the game */
Status game_create_player(Game *game, Id id_player){
  if (game == NULL){
    return ERROR;
  }

  game->player = player_create(id_player);

  return  OK;
  
}
Status game_create_object(Game *game, Id id_obj, char* name){
  if (game == NULL){
    return ERROR;
  }

  game->objects = obj_create(id_obj, name);

  return  OK;
  
}

Status game_set_object_location(Game *game, Id space_id) {
  int i;
  Space *dest = NULL;
  Id oid = NO_ID;

  if (!game || space_id == NO_ID || !game->objects) {
    return ERROR;
  }

  dest = game_get_space(game, space_id);
  if (!dest) {
    return ERROR;
  }

  oid = obj_get_id(game->objects);
  if (oid == NO_ID) {
    return ERROR;
  }

  /* Remove the object of anysite  */
  for (i = 0; i < game->n_spaces; i++) {
    if (space_get_object(game->spaces[i]) == oid) {
      space_set_object(game->spaces[i], NO_ID);
    }
  }

  /* object on dest space*/
  return space_set_object(dest, oid);
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
Id game_get_player_location(Game *game) { 
  if(!game){
    return  NO_ID;
  } 

  return  player_get_location(game->player);
}
Status game_player_take(Game *game, Id id_obj){

  Space* space_on = NULL;
  Id  location_player = NO_ID;

  if(game == NULL || id_obj == NO_ID){
    return ERROR;
  }

  location_player = player_get_location(game->player);

  if(location_player == NO_ID){
    return ERROR;
  }

  space_on = game_get_space(game, location_player);

  if(space_on == NULL){
    return ERROR;
  }

  space_set_object(space_on, NO_ID);
  
  return player_set_obj(game->player,id_obj);
}
Status game_player_drop(Game *game){

  Id  space_id = NO_ID;
  Id  obj_on_id = NO_ID;
  Space*  space_on = NULL;

  if(game == NULL){
    return ERROR;
  }

  obj_on_id = player_get_obj(game->player);
  space_id = player_get_location(game->player);

  if(obj_on_id == NO_ID || space_id == NO_ID){
    return ERROR;
  }

  if( (space_on = game_get_space(game, space_id)) == NULL){
    return ERROR;
  }
  player_set_obj(game->player, NO_ID);

  return  space_set_object(space_on, obj_on_id);
}
Id game_get_object_location(Game *game) { 
  int i;
  if(game == NULL ){
    return NO_ID;
  }
  for (i = 0; i < game->n_spaces; i++){
    if(space_get_object(game->spaces[i]) == obj_get_id(game->objects)){
      return space_get_id(game->spaces[i]);
    }
  }

  return NO_ID; 
}
Command* game_get_last_command(Game *game) { 
  if(game == NULL){
    return NULL;
  }

  return game->last_cmd; 
}
Bool game_get_finished(Game *game) { 
  if (game == NULL){
    return TRUE;
  }
  
  return game->finished; 
}


/* Functions to create the game */
Status game_create(Game *game) {
  int i;

  if(game == NULL ){
    return  ERROR;
  }

  for (i = 0; i < MAX_SPACES; i++) {
    game->spaces[i] = NULL;
  }

  game->n_spaces = 0;
  game->player = NULL;
  game->objects = NULL;
  game->last_cmd = command_create();
  game->finished = FALSE;

  return OK;
}
Status game_destroy(Game *game) {
  int i = 0;
  if(game == NULL)  {
    return ERROR;
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

void game_print(Game *game) {
  int i = 0;

  printf("\n\n-------------\n\n");

  printf("=> Spaces: \n");
  for (i = 0; i < game->n_spaces; i++) {
    space_print(game->spaces[i]);
  }

  printf("=> Object location: %d\n", (int)space_get_object(game->spaces[0]));
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
