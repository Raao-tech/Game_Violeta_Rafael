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
};


Player* player_create(Id id, char* name) {
  Player* newPlayer = NULL;

  /* Error control */
  if (id == NO_ID || !name) return NULL;

  newPlayer = (Player*)calloc(1, sizeof(Player));
  if (newPlayer == NULL) {
    return NULL;
  }

  /* Initialization of an empty player*/
  newPlayer->id = id;
  strcpy(newPlayer->name, name);

  return newPlayer;
}

Status player_destroy(Player* player) {
  if (!player) {
    return ERROR;
  }

  free(player);
  return OK;
}


Status player_set_name(Player* player, char* name) {
  if (!player || !name) {
    return ERROR;
  }

  if (!strcpy(player->name, name)) {
    return ERROR;
  }
  return OK;
}

const char* player_get_name(Player* player) {
  if (!player) {
    return NULL;
  }
  return player->name;
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