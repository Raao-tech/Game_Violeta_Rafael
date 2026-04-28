/**
 * @brief It defines the player module interface
 *
 * @file player.h
 * @author Violeta y Rafa
 * @version 3
 * @date 08-04-2026
 * @copyright GNU Public License
 */

#ifndef PLAYER_H
#define PLAYER_H

#include "entity.h"
#include "set.h"
#include "types.h"

typedef struct _Player Player;

/* ========== Create / Destroy ========== */

/**
 * @brief It creates a new player, allocating memory and initializing it
 * @author Violeta y Rafa
 *
 * @return a new player, initialized, or NULL if memory fails
 */
Player* player_create ();

/**
 * @brief It destroys a player, freeing the allocated memory
 * @author Violeta y Rafa
 *
 * @param player a pointer to the player
 * @return OK, if everything goes well, or ERROR if there was some mistake
 */
Status player_destroy (Player* player);

/* ========== Id ========== */

/**
 * @brief It sets the id of a player
 * @author Rafael
 *
 * @param player a pointer to the player
 * @param new_id the new id value
 * @return OK, if everything goes well, or ERROR if there was some mistake
 */
Status player_set_id (Player* player, Id new_id);

/**
 * @brief It gets the id of a player
 * @author Rafael
 *
 * @param player a pointer to the player
 * @return the player's id, or NO_ID if player is NULL
 */
Id player_get_id (Player* player);

/* ========== Name ========== */

/**
 * @brief It sets the name of a player
 * @author Violeta y Rafa
 *
 * @param player a pointer to the player
 * @param name the name of the player
 * @return OK, if everything goes well, or ERROR if there was some mistake
 */
Status player_set_name (Player* player, char* name);

/**
 * @brief It gets a copy of the name of a player (caller must free it)
 * @author Violeta y Rafa
 *
 * @param player a pointer to the player
 * @return a copy of the name, or NULL if player is NULL
 */
char* player_get_name (Player* player);

/**
 * @brief It checks whether the player has the given name
 * @author Salvador
 *
 * @param player a pointer to the player
 * @param name the name to compare against
 * @return TRUE if the names match, FALSE otherwise
 */
Bool player_has_name (Player* player, char* name);

/* ========== Stats (No se deben de usasr) ========== */

Status player_set_health (Player* player, int health);
int player_get_health (Player* player);

/* ========== Objects (Inventory) ========== */

/**
 * @brief It adds an object id to the player's inventory
 * @author Violeta y Rafa
 *
 * @param player a pointer to the player
 * @param new_obj the id of the object to add
 * @return OK, if everything goes well, or ERROR if there was some mistake
 */
Status player_add_object (Player* player, Id new_obj);

/**
 * @brief It checks whether the player carries a specific object
 * @author Violeta y Rafa
 *
 * @param player a pointer to the player
 * @param ref_obj the id of the object to search for
 * @return TRUE if found, FALSE otherwise
 */
Bool player_contains_object (Player* player, Id ref_obj);

/**
 * @brief It removes an object id from the player's inventory
 * @author Violeta y Rafa
 *
 * @param player a pointer to the player
 * @param trash_obj the id of the object to remove
 * @return OK if removed, ERROR if not found or player is NULL
 */
Status player_delete_object (Player* player, Id trash_obj);

/**
 * @brief It returns the number of objects the player carries
 * @author Violeta y Rafa
 *
 * @param player a pointer to the player
 * @return number of objects, or ERROR_MAIN if player is NULL
 */
int player_get_n_objects (Player* player);

/**
 * @brief It sets the maximum number of objects the player can carry
 * @author Rafael
 *
 * This is a wrapper around inventory_set_max_objs that respects
 * encapsulation: game_reader calls this instead of accessing
 * the Inventory directly.
 *
 * @param player a pointer to the player
 * @param max the maximum capacity of the backpack
 * @return OK if successful, ERROR if player is NULL or max < 0
 */
Status player_set_max_objects (Player* player, int max);
/* ========== Numens ==========*/

/**
 * @brief It adds a numen id to the player's numen inventory
 * @author Rafael
 *
 * @param player a pointer to the player
 * @param new_numen the id of the numen to add
 * @return OK if successful, ERROR if player is NULL or max < 0
 */
Status player_add_numen (Player* player, Id new_numen);

/**
 * @brief It deletes a numen id from the player's numen inventory
 * @author Rafael
 *
 * @param player a pointer to the player
 * @param trash_numen the id of the numen to delete
 * @return OK if successful, ERROR if player is NULL or numen not found
 */
