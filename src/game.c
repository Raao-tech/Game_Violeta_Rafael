/**
 * @brief It implements the game structure
 *
 * @file game.c
 * @author Profesores PPROG, Violeta, Rafael and Salvador
 * @version 3.2
 * @date 08-04-2026
 * @copyright GNU Public License
 */

#include "game.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Game
 *
 * This struct stores all the global state of the game.
 */
struct _Game
{
	Player* players[MAX_PLAYERS]; /*!< Array of player pointers */
	Numen* numens[MAX_NUMENS];    /*!< Array of  numens pointers */
	Object* objects[MAX_OBJECTS]; /*!< Array of object pointers */
	Space* spaces[MAX_SPACES];    /*!< Array of space pointers */
	Links* links[MAX_LINKS];      /*!< Array of link pointers */

	Command* last_cmd; /*!< Last command entered by the user */

	int turn;               /*!< Index of the active player (0 to n_players-1) */
	int n_players;          /*!< Current number of players */
	int n_spaces;           /*!< Current number of spaces */
	int n_objects;          /*!< Current number of objects */
	int n_numens;           /*!< Current number of numens */
	int n_links;            /*!< Current number of links */
	Bool finished;          /*!< TRUE if the game is over */
	Status last_cmd_status; /*!< Result of the last command (OK, ERROR, etc.) */
};

/* ========================================================================= */
/*                          CREATE / DESTROY                                 */
/* ========================================================================= */

Game*
game_create ()
{
	int i;
	Game* game = NULL;

	game       = (Game*)malloc (sizeof (Game));
	if (!game) return NULL;

	/* Initialize every pointer array to NULL */
	for (i = 0; i < MAX_PLAYERS; i++) game->players[i] = NULL;
	for (i = 0; i < MAX_SPACES; i++) game->spaces[i] = NULL;
	for (i = 0; i < MAX_OBJECTS; i++) game->objects[i] = NULL;
	for (i = 0; i < MAX_NUMENS; i++) game->numens[i] = NULL;
	for (i = 0; i < MAX_LINKS; i++) game->links[i] = NULL;

	/*
	 * Every counter MUST be set to 0 explicitly.
	 * malloc does NOT zero memory (unlike calloc), so without
	 * these lines the values are garbage.
	 */
	game->turn            = 0;
	game->n_players       = 0;
	game->n_spaces        = 0;
	game->n_objects       = 0;
	game->n_numens        = 0;
	game->n_links         = 0;
	game->finished        = FALSE;
	game->last_cmd_status = OK;

	/* Create the Command that will be reused across turns */
	game->last_cmd = command_create ();
	if (!game->last_cmd)
		{
			free (game);
			return NULL;
		}

	return game;
}

Status
game_destroy (Game* game)
{
	int i;
	if (!game) return ERROR;

	for (i = 0; i < game->n_spaces; i++) space_destroy (game->spaces[i]);
	for (i = 0; i < game->n_objects; i++) obj_destroy (game->objects[i]);
	for (i = 0; i < game->n_numens; i++) numen_destroy (game->numens[i]);
	for (i = 0; i < game->n_players; i++) player_destroy (game->players[i]);
	for (i = 0; i < game->n_links; i++) link_destroy (game->links[i]);

	command_destroy (game->last_cmd);
	free (game);
	return OK;
}

/* ========================================================================= */
/*                     ACCESS: ENFOQUE B (expose pointers)                   */
/* ========================================================================= */

Player**
game_get_players (Game* game)
{
	if (!game) return NULL;
	return game->players;
}

Player*
game_get_player (Game* game)
{
	if (!game) return NULL;
	return game_get_player_by_turn (game);
}

Command*
game_get_last_command (Game* game)
{
	if (!game) return NULL;
	return game->last_cmd;
}

Space*
game_get_space (Game* game, Id id)
{
	int i;
	if (!game || id == NO_ID) return NULL;

	for (i = 0; i < game->n_spaces; i++)
		{
			if (space_get_id (game->spaces[i]) == id) return game->spaces[i];
		}
	return NULL;
}

/* ========================================================================= */
/*                       CONNECTIONS (via Links)                              */
/* ========================================================================= */

