/**
 * @brief It defines the game interface
 *
 * This module manages the global state of the game: spaces, objects,
 * characters, players, links, commands, turns and game-over flag.
 * It follows Enfoque B (expose pointers directly rather than
 * abstracting through IDs at every layer).
 *
 * @file game.h
 * @author Profesores PPROG, Violeta, Rafael and Salvador
 * @version 3.2
 * @date 08-04-2026
 * @copyright GNU Public License
 */

#ifndef GAME_H
#define GAME_H

#include "command.h"
#include "space.h"
#include "types.h"
#include "player.h"
#include "object.h"
#include "character.h"
#include "links.h"

/** @brief Maximum number of spaces the game can hold */
#define MAX_SPACES     100
/** @brief Maximum number of objects the game can hold */
#define MAX_OBJECTS    100
/** @brief Maximum number of characters the game can hold */
#define MAX_CHARACTERS 100
/** @brief Maximum number of players the game can hold */
#define MAX_PLAYERS    10
/** @brief Maximum number of links the game can hold */
#define MAX_LINKS      (MAX_SPACES * 4)

/**
 * @brief Game ADT (opaque type)
 *
 * The internal structure contains arrays for all game entities,
 * a Command for the last input, counters, turn index, and flags.
 */
typedef struct _Game Game;


/* ========================================================================= */
/*                          CREATE / DESTROY                                 */
/* ========================================================================= */

/**
 * @brief It creates a new game, allocating memory and initializing all members
 * @author Profesores PPROG, Rafa, Violeta and Salvador
 *
 * All entity arrays are set to NULL, all counters to 0, turn to 0,
 * finished to FALSE, and a Command is created internally.
 *
 * @return pointer to the new game, or NULL if memory allocation fails
 */
Game *game_create();

/**
 * @brief It destroys a game, freeing all allocated memory
 * @author Violeta, Rafa and Salvador
 *
 * Iterates through all entity arrays and calls each TAD's destroy
 * function, then frees the Command and the Game struct itself.
 *
 * @param game a pointer to the game
 * @return OK if everything goes well, or ERROR if game is NULL
 */
Status game_destroy(Game *game);


/* ========================================================================= */
/*                     ACCESS: ENFOQUE B (expose pointers)                   */
/* ========================================================================= */

/**
 * @brief It gets the full array of player pointers
 * @author Salvador
 *
 * Used by game_reader to load multiple players from file.
 *
 * @param game a pointer to the game
 * @return pointer to the players array, or NULL if game is NULL
 */
Player **game_get_players(Game *game);

/**
 * @brief It gets the active player (whose turn it is)
 * @author Rafael
 *
 * Convenience function.  Equivalent to game_get_player_by_turn.
 * Provided for backward compatibility with code that calls
 * game_get_player(game).
 *
 * @param game a pointer to the game
 * @return pointer to the active player, or NULL if no players exist
 */
Player *game_get_player(Game *game);

/**
 * @brief It finds a space by its id
 * @author Violeta y Rafa
 *
 * Iterates through the spaces array comparing each space's id.
 *
 * @param game a pointer to the game
 * @param id the id of the space to find
 * @return pointer to the space, or NULL if not found or game is NULL
 */
Space *game_get_space(Game *game, Id id);

/**
 * @brief It gets the last command registered in the game
 * @author Violeta y Rafa
 *
 * The Command struct is owned by Game and created in game_create.
 * It is reused across turns — command_get_user_input overwrites it.
 *
 * @param game a pointer to the game
 * @return pointer to the last command, or NULL if game is NULL
 */
Command *game_get_last_command(Game *game);


/* ========================================================================= */
/*                       CONNECTIONS (via Links)                              */
/* ========================================================================= */

/**
 * @brief It gets the id of the destination space given an origin and direction
 * @author Salvador
 *
 * Searches all links for one that matches the origin space and
 * direction (either as origin->dest or as dest->origin using the
 * opposite direction).  This is the bidirectional link model.
 *
 * @param game a pointer to the game
 * @param space the id of the origin space
 * @param dir the direction to query (N, S, E, W)
 * @return the id of the destination space, or NO_ID if no link found
 */
Id game_get_connection(Game *game, Id space, Direction dir);

/**
 * @brief It checks whether a connection in a given direction is open
 * @author Salvador
 *
 * Same link search as game_get_connection, but returns the open/closed
 * state instead of the destination id.  In the bidirectional model,
 * it checks open_origin_to_dest or open_dest_to_origin depending
 * on which end of the link matches the queried space.
 *
 * @param game a pointer to the game
 * @param space the id of the origin space
 * @param dir the direction to query
 * @return TRUE if the connection is open, FALSE otherwise
 */
Bool game_connection_is_open(Game *game, Id space, Direction dir);


/* ========================================================================= */
/*                   ADD ELEMENTS (used by game_reader)                      */
/* ========================================================================= */

/**
 * @brief It adds a space to the game
 * @author Violeta y Rafa
 *
 * Appends the space pointer to the internal array and increments
 * the counter.  Fails if the array is full (MAX_SPACES).
 *
 * @param game a pointer to the game
 * @param space a pointer to the space to add
 * @return OK if added, ERROR if NULL or array full
 */
