/**
 * @brief It defines the game interface
 *
 * @file game.h
 * @author Profesores PPROG
 * @version 0
 * @date 27-01-2025
 * @copyright GNU Public License
 */

#ifndef GAME_H
#define GAME_H

#include "command.h"
#include "space.h"
#include "types.h"
#include "player.h"
#include "object.h"

#define MAX_SPACES 100


typedef struct _Game
{
  Player *player;
  Object *objects;
  Space *spaces[MAX_SPACES];
  Command *last_cmd;

  int n_spaces;
  Bool finished;
} Game;


/**
 * @brief It creates a new game, allocating memory and initializing it
 * @author Profesores PPROG
 *
 * @param game a pointer to game
 * @return OK, if everything goes well, or ERROR if there was some mistake
 */
Status game_create(Game *game);

/**
 * @brief It destroys a game, freeing the allocated memory
 * @author Violeta y Rafa
 *
 * @param obj a pointer to the game
 * @return OK, if everything goes well, or ERROR if there was some mistake
 */
Status game_destroy(Game *game);

/**
 * @brief It creates a game from a file
 * @author Violeta y Rafa
 *
 * @param game a pointer to game
 * @param filename string with the name of the file
 * @return OK, if everything goes well, or ERROR if there was some mistake
 */
Status game_create_from_file(Game *game, char *filename);


/**
 * @brief It creates a new player, allocating memory and initializing it
 * @author Violeta y Rafa
 *
 * @param game a pointer to game
 * @param id_player the player's ID
 * @return OK, if everything goes well, or ERROR if there was some mistake
 */
Status game_create_player(Game *game, Id id_player);

/**
 * @brief It creates a new object, allocating memory and initializing it
 * @author Violeta y Rafa
 *
 * @param game a pointer to game
 * @param id_obj the object's ID
 * @param name string of the object's name
 * @return OK, if everything goes well, or ERROR if there was some mistake
 */
Status game_create_object(Game *game, Id id_obj, char *name);

/**
 * @brief Sets the location of a player
 * @author Violeta y Rafa
 *
 * @param game a pointer to game
 * @param id the ID of the space where we want to set the player
 * @return OK, if everything goes well, or ERROR if there was some mistake
 */
Status game_set_player_location(Game *game, Id id);

/**
 * @brief Sets the location of an object
 * @author Violeta y Rafa
 *
 * @param game a pointer to game
 * @param space_id the ID of the space where we want to set the object
 * @return OK, if everything goes well, or ERROR if there was some mistake
 */
Status game_set_object_location(Game *game, Id space_id);

/**
 * @brief Sets the game's last command
 * @author Violeta y Rafa
 *
 * @param game a pointer to game
 * @param command a pointer to command
 * @return OK, if everything goes well, or ERROR if there was some mistake
 */
Status game_set_last_command(Game *game, Command *command);

/**
 * @brief Sets game as finished (TRUE) or not finished (FALSE)
 * @author Violeta y Rafa
 *
 * @param game a pointer to game
 * @param finished the boolean value (TRUE or FALSE)
 * @return OK, if everything goes well, or ERROR if there was some mistake
 */
Status game_set_finished(Game *game, Bool finished);



/**
 * @brief Gets a space from its ID
 * @author Violeta y Rafa
 *
 * @param game a pointer to game
 * @param id the ID of the space we want to get
 * @return the space we get
 */
Space *game_get_space(Game *game, Id id);

/**
 * @brief Gets the last command
 * @author Violeta y Rafa
 *
 * @param game a pointer to game
 * @return the last command
 */
Command *game_get_last_command(Game *game);

/**
 * @brief Gets the location of a player
 * @author Violeta y Rafa
 *
 * @param game a pointer to game
 * @return the ID of the space the player is located in
 */
Id game_get_player_location(Game *game);

/**
 * @brief Gets the location of an object
 * @author Violeta y Rafa
 *
 * @param game a pointer to game
 * @return the ID of the space the object is located in
 */
Id game_get_object_location(Game *game);

/**
 * @brief Gets the ID of an object
 * @author Violeta y Rafa
 *
 * @param game a pointer to game
 * @return the ID of the object
 */
Id game_get_object_id(Game *game);

/**
 * @brief Gets the ID of a space from its position in the array
 * @author Violeta y Rafa
 *
 * @param game a pointer to game
 * @param position the position of the space in the array
 * @return the ID of the space
 */
Id game_get_space_id_at(Game *game, int position);

/**
 * @brief Gets the number of spaces in the array
 * @author Violeta y Rafa
 *
 * @param game a pointer to game
 * @return the number of spaces in the spaces array
 */
int game_get_n_spaces(Game *game);



/**
 * @brief It verifies that there's an object and saves it inside the player
 * @author Violeta y Rafa
 *
 * @param game a pointer to game
 * @param id_obj the object's ID
 * @return OK, if everything goes well, or ERROR if there was some mistake
 */
Status game_player_take(Game *game, Id id_obj);

/**
 * @brief It verifies that there's an object and the player drops it
 * @author Violeta y Rafa
 *
 * @param game a pointer to game
 * @return OK, if everything goes well, or ERROR if there was some mistake
 */
Status game_player_drop(Game *game);

/**
 * @brief It checks if the game has finished or not
 * @author Violeta y Rafa
 *
 * @param game a pointer to the game
 * @return a boolean value: TRUE if it has finished (correctly or not), FALSE if it hasn't finished yet;
 */
Bool game_get_finished(Game *game);

/**
 * @brief It adds a space to the game
 * @author Violeta y Rafa
 *
 * @param game a pointer to game
 * @param space a pointer to space
 * @return OK, if everything goes well, or ERROR if there was some mistake
 */
Status game_add_space(Game *game, Space *space);

/**
 * @brief It prints a game from a file
 * @author Violeta y Rafa
 *
 * @param game a pointer to game
 * @param filename string with the name of the file
 */
void game_print(Game *game);


#endif
