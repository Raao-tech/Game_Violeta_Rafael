/**
 * @brief It implements the command interpreter
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

char *cmd_to_str[N_CMD][N_CMDT] = {
  {"", "No command"},
  {"", "Unknown"},
  {"e", "Exit"},
  {"m", "Move"},
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
  char *target;
  CommandCode code;
  /* PlayerCode player------> Es una posible  solución para nuestro multijugador simultaneo 7
  * typedef enum{            que nos digan constantemente quién está ejecutando el comando por
  *   NO_PLY,                cada frame (para graphic_engine_raylib()) y  por cada línea presente 
  *   PLY_1,                 en el game1.cmd (las pruebas de integración automatizadas).
  *   PLY_2
  * }PlayerCode;
  *                           Posiblers consescuencias: 
  *                  La máxima cantidad de jugadores, vendría definido por una macro en test.h o en el
  *                   propio command de la misma forma que se ecnuentra en CMD.  podemos establecerlo en 
  *                 el game->turn  = MAX_PLY para no cmabiar muchas lineas de codigo,pero todo estaría con
  *                   esta enumeracion de tipos de datos.
  *
  */
};

Command *command_create(){
  Command *newCommand = NULL;

  newCommand = (Command *)calloc(1, sizeof(Command));
  if (newCommand == NULL) return NULL;

  newCommand->code = NO_CMD;
  newCommand->target = NULL;
  return newCommand;
}

Status command_destroy(Command *command){
  if (!command) return ERROR;

  if (command->target) free(command->target);

  free(command);
  return OK;
}

Status command_set_code(Command *command, CommandCode code){
  if (!command) return ERROR;
  command->code = code;
  return OK;
}

CommandCode command_get_code(Command *command){
  if (!command) return NO_CMD;
  return command->code;
}
/*======= obtener la entidad señalada en la que se basa la accion (CommandCode)=============*/
char *command_get_target(Command *command){
  if (!command) return NULL;
  return command->target;
}



/*======= Eje central, Input del usuario =============*/
Status command_get_user_input(Command *command){
  char input[CMD_LENGHT] = "";
  char *token = NULL;
  int i = UNKNOWN - NO_CMD + 1;
  CommandCode cmd;

  if (!command) return ERROR;

  if (command->target) {
    free(command->target);
    command->target = NULL;
  }
/*Guardamos el comando escrito por el user en la terminal en un string llamado input (con 20 caracteres = CMD_LEGHT)*/
  if (fgets(input, CMD_LENGHT, stdin)){
    /* First token: the command itself */
    token = strtok(input, " \n");
    if (!token) return command_set_code(command, UNKNOWN);

    cmd = UNKNOWN;
    while (cmd == UNKNOWN && i < N_CMD){
      if (!strcasecmp(token, cmd_to_str[i][CMDS]) || !strcasecmp(token, cmd_to_str[i][CMDL])) cmd = i + NO_CMD;
      else  i++;
    }

    /* Second token: the target name (for take, drop, attack, chat, inspect, use, load, recruit, kick) */
    token = strtok(NULL, " \n");
    if (token)  command->target = strdup(token);
    return command_set_code(command, cmd);
  }
  else
    return command_set_code(command, EXIT);
}