Status game_add_space(Game *game, Space *space);

/**
 * @brief It adds an object to the game
 * @author Violeta y Rafa
 *
 * Appends the object pointer to the internal array and increments
 * the counter.  Fails if the array is full (MAX_OBJECTS).
 *
 * @param game a pointer to the game
 * @param obj a pointer to the object to add
 * @return OK if added, ERROR if NULL or array full
 */
Status game_add_object(Game *game, Object *obj);

/**
 * @brief It adds a character to the game
 * @author Violeta y Rafa
 *
 * Appends the character pointer to the internal array and increments
 * the counter.  Fails if the array is full (MAX_CHARACTERS).
 *
 * @param game a pointer to the game
 * @param character a pointer to the character to add
 * @return OK if added, ERROR if NULL or array full
 */
Status game_add_character(Game *game, Character *character);

/**
 * @brief It adds a player to the game
 * @author Salvador
 *
 * Multiple players can be added for the turn-based multiplayer
 * mode (F11).  Each player is appended to the players array.
 *
 * @param game a pointer to the game
 * @param player a pointer to the player to add
 * @return OK if added, ERROR if NULL or array full
 */
Status game_add_player(Game *game, Player *player);

/**
 * @brief It adds a link to the game
 * @author Salvador
 *
 * Links define the topology of the map.  Each link connects two
 * spaces in a given direction with an open/closed state per direction.
 *
 * @param game a pointer to the game
 * @param link a pointer to the link to add
 * @return OK if added, ERROR if NULL or array full
 */
Status game_add_links(Game *game, Links *link);


/* ========================================================================= */
/*                          SEARCH: OBJECTS                                  */
/* ========================================================================= */

/**
 * @brief It finds an object by its id
 * @author Violeta y Rafa
 *
 * @param game a pointer to the game
 * @param id the id of the object to find
 * @return pointer to the object, or NULL if not found
 */
Object *game_get_object_by_id(Game *game, Id id);

/**
 * @brief It finds an object by its name
 * @author Violeta y Rafa
 *
 * Uses obj_has_name for case-sensitive comparison.
 *
 * @param game a pointer to the game
 * @param name the name of the object to find
 * @return pointer to the object, or NULL if not found
 */
Object *game_get_object_by_name(Game *game, char *name);

/**
 * @brief It finds which space contains a given object
 * @author Violeta y Rafa
 *
 * Iterates through all spaces checking space_contains_object.
 * If the object is in a player's inventory (not in any space),
 * returns NO_ID.
 *
 * @param game a pointer to the game
 * @param obj_id the id of the object to locate
 * @return the id of the space containing the object, or NO_ID
 */
Id game_get_object_location(Game *game, Id obj_id);


/* ========================================================================= */
/*                        SEARCH: CHARACTERS                                 */
/* ========================================================================= */

/**
 * @brief It finds a character by its id
 * @author Violeta y Rafa
 *
 * @param game a pointer to the game
 * @param id the id of the character to find
 * @return pointer to the character, or NULL if not found
 */
Character *game_get_character_by_id(Game *game, Id id);

/**
 * @brief It finds a character by its name
 * @author Violeta y Rafa
 *
 * Uses character_has_name for case-sensitive comparison.
 *
 * @param game a pointer to the game
 * @param name the name of the character to find
 * @return pointer to the character, or NULL if not found
 */
Character *game_get_character_by_name(Game *game, char *name);

/**
 * @brief It finds which space contains a given character
 * @author Violeta y Rafa
 *
 * @param game a pointer to the game
 * @param char_id the id of the character to locate
 * @return the id of the space containing the character, or NO_ID
 */
Id game_get_character_location(Game *game, Id char_id);


/* ========================================================================= */
/*                         SEARCH: PLAYERS                                   */
/* ========================================================================= */

/**
 * @brief It finds a player by its id
 * @author Salvador
 *
 * @param game a pointer to the game
 * @param id the id of the player to find
 * @return pointer to the player, or NULL if not found
 */
Player *game_get_player_by_id(Game *game, Id id);

/**
 * @brief It finds a player by its name
 * @author Salvador
 *
 * @param game a pointer to the game
 * @param name the name of the player to find
 * @return pointer to the player, or NULL if not found
 */
Player *game_get_player_by_name(Game *game, char *name);

/**
 * @brief It returns the player whose turn it currently is
 * @author Rafa
 *
 * Uses the internal turn counter to index into the players array.
 *
 * @param game a pointer to the game
 * @return pointer to the active player, or NULL if no players or error
 */
Player *game_get_player_by_turn(Game *game);

/**
 * @brief It gets the location (space id) of a player by player id
 * @author Salvador
 *
 * @param game a pointer to the game
 * @param player_id the id of the player to locate
 * @return the id of the space containing the player, or NO_ID
 */
Id game_get_player_location(Game *game, Id player_id);