Status player_delete_numen (Player* player, Id trash_numen); /*!< ELimina a este Numen*/

/**
 * @brief It checks whether a numen id is in the player's numen inventory
 * @author Rafael
 *
 * @param player a pointer to the player
 * @param ref_numen the id of the numen to search for
 * @return TRUE if found, FALSE otherwise
 */
Bool player_contains_numen (Player* player, Id ref_numen); /*!< Lo contiene?*/

/**
 * @brief It gets the number of numens the player has
 * @author Rafael
 *
 * @param player a pointer to the player
 * @return the number of numens, or ERROR_MAIN if player is NULL
 */
int player_get_n_numens (Player* player); /*!< Cuántos tiene?*/

/**
 * @brief It sets the maximum number of numens the player can carry
 * @author Rafael
 *
 * @param player a pointer to the player
 * @param max_numens the maximum capacity of the numen inventory
 * @return OK if successful, ERROR if player is NULL or max_numens < 0
 */
Status player_set_max_numens (Player* player, int max_numens); /*!< El máximo es...*/

/**
 * @brief It gets the id of the active numen
 * @author Salvador
 *
 * @param player a pointer to the player
 * @return the id of the active numen, or NO_ID if player is NULL
 */
Id player_get_active_numen (Player* player);

/**
 * @brief It sets the active numen
 * @author Salvador
 *
 * @param player a pointer to the player
 * @param numen_id the id of the numen to set as active
 * @return OK if successful, ERROR if player is NULL or numen not found
 */
Status player_set_active_numen (Player* player, Id numen_id);

/* ========== Zone   ========== */

/**
 * @brief It sets the Zone of a player
 * @author Violeta y Rafa
 *
 * @param player a pointer to the player
 * @param new_Zone the id of the space where the player is
 * @return OK, if everything goes well, or ERROR if there was some mistake
 */
Status player_set_zone (Player* player, Id new_zone);

/**
 * @brief It gets the Zone of a player
 * @author Violeta y Rafa
 *
 * @param player a pointer to the player
 * @return the id of the space the player is in, or NO_ID if player is NULL
 */
Id player_get_zone (Player* player);

/* ========== Message ========== */

/**
 * @brief It sets the message of a player
 * @author Violeta y Rafa
 *
 * @param player a pointer to the player
 * @param messg a string with the message
 * @return OK, if everything goes well, or ERROR if there was some mistake
 */
Status player_set_message (Player* player, char* messg);

/**
 * @brief It gets a copy of the message of a player (caller must free it)
 * @author Violeta y Rafa
 *
 * @param player a pointer to the player
 * @return a copy of the message, or NULL if player is NULL
 */
char* player_get_message (Player* player);

/* ========== Graphic description ========== */

/**
 * @brief It sets the graphic description of a player
 * @author Violeta y Rafa
 *
 * @param player a pointer to the player
 * @param desc a string with the graphic description
 * @return OK, if everything goes well, or ERROR if there was some mistake
 */
Status player_set_gdesc (Player* player, char* desc);

/**
 * @brief It gets a copy of the graphic description of a player (caller must free it)
 * @author Violeta y Rafa
 *
 * @param player a pointer to the player
 * @return a copy of the gdesc, or NULL if player is NULL
 */
char* player_get_gdesc (Player* player);

/*========== Position =====================*/
/**
 * @brief It sets the position x and y of the player
 * @author Salvador
 *
 * @param player a pointer to the player
 * @param x x position to be set
 * @param y y position to be set
 * @return OK if position set properly, ERROR otherwise
 */
Status player_set_position (Player* player, int x, int y);


Position	player_get_position (Player* player);

/**
 * @brief It gets the position x of the player
 * @author Salvador
 *
 * @param player a pointer to the player
 * @return position x of the player, -1 otherwise
 */
int player_get_pos_x (Player* player);

/**
 * @brief It gets the position y of the player
 * @author Salvador
 *
 * @param player a pointer to the player
 * @return position y of the player, -1 otherwise
 */
int player_get_pos_y (Player* player);

/* =========== Vision (Set/Get) ==========*/
Status  player_set_vision(Player* player, int x, int y);
Position player_get_vision(Player* player);

Status  player_set_vision_x(Player* player, int x);
int     player_get_vision_x(Player* player);

Status  player_set_vision_y(Player* player, int y);
int     player_get_vision_y(Player* player);

/* ========== Print ========== */

/**
 * @brief It prints all the player information for debugging
 * @author Violeta y Rafa
 *
 * @param player a pointer to the player
 * @return OK, if everything goes well, or ERROR if player is NULL
 */
Status player_print (Player* player);

#endif