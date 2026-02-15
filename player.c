/**
 * @brief It implements the player struct
 *
 * @file player.c
 * @author Violeta y Rafael
 * @version 0
 * @date 04-02-2025
 * @copyright GNU Public License
 */

#include "player.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief player
 *
 * This struct stores all the information of a player.
 */
struct _Player {
  Id id;                    /*!< Id number of the object, it must be unique */
  char name[WORD_SIZE + 1]; /*!< Name of the object */
  Id  id_space_location;
  Id  id_object_take;

};


Player* player_create(Id id) {
  Player* newPlayer = NULL;

  /* Error control */
  if (id == NO_ID) {
    return NULL;
  }

  newPlayer = (Player*)malloc(sizeof(Player));
  if (newPlayer == NULL) {
    return NULL;
  }

  /* Initialization of an empty player*/
  newPlayer->id = id;
  newPlayer->id_space_location = NO_ID;
  newPlayer->id_object_take = NO_ID;
  newPlayer->name[0] = '\0';

  return newPlayer;
}

/* functions to sets properties and assets of the player */
Status player_destroy (Player* player) {
  if (!player) {
    return ERROR;
  }

  free(player);
  return OK;
}

Status player_set_obj (Player* player,  Id id_obj) {
  if(player == NULL || id_obj == NO_ID ){
    return ERROR;
  }

  player->id_object_take = id_obj;
  return OK;
}

Status player_set_location (Player* player, Id id_space){
  if(player == NULL || id_space == NO_ID ){
    return ERROR;
  }

  player->id_space_location = id_space;
  return OK;
}

Status player_set_name (Player* player, char* name) {
  if (!player || !name) {
    return ERROR;
  }

  if (!strcpy(player->name, name)) {
    return ERROR;
  }
  return OK;
}


/* functions to know properties and assets of the player */
char* player_get_name(Player* player) {
  if (!player) {
    return NULL;
  }
  return player->name;
}

Id player_get_location(Player* player) {
  if(player == NULL){
    return  NULL;
  }
  return player->id_space_location;
}

Id player_get_obj(Player* player) {
  if(player == NULL){
    return  NULL;
  }
  return player->id_object_take;
}

Status player_print(Player* player) {
  /* Error Control */
  if (!player || player->id == NO_ID) {
    return ERROR;
  }

  /* 1. Print the id and the name of the player */
  fprintf(stdout, "--> Player %ld: %s)\n", player->id, player->name);

  return OK;
}