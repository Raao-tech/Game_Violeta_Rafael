/**
 * @brief It implements the command interpreter
 *
 * @file command.c
 * @author Profesores PPROG
 * @version 0
 * @date 24-01-2026
 * @copyright GNU Public License
 */

#include "command.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#define CMD_LENGHT 30

char *cmd_to_str[N_CMD][N_CMDT] = {{"", "No command"}, {"", "Unknown"}, {"e", "Exit"}, {"n", "Next"}, {"b", "Back"}, {"t", "Take"}, {"g", "Get"}};

/**
 * @brief Command
 *
 * This struct stores all the information related to a command.
 */
struct _Command
{
  CommandCode code; /*!< Name of the command */
};

/** space_create allocates memory for a new space
 *  and initializes its members
 */
Command *command_create()
{
  Command *newCommand = NULL;

  newCommand = (Command *)calloc(1, sizeof(Command));
  if (newCommand == NULL)
  {
    return NULL;
  }

  /* Initialization of an empty command*/
  newCommand->code = NO_CMD;

  return newCommand;
}

Status command_destroy(Command *command)
{
  if (!command)
  {
    return ERROR;
  }

  free(command);
  command = NULL;
  return OK;
}

Status command_set_code(Command *command, CommandCode code)
{
  if (!command)
  {
    return ERROR;
  }

  command->code = code;

  return OK;
}


CommandCode command_get_code(Command *command)
{
  if (!command)
  {
    return NO_CMD;
  }
  return command->code;
}

Status command_get_user_input(Command *command)
{
  char input[CMD_LENGHT] = "", *token = NULL;
  int i = UNKNOWN - NO_CMD + 1;
  CommandCode cmd;

  if (!command)
  {
    return ERROR;
  }

  if (fgets(input, CMD_LENGHT, stdin))
  {
    token = strtok(input, " \n");
    if (!token)
    {
      return command_set_code(command, UNKNOWN);
    }

    cmd = UNKNOWN;
    while (cmd == UNKNOWN && i < N_CMD)
    {
      if (!strcasecmp(token, cmd_to_str[i][CMDS]) || !strcasecmp(token, cmd_to_str[i][CMDL]))
      {
        cmd = i + NO_CMD;
      }
      else
      {
        i++;
      }
    }
    return command_set_code(command, cmd);
  }
  else
    return command_set_code(command, EXIT);
}
