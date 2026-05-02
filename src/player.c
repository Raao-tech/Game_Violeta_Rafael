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
struct _Player
{
    Entity* e_player;           /*!< Entity base (id, name, gdesc, message, health, attack) */
    Inventory* backpack_items;  /*!< Inventory of object IDs the player carries */
    Inventory* backpack_numens; /*!< Inventory of nuemns IDs the player carries */
    Id zone;                    /*!< Id of the space where the player is */
    Position vision;
    Id active_numen; /*!< Id of the active numen */
    Id active_object; /*!< Id of the active numen */
    Bool loading       /*!< Flag to indicate if a player function is being called while loading the game (for now only used in set active object and numen) */
};

/* ========== Create / Destroy ========== */

Player*
player_create ()
{
    Player* newPlayer = (Player*)malloc (sizeof (Player));
    if (!newPlayer) return NULL;

    newPlayer->zone         = NO_ID;
    newPlayer->vision.pos_x = NO_POS;
    newPlayer->vision.pos_y = NO_POS;
    newPlayer->active_numen = NO_ID;
    newPlayer->active_object = NO_ID;
    newPlayer->loading = FALSE;
    newPlayer->e_player     = entity_create ();
    if (!newPlayer->e_player)
        {
            free (newPlayer);
            return NULL;
        }

    newPlayer->backpack_items = inventory_create ();
    if (!newPlayer->backpack_items)
        {
            entity_destroy (newPlayer->e_player);
            free (newPlayer);
            return NULL;
        }
    newPlayer->backpack_numens = inventory_create ();
    if (!newPlayer->backpack_numens)
        {
            inventory_destroy (newPlayer->backpack_items);
            entity_destroy (newPlayer->e_player);
            free (newPlayer);
            return NULL;
        }

    return newPlayer;
}

Status
player_destroy (Player* player)
{
    if (!player) return ERROR;

    entity_destroy (player->e_player);           /*!< Destroy pointer to entity*/
    inventory_destroy (player->backpack_items);  /*!< Destroy pointer to backpack of intems*/
    inventory_destroy (player->backpack_numens); /*!< Destroy pointer to backpack of numens*/
    free (player);

    return OK;
}

/* ========== Id ========== */

Status
player_set_id (Player* player, Id new_id)
{
    if (!player || !(player->e_player)) return ERROR;
    return entity_set_id (player->e_player, new_id);
}

Id
player_get_id (Player* player)
{
    if (!player || !(player->e_player)) return NO_ID;
    return entity_get_id (player->e_player);
}

/* ========== Name ========== */

Status
player_set_name (Player* player, char* name)
{
    if (!player || !name) return ERROR;
    return entity_set_name (player->e_player, name);
}

char*
player_get_name (Player* player)
{
    if (!player) return NULL;
    return entity_get_name (player->e_player);
}

Bool
player_has_name (Player* player, char* name)
{
    if (!player || !name) return FALSE;
    return entity_has_name (player->e_player, name);
}

/* ===========================
Stats (No usar, el tema es que necesito compilar el codigo
para probar el graphic ENgine y para ello obviaré que en game action se esta
haciendo pvp y por eso se llama a set/get de health de player
====================== */
Status
player_set_health (Player* player, int health)
{
    if (!player) return ERROR;
    return entity_set_health (player->e_player, health);
}

int
player_get_health (Player* player)
{
    if (!player) return ERROR_LIFE;
    return entity_get_health (player->e_player);
}
/* ========== Attack ========== */

/* ========== Backpack_items (Inventory) ========== */

Status
player_add_object (Player* player, Id new_obj)
{
    if (!player) return ERROR;
    return inventory_add (player->backpack_items, new_obj);
}

Status
player_delete_object (Player* player, Id trash_obj)
{
    if (!player) return ERROR;

    return inventory_delete_obj (player->backpack_items, trash_obj);
}

Bool
player_contains_object (Player* player, Id ref_obj)
{
    if (!player) return FALSE;
    return inventory_contains_object (player->backpack_items, ref_obj);
}

int
player_get_n_objects (Player* player)
{
    if (!player) return ERROR_MAIN;
    return inventory_get_n_ids (player->backpack_items);
}

Status
player_set_max_objects (Player* player, int max)
{
    if (!player) return ERROR;
    return inventory_set_max_objs (player->backpack_items, max);
}

int
player_get_max_objects (Player* player)
{
    if (!player) return ERROR;
    return inventory_get_max_objs (player->backpack_items);
}

/* =========== Backpack_Numens (Inventory) ==========*/
Status
player_add_numen (Player* player, Id new_numen)
{
    if (!player) return ERROR;
    return inventory_add (player->backpack_numens, new_numen);
}

Status
player_delete_numen (Player* player, Id trash_numen)
{
    if (!player) return ERROR;
    return inventory_delete_obj (player->backpack_numens, trash_numen);
}

Bool
player_contains_numen (Player* player, Id ref_numen)
{
    if (!player) return FALSE;
    return inventory_contains_object (player->backpack_numens, ref_numen);
}

int
player_get_n_numens (Player* player)
{
    if (!player) return ERROR_MAIN;
    return inventory_get_n_ids (player->backpack_numens);
}

Status
player_set_max_numens (Player* player, int max_numens)
{
    if (!player) return ERROR;
    return inventory_set_max_objs (player->backpack_numens, max_numens);
}

int
player_get_max_numens (Player* player)
{
    if (!player) return ERROR;
    return inventory_get_max_objs (player->backpack_numens);
}