Id
game_get_connection (Game* game, Id space, Direction dir)
{
	int i;
	Direction dir_norm, dir_opp;
	Id id_orig, id_dest;

	if (!game || space == NO_ID || dir == U) return NO_ID;

	for (i = 0; i < game->n_links; i++)
		{
			dir_norm = link_get_direction (game->links[i]);
			dir_opp  = link_get_opposite_direction (game->links[i]);
			id_orig  = link_get_origin_id (game->links[i]);
			id_dest  = link_get_destination_id (game->links[i]);

			/* Forward match: we are at the origin and want to go in dir */
			if (id_orig == space && dir_norm == dir) return id_dest;

			/* Reverse match: we are at the dest and want to go in opposite dir */
			if (id_dest == space && dir_opp == dir) return id_orig;
		}
	return NO_ID;
}

Bool
game_connection_is_open (Game* game, Id space, Direction dir)
{
	int i;
	Direction dir_norm, dir_opp;
	Id id_orig, id_dest;

	if (!game || space == NO_ID || dir == U) return FALSE;

	for (i = 0; i < game->n_links; i++)
		{
			dir_norm = link_get_direction (game->links[i]);
			dir_opp  = link_get_opposite_direction (game->links[i]);
			id_orig  = link_get_origin_id (game->links[i]);
			id_dest  = link_get_destination_id (game->links[i]);

			if (id_orig == space && dir_norm == dir) return link_get_open_origin_to_dest (game->links[i]);

			if (id_dest == space && dir_opp == dir) return link_get_open_dest_to_origin (game->links[i]);
		}
	return FALSE;
}

/* ========================================================================= */
/*                   ADD ELEMENTS (used by game_reader)                      */
/* ========================================================================= */

Status
game_add_space (Game* game, Space* space)
{
	if (!game || !space || game->n_spaces >= MAX_SPACES) return ERROR;
	game->spaces[game->n_spaces] = space;
	game->n_spaces++;
	return OK;
}

Status
game_add_object (Game* game, Object* obj)
{
	if (!game || !obj || game->n_objects >= MAX_OBJECTS) return ERROR;
	game->objects[game->n_objects] = obj;
	game->n_objects++;
	return OK;
}

Status
game_add_numen (Game* game, Numen* numen)
{
	if (!game || !numen || game->n_numens >= MAX_NUMENS) return ERROR;
	game->numens[game->n_numens] = numen;
	game->n_numens++;
	return OK;
}

Status
game_add_player (Game* game, Player* player)
{
	if (!game || !player || game->n_players >= MAX_PLAYERS) return ERROR;
	game->players[game->n_players] = player;
	game->n_players++;
	return OK;
}

Status
game_add_links (Game* game, Links* link)
{
	if (!game || !link || game->n_links >= MAX_LINKS) return ERROR;
	game->links[game->n_links] = link;
	game->n_links++;
	return OK;
}

/* ========================================================================= */
/*                          SEARCH: OBJECTS                                  */
/* ========================================================================= */

Object*
game_get_object_by_id (Game* game, Id id)
{
	int i;
	if (!game || id == NO_ID) return NULL;
	for (i = 0; i < game->n_objects; i++)
		{
			if (obj_get_id (game->objects[i]) == id) return game->objects[i];
		}
	return NULL;
}

Object*
game_get_object_by_name (Game* game, char* name)
{
	int i;
	if (!game || !name) return NULL;
	for (i = 0; i < game->n_objects; i++)
		{
			if (obj_has_name (game->objects[i], name) == TRUE) return game->objects[i];
		}
	return NULL;
}

Object*
game_get_object_by_vision (Game* game, Position vision_player)
{
	Object		*obj	= NULL;
	Position    pos_obj;
	int i;
	pos_obj.pos_x = NO_POS;
	pos_obj.pos_y = NO_POS;

	if (!game || vision_player.pos_x == NO_POS || vision_player.pos_y == NO_POS) return NULL;

	for (i = 0; i < game->n_objects; i++)
		{
			/*obtenemos el objeto*/
			obj = game_get_object_at (game, i);
			if(!obj) continue;
			/*preguntamos la posición*/
			pos_obj = obj_get_position (obj);
			if(pos_obj.pos_x == NO_POS || pos_obj.pos_y == NO_POS) continue;

			/*si las coordenadas coinciden, entonces, devolver el valor id del objeto*/
			if ( pos_obj.pos_x == vision_player.pos_x && pos_obj.pos_y == vision_player.pos_y)
				{return game->objects[i];}
			
		}
	/* Object might be in a player's inventory — not in any space */
	return NULL;
}


