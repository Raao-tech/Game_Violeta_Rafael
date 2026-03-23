/**
 * @brief It implements the player struct
 *
 * @file player.c
 * @author Violeta, Rafael and Salvador
 * @version 1.0
 * @date 23-4-2026
 * @copyright GNU Public License
 */


#include "player.h"
#include "inventory.h"



/**
 * @brief player
 *
 * This struct stores all the information of a player.
 */
struct _Player{
  Entity  *e_player;
 Inventory *backpack;
  Id      location;
};

/* player */
Player    *player_create(){
  /* Creamos espacio  para el player */
  Player *newPlayer = (Player *) malloc(sizeof(Player));
  if(!newPlayer) return NULL;

  /* locacion indefinidad */
  newPlayer->location = NO_ID;


  /* Creamos el entity del player*/
  newPlayer->e_player = entity_create();
  if(!newPlayer->e_player) {
    free(newPlayer);
    return  NULL;
  }

  /* Creamos el set de ids de objects */
  newPlayer->backpack = inventory_create();
  if(!newPlayer->backpack){
    entity_destroy(newPlayer->e_player);
    free(newPlayer);
    return  NULL;
  }
  //inicializar el resto de la estructura player//

  return newPlayer;
}
Status    player_destroy(Player *player){
  if (!player)return ERROR;

  entity_destroy(player->e_player);
  inventory_destroy(player->backpack);
  free(player);

  return OK;
}


/* id */
Status    player_set_id(Player  *player, Id new_id){
  if(!player || !(player->e_player)) return ERROR;

  return entity_set_id(player->e_player, new_id);
}
Id        player_get_id(Player  *player){
  if(!player || !(player->e_player)) return NO_ID;

  return entity_get_id(player->e_player);
}


/* name */
Status      player_set_name(Player *player, char *name){
  if (!player || !name) return ERROR;
  return entity_set_name(player->e_player, name);
}
char        *player_get_name(Player *player){
  if (!player) return NULL;
  return entity_get_name(player->e_player);
}


/* health */
Status    player_set_health(Player *player, int life){
    if(!player) return ERROR;
    return  entity_set_health(player->e_player, life);
}
int       player_get_health(Player *player){
  if(!player || !player->e_player) return ERROR_LIFE;

  return  entity_get_health(player->e_player);
}


/* attack */
Status    player_set_attack(Player *player, int value){
  if(!player) return ERROR;
  return entity_set_attack(player->e_player, value);
}
int       player_get_attack(Player *player){
  if(!player) return ERROR_ATTACK;
  return entity_get_attack(player->e_player);
}

/* objects*/
Status    player_add_object(Player *player, Id new_obj){
  if(!player) return ERROR;
  return inventory_add(player->backpack,new_obj);
}
Bool      player_contains_object(Player *player, Id ref_obj){
  if(!player) return FALSE;
  return  inventory_contains_id(player->backpack, ref_obj);
}
Status    player_delete_object(Player *player, Id trash_obj){
  if(!player) return ERROR;
  return  inventory_delete_id(player->backpack, trash_obj);
}
int       player_get_n_objects(Player *player){
  if(!player) return ERROR_MAIN;
  return  inventory_get_n_ids(player->backpack);
}


/* Location*/
Status    player_set_location(Player *player, Id new_location){
  if(!player || new_location == NO_ID) return ERROR;
  player->location = new_location;
  return  OK;
}
Id        player_get_location(Player *player){
  if(!player) return NO_ID;
  return  player->location;
}



/* message: esto lo implementaremos en la 3era iteracion */
Status    player_set_message(Player *player, char* messg){
  if(!player || !messg) return ERROR;
  return entity_set_message(player->e_player, messg);
}
char      *player_get_message(Player *player){
  if(!player) return NULL;
  return  entity_get_message(player->e_player);
}
   


/* gDescription */
Status    player_set_gdesc(Player *player, char* desc){
  if(!player || !desc) return ERROR;
  return entity_set_gdesc(player->e_player, desc);
}
char      *player_get_gdesc(Player *player){
  if(!player) return NULL;
  return  entity_get_gdesc(player->e_player);
}


/*================= PRINT =================== */
Status player_print(Player *player){
  Id    player_id;
  Id    player_location;
  int   player_n_objects;
  int   player_health;
  int   player_attack;
  char* player_name = NULL;
  char* player_gdesc = NULL;
  char* player_message = NULL;
  if (!player) return ERROR;


  player_id =  player_get_id(player);
  player_location = player_get_location(player);
  player_n_objects = player_get_n_objects(player);
  player_health = player_get_health(player);
  player_attack = player_get_attack(player);
  player_name = player_get_name(player);
  player_gdesc = player_get_gdesc(player);
  player_message = player_get_message(player);

  
  if(player_name == NULL){
    free(player_gdesc);
    free(player_message);
    return ERROR;
  }
  if(player_gdesc == NULL){
    free(player_name);
    free(player_message);
    return ERROR;
  }
  if(player_message == NULL){
    free(player_name);
    free(player_gdesc);
    return ERROR;
  }
  if(player_id == NO_ID){
    free(player_name);
    free(player_gdesc);
    free(player_message);
    return ERROR;
  }
  if(player_location == NO_ID){
    free(player_name);
    free(player_gdesc);
    free(player_message);
    return ERROR;
  }
  if(player_n_objects == ERROR_MAIN){
    free(player_name);
    free(player_gdesc);
    free(player_message);
    return ERROR;
  }
  if(player_health == ERROR_MAIN){
    free(player_name);
    free(player_gdesc);
    free(player_message);
    return ERROR;
  }
  if(player_attack == ERROR_MAIN){
    free(player_name);
    free(player_gdesc);
    free(player_message);
    return ERROR;
  }


  fprintf(stdout, "\n====================\n");
  fprintf(stdout, "PLAYER:  %s", player_gdesc);
  fprintf(stdout, "\n========DATA========\n");
  fprintf(stdout, "|| NAME:  %s\n", player_name);
  fprintf(stdout, "|| ID:    %ld\n", player_id);
  fprintf(stdout, "|| HEALHT:  %d\n", player_health);
  fprintf(stdout, "|| ATTACK:   %d\n", player_attack);
  fprintf(stdout, "|| LOCATION:   %ld\n", player_location);
  fprintf(stdout, "|| MESSAGE_DETER:   %s\n", player_message);
  fprintf(stdout, "|| NUMBER OF OBJECTS:   %d\n", player_n_objects);
  fprintf(stdout, "=====================\n");

  free(player_name);
  free(player_gdesc);
  free(player_message);
  
  return OK;
}


