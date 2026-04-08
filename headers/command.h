/**
 * @brief It defines the command interpreter interface
 *
 * @file command.h
 * @author Profesores PPROG
 * @version 2
 * @date 27-01-2025
 * @copyright GNU Public License
 */

#ifndef COMMAND_H
#define COMMAND_H

#include "types.h"

#define N_CMDT 2
#define N_CMD 11

typedef enum {
  CMDS,
  CMDL
} CommandType;

typedef enum {
  NO_CMD = -1,
  UNKNOWN,
  EXIT,
  NEXT,
  LEFT,
  RIGHT,
  BACK,
  TAKE,
  DROP,
  ATTACK,
  CHAT,
  INSPECT
} CommandCode;

typedef struct _Command Command;

Command *command_create();
Status command_destroy(Command *command);
Status command_set_code(Command *command, CommandCode code);
CommandCode command_get_code(Command *command);
Status command_get_user_input(Command *command);

/**
 * @brief It gets the object/target name from the last command (e.g. "sword" from "take sword")
 * @author Violeta y Rafa
 *
 * @param command a pointer to the command
 * @return pointer to the object name string, or NULL if none
 */
char *command_get_obj(Command *command);

#endif
