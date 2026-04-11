/**
 * @brief It implements the player struct
 *
 * @file player.c
 * @author Violeta y Rafael
 * @version 3
 * @date 08-04-2026
 * @copyright GNU Public License
 */

#include "player.h"
#include "inventory.h"

/**
 * @brief player
 *
 * This struct stores all the information of a player.
 */
struct _Player {
  Entity    *e_player;   /*!< Entity base (id, name, gdesc, message, health, attack) */
  Inventory *backpack;   /*!< Inventory of object IDs the player carries */
  Id         location;   /*!< Id of the space where the player is */
};

/* ========== Create / Destroy ========== */

Player *player_create() {
  Player *newPlayer = (Player *)malloc(sizeof(Player));
  if (!newPlayer) return NULL;

  newPlayer->location = NO_ID;

  newPlayer->e_player = entity_create();
  if (!newPlayer->e_player) {
    free(newPlayer);
    return NULL;
  }

  newPlayer->backpack = inventory_create();
  if (!newPlayer->backpack) {
    entity_destroy(newPlayer->e_player);
    free(newPlayer);
    return NULL;
  }

  return newPlayer;
}

Status player_destroy(Player *player) {
  if (!player) return ERROR;

  entity_destroy(player->e_player);
  inventory_destroy(player->backpack);
  free(player);

  return OK;
}

/* ========== Id ========== */

Status player_set_id(Player *player, Id new_id) {
  if (!player || !(player->e_player)) return ERROR;
  return entity_set_id(player->e_player, new_id);
}

Id player_get_id(Player *player) {
  if (!player || !(player->e_player)) return NO_ID;
  return entity_get_id(player->e_player);
}

/* ========== Name ========== */

Status player_set_name(Player *player, char *name) {
  if (!player || !name) return ERROR;
  return entity_set_name(player->e_player, name);
}

char *player_get_name(Player *player) {
  if (!player) return NULL;
  return entity_get_name(player->e_player);
}

Bool player_has_name(Player *player, char *name) {
  if (!player || !name) return FALSE;
  return entity_has_name(player->e_player, name);
}

/* ========== Health ========== */

Status player_set_health(Player *player, int life) {
  if (!player) return ERROR;
  return entity_set_health(player->e_player, life);
}

int player_get_health(Player *player) {
  if (!player || !player->e_player) return ERROR_LIFE;
  return entity_get_health(player->e_player);
}

/* ========== Attack ========== */

Status player_set_attack(Player *player, int value) {
  if (!player) return ERROR;
  return entity_set_attack(player->e_player, value);
}

int player_get_attack(Player *player) {
  if (!player) return ERROR_ATTACK;
  return entity_get_attack(player->e_player);
}

/* ========== Objects (Inventory) ========== */

Status player_add_object(Player *player, Id new_obj) {
  if (!player) return ERROR;
  return inventory_add(player->backpack, new_obj);
}

Bool player_contains_object(Player *player, Id ref_obj) {
  if (!player) return FALSE;
  return inventory_contains_object(player->backpack, ref_obj);
}

Status player_delete_object(Player *player, Id trash_obj) {
  if (!player) return ERROR;
  return inventory_delete_obj(player->backpack, trash_obj);
}

int player_get_n_objects(Player *player) {
  if (!player) return ERROR_MAIN;
  return inventory_get_n_ids(player->backpack);
}

Status player_set_max_objects(Player *player, int max) {
  if (!player) return ERROR;
  return inventory_set_max_objs(player->backpack, max);
}

/* ========== Location ========== */

Status player_set_location(Player *player, Id new_location) {
  if (!player || new_location == NO_ID) return ERROR;
  player->location = new_location;
  return OK;
}

Id player_get_location(Player *player) {
  if (!player) return NO_ID;
  return player->location;
}

/* ========== Message ========== */

Status player_set_message(Player *player, char *messg) {
  if (!player || !messg) return ERROR;
  return entity_set_message(player->e_player, messg);
}

char *player_get_message(Player *player) {
  if (!player) return NULL;
  return entity_get_message(player->e_player);
}

/* ========== Graphic description ========== */

Status player_set_gdesc(Player *player, char *desc) {
  if (!player || !desc) return ERROR;
  return entity_set_gdesc(player->e_player, desc);
}

char *player_get_gdesc(Player *player) {
  if (!player) return NULL;
  return entity_get_gdesc(player->e_player);
}

/* ========== Print ========== */

Status player_print(Player *player) {
  Id    player_id;
  Id    player_location;
  int   player_n_objects;
  int   player_health;
  int   player_attack;
  char *player_name    = NULL;
  char *player_gdesc   = NULL;
  char *player_message = NULL;

  if (!player) return ERROR;

  player_id        = player_get_id(player);
  player_location  = player_get_location(player);
  player_n_objects = player_get_n_objects(player);
  player_health    = player_get_health(player);
  player_attack    = player_get_attack(player);
  player_name      = player_get_name(player);
  player_gdesc     = player_get_gdesc(player);
  player_message   = player_get_message(player);

  fprintf(stdout, "--> Player (Id: %ld; Name: %s; Gdesc: %s; Location: %ld; "
                  "Health: %d; Attack: %d; Objects: %d; Message: %s)\n",
          player_id,
          player_name    ? player_name    : "NULL",
          player_gdesc   ? player_gdesc   : "NULL",
          player_location,
          player_health,
          player_attack,
          player_n_objects,
          player_message ? player_message : "NULL");

  if (player_name)    free(player_name);
  if (player_gdesc)   free(player_gdesc);
  if (player_message) free(player_message);

  return OK;
}