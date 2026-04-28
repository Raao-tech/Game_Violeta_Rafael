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
#include "command.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>


#define CMD_LENGHT 30

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
    CommandCode accion;
    char*       target;
    Direction   dir;
    Bool        is_command;
};

/**
 * ========================================================================
 *                      FUNCTIONS PRIVATE
 * ========================================================================
 */

Status		_command_set_target (Command* command, char* target);
Status		_command_set_direction (Command* command, Direction dir);
Direction	_command_get_direction (Command* command);
Status		_command_set_is_command (Command* command, Bool is_command);
Bool		_command_get_is_command (Command* command);



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
Status
command_raylib_get_user_input (Command* command)
{
    if (!command) return ERROR;

    if (IsKeyPressed (KEY_ESCAPE))
    {
        _command_set_is_command(command, TRUE);
		if(command_set_code (command, EXIT);)
        return 
    }
    if (IsKeyPressed (KEY_UP))      return command_set_code (command, WALK); _command_set_target(command, "");
    if (IsKeyPressed (KEY_ESCAPE))  return command_set_code (command, EXIT);
    if (IsKeyPressed (KEY_ESCAPE))  return command_set_code (command, EXIT);
    if (IsKeyPressed (KEY_ESCAPE))  return command_set_code (command, EXIT);

    /*
     *   if (IsKeyPressed(KEY_UP)    || IsKeyPressed(KEY_W)) -> WALK + N
     *   if (IsKeyPressed(KEY_DOWN)  || IsKeyPressed(KEY_S)) -> WALK + S
     *   if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) -> WALK + E
     *   if (IsKeyPressed(KEY_LEFT)  || IsKeyPressed(KEY_A)) -> WALK + W
     *   if (IsKeyPressed(KEY_E))                            -> MOVE (interactuar)
     *   if (IsKeyPressed(KEY_ONE..FOUR))                    -> ATTACK + skill_id
     *   En todos los casos: command_set_has_input(command, TRUE).
     */

    return OK;
}







/* ----------------------------------------------------------------------
 * Create / Destroy
 * ---------------------------------------------------------------------- */

Command*  command_create()
{
    Command* new_command = (Command*) malloc(sizeof(Command));
    if(!new_command) return NULL;

    new_command->accion =       NO_CMD;
    new_command->dir    =       U;
    new_command->is_command =   FALSE;
    new_command->target =       NULL;

    return new_command;
}
Status command_destroy (Command* command)
{
    if (!command) return ERROR;

    if (command->target) free (command->target);
    free (command);
    return OK;
}
/* ----------------------------------------------------------------------
 * (Set / Get) Code or Accion
* ---------------------------------------------------------------------- */

Status  command_set_code (Command* command, CommandCode code)
{
    if (!command) return ERROR;
    command->accion = code;
    return OK;
}

CommandCode command_get_code (Command* command)
{
    if (!command) return NO_CMD;
    return command->accion;
}
/* ----------------------------------------------------------------------
 * (_Set / Get) Target
* ---------------------------------------------------------------------- */


Status   _command_set_target (Command* command, char* target)
{
    int lentgh_target;
    if (!command || !target) return ERROR;
    lentgh_target = strlen(target);

    command->target = (char*) calloc(lentgh_target+1, sizeof(char));
    if(!target) return ERROR;

    strncpy(command->target, target, lentgh_target);
    command->target[lentgh_target] = '\0';

    return OK;
}

char*   command_get_target (Command* command)
{
    if (!command) return NULL;
    return command->target;
}

/* ----------------------------------------------------------------------
 * (_Set / Get) Directions
* ---------------------------------------------------------------------- */

Status  _command_set_direction (Command* command, Direction dir)
{
    if (!command) return ERROR;
    command->dir = dir;
    return OK;
}

Direction _command_get_direction (Command* command)
{
    if (!command) return U;
    return command->dir;
}


/* ----------------------------------------------------------------------
 * (_Set / Get) Is_command
* ---------------------------------------------------------------------- */

Status  _command_set_is_command (Command* command, Bool is_command)
{
    if (!command) return ERROR;
    command->is_command = is_command;
    return OK;
}

Bool _command_get_is_command (Command* command)
{
    if (!command) return FALSE;
    return command->is_command;
}






/* ----------------------------------------------------------------------
 * command_get_user_input  (modo test: lee de stdin)
 *
 * Lee una linea de stdin con formato "<cmd> [<target>]" (ej. "move north",
 * "take sword", "walk n"). Rellena el Command con el codigo y el target.
 *
 * Si fgets devuelve NULL (EOF, p.ej. al terminar la redireccion del .cmd),
 * tratamos la condicion como un EXIT implicito.
 * ---------------------------------------------------------------------- */
Status command_get_user_input (Command* command)
{
    char input[CMD_LENGHT] = "";
    char* token            = NULL;
    int i                  = UNKNOWN - NO_CMD + 1; /* = 2 */
    CommandCode cmd;

    if (!command) return ERROR;

    /* Limpiamos el target del turno anterior antes de leer el nuevo */
    if (command->target)
        {
            free (command->target);
            command->target = NULL;
        }

    if (fgets (input, CMD_LENGHT, stdin))
        {
            /* Primer token: el comando */
            token = strtok (input, " \n");
            if (!token) return command_set_code (command, UNKNOWN);

            cmd = UNKNOWN;
            while (cmd == UNKNOWN && i < N_CMD)
                {
                    if (!strcasecmp (token, cmd_to_str[i][CMDS]) || !strcasecmp (token, cmd_to_str[i][CMDL]))
                        {
                            cmd = i + NO_CMD;
                        }
                    else
                        {
                            i++;
                        }
                }

            /* Segundo token: el target (opcional segun el comando) */
            token = strtok (NULL, " \n");
            if (token) command->target = strdup (token);

            return command_set_code (command, cmd);
        }

    /* fgets fallo (EOF tipicamente): salida implicita */
    return command_set_code (command, EXIT);
}