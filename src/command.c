/**
 * @brief It implements the command interpreter (mode test, stdin)
 *
 * @file command.c
 * @author Profesores PPROG and Javier Jarque
 * @version 2
 * @date 24-01-2026
 * @copyright GNU Public License
 */

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
char *cmd_to_str[N_CMD][N_CMDT] = {
    {"",  "No command"},
    {"",  "Unknown"},
    {"e", "Exit"},
    {"m", "Move"},
    {"w", "Walk"},
    {"t", "Take"},
    {"d", "Drop"},
    {"a", "Attack"},
    {"c", "Chat"},
    {"i", "Inspect"},
    {"u", "Use"},
    {"o", "Open"},
    {"s", "Save"},
    {"l", "Load"},
    {"r", "Recruit"},
    {"k", "Kick"}
};

struct _Command {
    char       *target;
    CommandCode code;

    /* PlayerCode player ----> Posible solucion para multijugador simultaneo:
     *   typedef enum { NO_PLY, PLY_1, PLY_2 } PlayerCode;
     *   Permitiria identificar quien ejecuta cada comando, util para los
     *   tests de integracion automatizados (game1.cmd) y para el render
     *   por frame en graphic_engine.
     */
};

/* ----------------------------------------------------------------------
 * Create / Destroy
 * ---------------------------------------------------------------------- */

Command *command_create(void) {
    Command *newCommand = (Command *) calloc(1, sizeof(Command));
    if (!newCommand) return NULL;

    newCommand->code   = NO_CMD;
    newCommand->target = NULL;
    return newCommand;
}

Status command_destroy(Command *command) {
    if (!command) return ERROR;

    if (command->target) free(command->target);
    free(command);
    return OK;
}

/* ----------------------------------------------------------------------
 * Set / Get
 * ---------------------------------------------------------------------- */

Status command_set_code(Command *command, CommandCode code) {
    if (!command) return ERROR;
    command->code = code;
    return OK;
}

CommandCode command_get_code(Command *command) {
    if (!command) return NO_CMD;
    return command->code;
}

char *command_get_target(Command *command) {
    if (!command) return NULL;
    return command->target;
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
Status command_get_user_input(Command *command) {
    char        input[CMD_LENGHT] = "";
    char       *token             = NULL;
    int         i                 = UNKNOWN - NO_CMD + 1;  /* = 2 */
    CommandCode cmd;

    if (!command) return ERROR;

    /* Limpiamos el target del turno anterior antes de leer el nuevo */
    if (command->target) {
        free(command->target);
        command->target = NULL;
    }

    if (fgets(input, CMD_LENGHT, stdin)) {
        /* Primer token: el comando */
        token = strtok(input, " \n");
        if (!token) return command_set_code(command, UNKNOWN);

        cmd = UNKNOWN;
        while (cmd == UNKNOWN && i < N_CMD) {
            if (!strcasecmp(token, cmd_to_str[i][CMDS]) ||
                !strcasecmp(token, cmd_to_str[i][CMDL])) {
                cmd = i + NO_CMD;
            } else {
                i++;
            }
        }

        /* Segundo token: el target (opcional segun el comando) */
        token = strtok(NULL, " \n");
        if (token) command->target = strdup(token);

        return command_set_code(command, cmd);
    }

    /* fgets fallo (EOF tipicamente): salida implicita */
    return command_set_code(command, EXIT);
}