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
struct _Player{
  Entity *e_player;
  Set *objects;
};

Player *player_create(){
  /* Creamos espacio  para el player */
  Player *newPlayer = (Player *) malloc(sizeof(Player));
  if(!newPlayer) return NULL;


  /* Creamos el entity del player*/
  newPlayer->e_player = entity_create();

  if(!newPlayer->e_player) {
    free(newPlayer);
    return  NULL;
  }

  /* Creamos el set de ids de objects */
  newPlayer->objects = set_creat();
  if(!newPlayer->objects){
    entity_destroy(newPlayer->e_player);
    free(newPlayer);
    return  NULL;
  }
  //inicializar el resto de la estructura player//

  return newPlayer;
}

Status player_destroy(Player *player){
  if (!player)return ERROR;

  entity_destroy(player->e_player);
  set_destroy(player->objects);
  free(player);

  return OK;
}


Status player_set_name(Player *player, char *name){
  if (!player || !name) return ERROR;

  if (entity_set_name(player->e_player, name) == ERROR)return ERROR;
  
  return OK;
}

char *player_get_name(Player *player){
  if (!player) return NULL;

  return entity_get_name(player->e_player);
}



Status  player_set_health(Player *player, int life){
    if(!player || !player->e_player) return ERROR;

    return  entity_set_health(player->e_player, life);
}

int player_get_health(Player *player){
  if(!player || !player->e_player) return ERROR_LIFE;

  return  entity_get_health(player->e_player);
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