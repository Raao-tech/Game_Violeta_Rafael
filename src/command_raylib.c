/**
 * @brief It implements the command interpreter for visual mode (Raylib)
 *
 * @file command_raylib.c
 * @author Rafael
 * @version 2
 * @date 24-01-2026
 * @copyright GNU Public License
 */
#include "command.h"
#include "raylib.h"




#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>


#define CMD_LENGHT 30

#define	Nc	"N"
#define	Sc	"S"
#define	Ec	"E"
#define	Wc	"W"
#define	WAIT	0.2

/* ----------------------------------------------------------------------
 * Tabla de comandos
 *
 * Correspondencia con CommandCode:
 *   indice de tabla = valor de enum + 1
 *   (porque NO_CMD = -1 ocupa la fila 0)
 *
 * ---------------------------------------------------------------------- */
struct _Command
{
	CommandCode code;
	char* target;
};

/**
 * ========================================================================
 *                      FUNCTIONS PRIVATE
 * ========================================================================
 */

Status _command_set_target (Command* command, char* target);
Status _command_set_direction (Command* command, Direction dir);
Direction _command_get_direction (Command* command);
Status _command_set_is_command (Command* command, Bool is_command);
Bool _command_get_is_command (Command* command);

/* ----------------------------------------------------------------------
 * command_raylib_get_user_input  (modo visual: traduce teclas a Command)
 *
 * STUB del Bloque 0. Solo capturamos ESC para que el usuario pueda
 * salir limpiamente del modo visual. La traduccion completa de
 * flechas/WASD a WALK + Direction, teclas 1-4 a ATTACK + skill_id,
 * y la deteccion de "tengo input este frame" llegara en el Bloque 1
 * cuando ampliemos Command con direction, skill_id y has_input.
 *
 * Reglas a recordar :
 *   - IsKeyPressed(KEY_X)  -> evento puntual (al pulsar la tecla una vez)
 *   - IsKeyDown(KEY_X)     -> mientras este pulsada (continuo)
 *   - GetKeyPressed()      -> devuelve int (keycode); util para texto,
 *                             NO para reconocer una tecla concreta.
 * ---------------------------------------------------------------------- */
void
command_raylib_get_user_input (Command* command)
{
	if (!command)	return;


	/*========== ========== EXIT ========== ============*/
	if (IsKeyPressed (KEY_ESCAPE))
		{
			command_set_code (command, EXIT);
			return;
		}

	/*========== ========== WAlK ========== ============*/
	if (IsKeyPressed (KEY_UP))
		{
			_command_set_target (command, Nc);
			command_set_code (command, WALK);
			return;
		}
	if (IsKeyPressed (KEY_DOWN))
		{
			_command_set_target (command, Sc);
			command_set_code (command, WALK);
			return;
		}
	if (IsKeyPressed (KEY_RIGHT))
		{
			_command_set_target (command, Ec);
			command_set_code (command, WALK);
			return;
		}
	if (IsKeyPressed (KEY_LEFT))
		{
			_command_set_target (command, Wc);
			command_set_code (command, WALK);
			return;
		}
	/*========== ========== TAKE ========== ============*/
	if (IsKeyPressed (KEY_T))
		{
			command_set_code (command, TAKE);
			return;
		}
	/*========== ========== DROP ========== ============*/
	if (IsKeyPressed (KEY_R))
		{
			command_set_code (command, DROP);
			return;
		}
	/*========== ========== ATTACK con skill 0..3 ========== ============*/
	/* Las teclas 1, 2, 3, 4 disparan el skill[0], skill[1], skill[2], skill[3]*/
	if (IsKeyPressed (KEY_ONE))
		{
			_command_set_target (command, "2");
			command_set_code (command, ATTACK);
			return;
		}
	if (IsKeyPressed (KEY_TWO))
		{
			_command_set_target (command, "3");
			command_set_code (command, ATTACK);
			return;
		}
	if (IsKeyPressed (KEY_THREE))
		{
			_command_set_target (command, "4");
			command_set_code (command, ATTACK);
			return;
		}
	if (IsKeyPressed (KEY_FOUR))
		{
			_command_set_target (command, "3");
			command_set_code (command, ATTACK);
			return;
		}

	













	/**
	* 	switch (cmd)
	* 	{
	*		case UNKNOWN: game_actions_unknown (game); break;
	*		case EXIT: game_actions_exit (game); break;
	*		case MOVE: game_actions_move (game); break; tiene que preguntarse adentro de walk no deberia de ser algo independietne
	*		case WALK: game_actions_walk (game); break;
	*		case TAKE: game_actions_take (game); break;
	*		case DROP: game_actions_drop (game); break;
	*		case ATTACK: game_actions_attack (game); break;
	*		case CHAT: game_actions_chat (game); break;
	*		case INSPECT: game_actions_inspect (game); break;
	*		case USE: game_actions_use (game); break;
	*		case OPEN: game_actions_open (game); break;
	*		case SAVE: game_actions_save (game); break;
	*		case LOAD: game_actions_load (game); break;
	*		case RECRUIT: game_actions_recruit (game); break;
	*		case KICK: game_actions_kick (game); break;
	*		default: break;
	*	}
	*/
	/*
	 *   if (IsKeyPressed(KEY_UP)    || IsKeyPressed(KEY_W)) -> WALK + N
	 *   if (IsKeyPressed(KEY_DOWN)  || IsKeyPressed(KEY_S)) -> WALK + S
	 *   if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) -> WALK + E
	 *   if (IsKeyPressed(KEY_LEFT)  || IsKeyPressed(KEY_A)) -> WALK + W
	 *   if (IsKeyPressed(KEY_E))                            -> MOVE (interactuar)
	 *   if (IsKeyPressed(KEY_ONE..FOUR))                    -> ATTACK + skill_id
	 *   En todos los casos: command_set_has_input(command, TRUE).
	 */

	return;
}
/* ----------------------------------------------------------------------
 * (Set / Get) Code or Accion
 * ---------------------------------------------------------------------- */


/* ----------------------------------------------------------------------
 * (_Set / Get) Target
 * ---------------------------------------------------------------------- */

Status
_command_set_target (Command* command, char* target)
{
	int lentgh_target;
	if (!command || !target) return ERROR;
	if (command->target)
		{
			free (command->target);
			command->target = NULL;
		}
	lentgh_target   = strlen (target);

	command->target = (char*)calloc (lentgh_target + 1, sizeof (char));
	if (!command->target) return ERROR;

	strncpy (command->target, target, lentgh_target);
	command->target[lentgh_target] = '\0';

	return OK;
}


/* ----------------------------------------------------------------------
 * (_Set / Get) Directions
 * ---------------------------------------------------------------------- */


/* ----------------------------------------------------------------------
 * (_Set / Get) Is_command
 * ---------------------------------------------------------------------- */