/**
 * @brief It gets the current turn index
 * @author Rafa
 *
 * @param game a pointer to the game
 * @return the turn index (0 to n_players-1), or -1 if game is NULL
 */
int game_get_turn(Game *game);


/* ========================================================================= */
/*                     ACCESS BY INDEX (for iteration)                       */
/* ========================================================================= */

/**
 * @brief It gets an object by its position in the array
 * @author Violeta y Rafa
 *
 * @param game a pointer to the game
 * @param position the zero-based index in the objects array
 * @return pointer to the object, or NULL if out of range
 */
Object *game_get_object_at(Game *game, int position);

/**
 * @brief It gets a character by its position in the array
 * @author Violeta y Rafa
 *
 * @param game a pointer to the game
 * @param position the zero-based index in the characters array
 * @return pointer to the character, or NULL if out of range
 */
Character *game_get_character_at(Game *game, int position);

/**
 * @brief It gets a player by its position in the array
 * @author Salvador
 *
 * @param game a pointer to the game
 * @param position the zero-based index in the players array
 * @return pointer to the player, or NULL if out of range
 */
Player *game_get_player_at(Game *game, int position);

/**
 * @brief It gets the id of a space by its position in the array
 * @author Violeta y Rafa
 *
 * @param game a pointer to the game
 * @param position the zero-based index in the spaces array
 * @return the id of the space, or NO_ID if out of range
 */
Id game_get_space_id_at(Game *game, int position);

/**
 * @brief It returns the number of spaces in the game
 * @author Violeta y Rafa
 *
 * @param game a pointer to the game
 * @return number of spaces, or -1 if game is NULL
 */
int game_get_n_spaces(Game *game);

/**
 * @brief It returns the number of objects in the game
 * @author Violeta y Rafa
 *
 * @param game a pointer to the game
 * @return number of objects, or -1 if game is NULL
 */
int game_get_n_objects(Game *game);

/**
 * @brief It returns the number of characters in the game
 * @author Violeta y Rafa
 *
 * @param game a pointer to the game
 * @return number of characters, or -1 if game is NULL
 */
int game_get_n_characters(Game *game);

/**
 * @brief It returns the number of players in the game
 * @author Salvador
 *
 * @param game a pointer to the game
 * @return number of players, or -1 if game is NULL
 */
int game_get_n_players(Game *game);

/**
 * @brief It returns the number of links in the game
 * @author Rafael
 *
 * Useful for graphic_engine to iterate over links when
 * displaying the map topology (F13).
 *
 * @param game a pointer to the game
 * @return number of links, or -1 if game is NULL
 */
int game_get_n_links(Game *game);


/* ========================================================================= */
/*                            GAME STATE                                     */
/* ========================================================================= */

/**
 * @brief It sets the game as finished or not
 * @author Violeta y Rafa
 *
 * Called when a player dies (health <= 0) or when the user
 * types the EXIT command.
 *
 * @param game a pointer to the game
 * @param finished TRUE to mark as finished, FALSE otherwise
 * @return OK if set, ERROR if game is NULL
 */
Status game_set_finished(Game *game, Bool finished);

/**
 * @brief It checks if the game has finished
 * @author Violeta y Rafa
 *
 * @param game a pointer to the game
 * @return TRUE if finished, FALSE otherwise (TRUE if game is NULL)
 */
Bool game_get_finished(Game *game);

/**
 * @brief It sets the last command of the game
 * @author Violeta y Rafa
 *
 * Called by game_actions_update to register the command before
 * executing it.  The Game does NOT take ownership of the Command
 * pointer — it was created in game_create and reused across turns.
 *
 * @param game a pointer to the game
 * @param command a pointer to the command
 * @return OK if set, ERROR if either pointer is NULL
 */
Status game_set_last_command(Game *game, Command *command);

/**
 * @brief It sets the status result of the last command executed
 * @author Violeta y Rafa
 *
 * The status can be OK, ERROR, or any of the specific error
 * codes defined in types.h (ERROR_Attack, ERROR_Chat, etc.).
 *
 * @param game a pointer to the game
 * @param status the result status to store
 * @return OK if set, ERROR if game is NULL
 */
Status game_set_last_cmd_status(Game *game, Status status);

/**
 * @brief It gets the status result of the last command executed
 * @author Violeta y Rafa
 *
 * @param game a pointer to the game
 * @return the stored status, or ERROR if game is NULL
 */
Status game_get_last_cmd_status(Game *game);

/**
 * @brief It advances the turn to the next player
 * @author Salvador
 *
 * Increments the turn counter modulo n_players, so it wraps
 * around from the last player back to the first.
 *
 * @param game a pointer to the game
 * @return OK if updated, ERROR if game is NULL or no players exist
 */
Status game_turn_update(Game *game);


/* ========================================================================= */
/*                         PRINT (debugging)                                 */
/* ========================================================================= */

/**
 * @brief It prints the full game state to stdout for debugging
 * @author Violeta, Rafa and Salvador
 *
 * Shows all players (with turn), spaces, objects, characters, links,
 * and whether the game is finished.
 *
 * @param game a pointer to the game
 */
void game_print(Game *game);

#endif