Id
game_get_object_location (Game* game, Id obj_id)
{
	int i;
	if (!game || obj_id == NO_ID) return NO_ID;
	for (i = 0; i < game->n_spaces; i++)
		{
			if (space_contains_object (game->spaces[i], obj_id) == TRUE) return space_get_id (game->spaces[i]);
		}
	/* Object might be in a player's inventory — not in any space */
	return NO_ID;
}



/* ========================================================================= */
/*                        SEARCH: NUMENS                               */
/* ========================================================================= */

Numen*
game_get_numen_by_id (Game* game, Id id_numen)
{
	int i;
	if (!game || id_numen == NO_ID) return NULL;
	for (i = 0; i < game->n_numens; i++)
		{
			if (numen_get_id (game->numens[i]) == id_numen) return game->numens[i];
		}
	return NULL;
}
Numen* 
game_get_numen_by_vision (Game* game, Position vision_player)
{
    Numen*   num = NULL;
    Position pos_num;
    int      i;

    if (!game || vision_player.pos_x == NO_POS || vision_player.pos_y == NO_POS)
        return NULL;

    for (i = 0; i < game->n_numens; i++)
    {
        num = game_get_numen_at (game, i);
        if (!num) continue;

        pos_num = numen_get_position (num);
        if (pos_num.pos_x == NO_POS || pos_num.pos_y == NO_POS) continue;

        if (pos_num.pos_x == vision_player.pos_x &&
            pos_num.pos_y == vision_player.pos_y)
            return num;
    }
    return NULL;
}

Numen*
game_get_numen_by_name (Game* game, char* name_numen)
{
	int i;
	if (!game || !name_numen) return NULL;
	for (i = 0; i < game->n_numens; i++)
		{
			if (numen_has_name (game->numens[i], name_numen) == TRUE) return game->numens[i];
		}
	return NULL;
}

/* ========================================================================= */
/*                         SEARCH: PLAYERS                                   */
/* ========================================================================= */

Player*
game_get_player_by_id (Game* game, Id id)
{
	int i;
	if (!game || id == NO_ID) return NULL;
	for (i = 0; i < game->n_players; i++)
		{
			if (player_get_id (game->players[i]) == id) return game->players[i];
		}
	return NULL;
}

Player*
game_get_player_by_name (Game* game, char* name)
{
	int i;
	if (!game || !name) return NULL;
	for (i = 0; i < game->n_players; i++)
		{
			if (player_has_name (game->players[i], name) == TRUE) return game->players[i];
		}
	return NULL;
}

Player*
game_get_player_by_turn (Game* game)
{
	if (!game) return NULL;
	if (game->n_players <= 0) return NULL;
	if (game->turn < 0 || game->turn >= game->n_players) return NULL;

	return game->players[game->turn];
}

int
game_get_turn (Game* game)
{
	if (!game) return -1;
	return game->turn;
}

Id
game_get_player_location (Game* game, Id player_id)
{
	int i;
	if (!game || player_id == NO_ID) return NO_ID;
	for (i = 0; i < game->n_players; i++)
		{
			if (player_get_id (game->players[i]) == player_id) return player_get_zone (game->players[i]);
		}
	return NO_ID;
}

/*
		game_player_get_objects_close()    Es una función para obenter los objetos cercanos al jugador
*/

/* ========================================================================= */
/*                         SEARCH: LINKS                                     */
/* ========================================================================= */
Links*
game_get_link_by_id (Game* game, Id id)
{
	int i;
	if (!game || id == NO_ID) return NULL;
	for (i = 0; i < game->n_links; i++)
		{
			if (link_get_id (game->links[i]) == id) return game->links[i];
		}
	return NULL;
}
Links*
game_get_link_by_name (Game* game, char* name)
{
	int i;
	if (!game || !name) return NULL;
	for (i = 0; i < game->n_links; i++)
		{
			if (link_has_name (game->links[i], name) == TRUE) return game->links[i];
		}
	return NULL;
}

/* ========================================================================= */
/*                     ACCESS BY INDEX (for iteration)                       */
/* ========================================================================= */

Object*
game_get_object_at (Game* game, int position)
{
	if (!game || position < 0 || position >= game->n_objects) return NULL;
	return game->objects[position];
}

Numen*
game_get_numen_at (Game* game, int position)
{
	if (!game || position < 0 || position >= game->n_numens) return NULL;
	return game->numens[position];
}

Player*
game_get_player_at (Game* game, int position)
{
	if (!game || position < 0 || position >= game->n_players) return NULL;
	return game->players[position];
}

Id
game_get_space_id_at (Game* game, int position)
{
	if (!game || position < 0 || position >= game->n_spaces) return NO_ID;
	return space_get_id (game->spaces[position]);
}

