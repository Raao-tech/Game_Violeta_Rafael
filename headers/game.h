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

typedef struct _Game {
  Player*	player;
  Object*	objects;
  Space*	spaces[MAX_SPACES];
  Command *	last_cmd;  

  int		n_spaces;
  Bool		finished;
} Game;

/* functions to create or destroy game */
Status game_create(Game *game);
Status game_destroy(Game *game);

Status game_create_from_file(Game *game, char *filename);

/* functions to stes properties of games */
Status game_create_player(Game *game, Id id_player);
Status game_create_object(Game *game, Id id_obj, char* name);
Status game_set_player_location(Game *game, Id id);
Status game_set_object_location(Game *game, Id space_id);
Status game_set_last_command(Game *game, Command *command);
Status game_set_finished(Game *game, Bool finished);

/* functions to gets properties of games */
Space*	game_get_space(Game *game, Id id);
Command* game_get_last_command(Game *game);
Id game_get_player_location(Game *game);
Id game_get_object_location(Game *game);
Id game_get_object_id(Game *game);
Id game_get_space_id_at(Game *game, int position);


/* functions to actions of the player */
Status game_player_take(Game *game, Id id_obj);
Status game_player_drop(Game *game);
/**
 * @brief It gets value boolean (TRUE) if game finish or (FALSE) while game do not finish yet
 * @author Violeta y Rafa
 *
 * @param game a pointer to the game
 * @return a value boolean, (TRUE) if game finish  or there was a problem; (FALSE) while game do not finish yet;
 */
Bool game_get_finished(Game *game);

Status game_add_space(Game *game, Space *space);

void game_print(Game *game);

#endif
