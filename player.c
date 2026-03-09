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


/**
 * @brief player
 *
 * This struct stores all the information of a player.
 */
struct _Player
{
  Entity *e_player;
  Set *objects;
};

Player *player_create()
{
  Player *newPlayer = NULL;

  if(!character_create(newPlayer->e_player)){
    return NULL;
  }

  //inicializar el resto de la estructura player//

  return newPlayer;
}


Status player_destroy(Player *player)
{
  if (!player)
  {
    return ERROR;
  }

  entity_destroy(player->e_player);
  set_destroy(player->objects);
  free(player);

  return OK;
}

Status player_set_name(Player *player, char *name)
{
  if (!player || !name)
  {
    return ERROR;
  }

  if (entity_set_name(player->e_player, name) == ERROR)
  {
    return ERROR;
  }
  
  return OK;
}

char *player_get_name(Player *player)
{
  char *name=NULL;

  if (!player)
  {
    return NULL;
  }

  name=entity_get_name(player->e_player);

  return name;
}


/*
Status player_set_obj(Player *player, Id id_obj)
{
  if (player == NULL)
  {
    return ERROR;
  }

  player->id_object_take = id_obj;
  return OK;
}



Id player_get_obj(Player *player)
{
  if (player == NULL)
  {
    return NO_ID;
  }
  return player->id_object_take;
}

Status player_print(Player *player)
{

  if (!player || player->id == NO_ID)
  {
    return ERROR;
  }


  fprintf(stdout, "--> Player %ld: %s)\n", player->id, player->name);

  return OK;
}

*/