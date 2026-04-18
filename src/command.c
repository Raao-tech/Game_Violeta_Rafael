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
  {"i","Inspect"},
  {"u","Use"},
  {"o","Open"}
};

struct _Command {
  char *obj;
  CommandCode code;
};

Command *command_create(){
  Command *newCommand = NULL;

  newCommand = (Command *)calloc(1, sizeof(Command));
  if (newCommand == NULL) return NULL;

  newCommand->code = NO_CMD;
  newCommand->obj = NULL;
  return newCommand;
}

Status command_destroy(Command *command){
  if (!command) return ERROR;

  if (command->obj) free(command->obj);

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

char *command_get_obj(Command *command){
  if (!command) return NULL;
  return command->obj;
}

Status command_get_user_input(Command *command){
  char input[CMD_LENGHT] = "";
  char *token = NULL;
  int i = UNKNOWN - NO_CMD + 1;
  CommandCode cmd;

  if (!command) return ERROR;

  if (command->obj) {
    free(command->obj);
    command->obj = NULL;
  }

  if (fgets(input, CMD_LENGHT, stdin)){
    /* First token: the command itself */
    token = strtok(input, " \n");
    if (!token) return command_set_code(command, UNKNOWN);

    cmd = UNKNOWN;
    while (cmd == UNKNOWN && i < N_CMD){
      if (!strcasecmp(token, cmd_to_str[i][CMDS]) || !strcasecmp(token, cmd_to_str[i][CMDL])) cmd = i + NO_CMD;
      else  i++;
    }

    /* Second token: the object/target name (for take, drop, attack, chat, inspect, use) */
    token = strtok(NULL, " \n");
    if (token)  command->obj = strdup(token);
    return command_set_code(command, cmd);
  }
  else
    return command_set_code(command, EXIT);
}