/* ========== Numen_active ========== */
Status
player_set_active_numen (Player* player, Id numen_id)
{
    if (!player|| numen_id == NO_ID ||player_contains_numen (player, numen_id) == FALSE && player->loading == FALSE) return ERROR;
    player->active_numen = numen_id;
    return OK;
}

Id
player_get_active_numen (Player* player)
{
    if (!player) return NO_ID;
    return player->active_numen;
}

/* ========== Object_active ========== */
Id
player_get_active_object (Player* player)
{
    if (!player) return NO_ID;
    return player->active_object;   /* nuevo campo en struct _Player */
}

Status
player_set_active_object (Player* player, Id obj_id)
{
    if (!player ||player_contains_object (player, obj_id) == FALSE && player->loading == FALSE) return ERROR;
    /* Permitimos NO_ID para "deseleccionar" */
    player->active_object = obj_id;
    return OK;
}

/* ========== Zone ========== */

Status
player_set_zone (Player* player, Id new_zone)
{
    if (!player || new_zone == NO_ID) return ERROR;
    player->zone = new_zone;
    return OK;
}

Id
player_get_zone (Player* player)
{
    if (!player) return NO_ID;
    return player->zone;
}

/* ========== Message ========== */

Status
player_set_message (Player* player, char* messg)
{
    if (!player || !messg) return ERROR;
    return entity_set_message (player->e_player, messg);
}

char*
player_get_message (Player* player)
{
    if (!player) return NULL;
    return entity_get_message (player->e_player);
}

/* ========== Graphic description ========== */

Status
player_set_gdesc (Player* player, char* desc)
{
    if (!player || !desc) return ERROR;
    return entity_set_gdesc (player->e_player, desc);
}

char*
player_get_gdesc (Player* player)
{
    if (!player) return NULL;
    return entity_get_gdesc (player->e_player);
}

/*========== Position =====================*/

Status
player_set_position (Player* player, int x, int y)
{
    if (!player) return ERROR;
    return entity_set_position (player->e_player, x, y);
}
Position
player_get_position (Player* player)
{
    Position pos;
    pos.pos_x = NO_POS;
    pos.pos_y = NO_POS;

    if (!player) return pos;
    return entity_get_position (player->e_player);
}

int
player_get_pos_x (Player* player)
{
    if (!player) return NO_POS;
    return entity_get_pos_x (player->e_player);
}

int
player_get_pos_y (Player* player)
{
    if (!player) return NO_POS;
    return entity_get_pos_y (player->e_player);
}


/* =========== Speed ======================= */
Status
player_set_speed (Player* player, int speed)
{
    if (!player) return ERROR;
    return entity_set_speed (player->e_player, speed);
}
int
player_get_speed (Player* player)
{
    if (!player) return 0;
    return entity_get_speed (player->e_player);
}
/* =========== Vision (Set / Get) ========== */

Status
player_set_vision (Player* player, int x, int y)
{
    if (!player) return ERROR;
    if (x >= WIDHT_SCREEN || x < 0 || y >= HIGHT_SCREEN || y < 0)
        {
            player->vision.pos_x = NO_POS;
            player->vision.pos_y = NO_POS;
            return ERROR; /*Es un error que notifica que Vision ya no existirá*/
        }
    player->vision.pos_x = x;
    player->vision.pos_y = y;

    return OK;
}

Position
player_get_vision (Player* player)
{
    Position vision;
    vision.pos_x = NO_POS;
    vision.pos_y = NO_POS;

    if (!player) return vision;
    vision.pos_x = player->vision.pos_x;
    vision.pos_y = player->vision.pos_y;

    return vision;
}

Status
player_set_vision_x (Player* player, int x)
{
    if (!player) return ERROR;
    if (x >= WIDHT_SCREEN || x < 0)
        {
            player->vision.pos_x = NO_POS;
            player->vision.pos_y = NO_POS;
            return ERROR; /*Es un error que notifica que Vision ya no existirá*/
        }
    player->vision.pos_x = x;
    return OK;
}

Status
player_set_vision_y (Player* player, int y)
{
    if (!player) return ERROR;
    if (y >= HIGHT_SCREEN || y < 0)
        {
            player->vision.pos_x = NO_POS;
            player->vision.pos_y = NO_POS;
            return ERROR; /*Es un error que notifica que Vision ya no existirá*/
        }
    player->vision.pos_y = y;
    return OK;
}

int
player_get_vision_x (Player* player)
{
    if (!player) return NO_POS;
    return player->vision.pos_x;
}

int
player_get_vision_y (Player* player)
{
    if (!player) return NO_POS;
    return player->vision.pos_y;
}

/* ========== Print ========== */

Status
player_print (Player* player)
{
    Id player_id;
    Id player_zone;
    int player_n_objects;
    char* player_name    = NULL;
    char* player_gdesc   = NULL;
    char* player_message = NULL;

    if (!player) return ERROR;

    player_id        = player_get_id (player);
    player_zone      = player_get_zone (player);
    player_n_objects = player_get_n_objects (player);
    player_name      = player_get_name (player);
    player_gdesc     = player_get_gdesc (player);
    player_message   = player_get_message (player);

    fprintf (stdout,
             "--> Player (Id: %ld; Name: %s; Gdesc: %s; Zone: %ld; "
             "Objects: %d; Message: %s)\n",
             player_id, player_name ? player_name : "NULL", player_gdesc ? player_gdesc : "NULL", player_zone, player_n_objects,
             player_message ? player_message : "NULL");

    if (player_name) free (player_name);
    if (player_gdesc) free (player_gdesc);
    if (player_message) free (player_message);

    return OK;
}