Numen*
game_get_numen__at (Game* game, int position)
{
	if (!game || position < 0 || position >= game->n_numens) return NULL;
	return game->numens[position];
}

Links*
game_get_link_at (Game* game, int position)
{
	if (!game || position < 0 || position >= game->n_links) return NULL;
	return game->links[position];
}

int
game_get_n_spaces (Game* game)
{
	if (!game) return -1;
	return game->n_spaces;
}

int
game_get_n_objects (Game* game)
{
	if (!game) return -1;
	return game->n_objects;
}

int
game_get_n_numens (Game* game)
{
	if (!game) return -1;
	return game->n_numens;
}

int
game_get_n_players (Game* game)
{
	if (!game) return -1;
	return game->n_players;
}

int
game_get_n_links (Game* game)
{
	if (!game) return -1;
	return game->n_links;
}

/* ========================================================================= */
/*                            GAME STATE                                     */
/* ========================================================================= */

Status
game_set_finished (Game* game, Bool finished)
{
	if (!game) return ERROR;
	game->finished = finished;
	return OK;
}

Bool
game_get_finished (Game* game)
{
	if (!game) return TRUE;
	return game->finished;
}

Status
game_set_last_command (Game* game, Command* command)
{
	if (!game || !command) return ERROR;
	game->last_cmd = command;
	return OK;
}

Status
game_set_last_cmd_status (Game* game, Status status)
{
	if (!game) return ERROR;
	game->last_cmd_status = status;
	return OK;
}

Status
game_get_last_cmd_status (Game* game)
{
	if (!game) return ERROR;
	return game->last_cmd_status;
}

Status
game_turn_update (Game* game)
{
	int checked = 0;

	if (!game) return ERROR;
	if (game->n_players <= 0) return ERROR;

	do
		{
			game->turn = (game->turn + 1) % game->n_players;
			checked++;

			if (checked >= game->n_players)
				{
					/* Todos muertos -> game over */
					game->finished = TRUE;
					return OK;
				}
		}
	while (player_get_health (game->players[game->turn]) <= 0);

	return OK;
}



/* ========================================================================= */
/*                         PRINT (debugging)                                 */
/* ========================================================================= */

void
game_print (Game* game)
{
	int i;
	Id loc;

	if (!game) return;

	printf ("\n============ GAME STATE ============\n");

	/* Players */
	printf ("\n=> Players (%d), active turn = %d:\n", game->n_players, game->turn);
	for (i = 0; i < game->n_players; i++)
		{
			loc = player_get_zone (game->players[i]);
			player_print (game->players[i]);
			printf ("   Located in space: %ld%s\n", loc, (i == game->turn) ? "  <-- ACTIVE" : "");
		}

	/* Spaces */
	printf ("\n=> Spaces (%d):\n", game->n_spaces);
	for (i = 0; i < game->n_spaces; i++) { space_print (game->spaces[i]); }

	/* Objects */
	printf ("\n=> Objects (%d):\n", game->n_objects);
	for (i = 0; i < game->n_objects; i++)
		{
			loc = game_get_object_location (game, obj_get_id (game->objects[i]));
			obj_print (game->objects[i]);
			if (loc != NO_ID) printf ("   Located in space: %ld\n", loc);
			else printf ("   In player inventory\n");
		}

	/* Characters */
	printf ("\n=> Characters (%d):\n", game->n_numens);
	for (i = 0; i < game->n_numens; i++)
		{
			loc = game_get_numen_location (game, numen_get_id (game->numens[i]));
			numen_print (game->numens[i]);
			if (loc != NO_ID) printf ("   Located in space: %ld\n", loc);
		}

	/* Links */
	printf ("\n=> Links (%d):\n", game->n_links);
	for (i = 0; i < game->n_links; i++) { link_print (game->links[i], stdout); }

	printf ("\n=> Finished: %s\n", game->finished == TRUE ? "YES" : "NO");
	printf ("====================================\n");
}

Id
game_get_numen_location (Game* game, Id numen_id)
{
	int i;
	if (!game) return NO_ID;
	for (i = 0; i < game->n_spaces; i++)
		{
			if (game->spaces[i] && space_contains_numen (game->spaces[i], numen_id)) return space_get_id (game->spaces[i]);
		}
	return NO_ID;
}

Status
game_add_character (Game* game, Character* character)
{
	(void)character;
	if (!game) return ERROR;
	return OK;
}
