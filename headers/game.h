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
 * @brief 
 * @author Violeta y Rafa
 *
 * @param game a pointer to game
 * @param id the palyer's ID
 * @return OK, if everything goes well, or ERROR if there was some mistake
 */
Status game_set_player_location(Game *game, Id id);

/**
 * @brief 
 * @author Violeta y Rafa
 *
 * @param game a pointer to game
 * @param id_obj the object's ID
 * @return OK, if everything goes well, or ERROR if there was some mistake
 */
Status game_set_object_location(Game *game, Id space_id);

/**
 * @brief It verifies that there's an object and saves it inside the player
 * @author Violeta y Rafa
 *
 * @param game a pointer to game
 * @param id_obj the object's ID
 * @return OK, if everything goes well, or ERROR if there was some mistake
 */
Status game_set_last_command(Game *game, Command *command);

/**
 * @brief It verifies that there's an object and saves it inside the player
 * @author Violeta y Rafa
 *
 * @param game a pointer to game
 * @param id_obj the object's ID
 * @return OK, if everything goes well, or ERROR if there was some mistake
 */
Status game_set_finished(Game *game, Bool finished);




/* functions to gets properties of games */
Space *game_get_space(Game *game, Id id);

Command *game_get_last_command(Game *game);

Id game_get_player_location(Game *game);

Id game_get_object_location(Game *game);

Id game_get_object_id(Game *game);


Id game_get_space_id_at(Game *game, int position);
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
