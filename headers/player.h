#ifndef PLAYER_H
#define PLAYER_H

#include "game.h"

typedef struct _Player Player;


/**
 * @brief It creates a new player, allocating memory and initializing it
 * @author Violeta y Rafa
 *
 * @param id the identification number for the new player
 * @param name the name for the new player
 * @return a new player, initialized
 */
Player* player_create(Id id);

/**
 * @brief It destroys a player, freeing the allocated memory
 * @author Violeta y Rafa
 *
 * @param player a pointer to the player
 * @return OK, if everything goes well, or ERROR if there was some mistake
 */
Status player_destroy(Player* player);

/**
 * @brief It sets the name of a player
 * @author Violeta y Rafa
 *
 * @param player a pointer to the player
 * @param name the name of the player
 * @return OK, if everything goes well, or ERROR if there was some mistake
 */
Status player_set_name(Player* player, char* name);

/**
 * @brief It sets the id of the obcjet of the player
 * @author Violeta y Rafa
 *
 * @param player a pointer to the player
 * @param id_obj a Id of the object of the player
 * @return OK, if object is on the player, or ERROR if there  was not  some object
 */
Status player_set_Obj(Player* player,  Id id_obj) ;


/**
 * @brief It sets location of player
 * @author Violeta y Rafa
 *
 * @param player a pointer to the player
 * @param id_space a space's id of location of player
 * @return OK, if everything goes well, or ERROR if there was some mistake
 */
Status player_set_location (Player* player, Id id_space);


/**
 * @brief It gets the name of a player
 * @author Violeta y Rafa
 *
 * @param player a pointer to the player
 * @return a string with the name of the player
 */
const char* player_get_name(Player* player);


/**
 * @brief It prints the player information
 * @author Violeta y Rafa
 *
 * @param player a pointer to the player
 * @return OK, if everything goes well, or ERROR if there was some mistake
 */
Status player_print(Player* player);


#endif