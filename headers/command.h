/**
 * @brief It defines the command interpreter interface
 *
 * @file command.h
 * @author Profesores PPROG
 * @version 0
 * @date 27-01-2025
 * @copyright GNU Public License
 */

#ifndef COMMAND_H
#define COMMAND_H

#include "types.h"

#define N_CMDT 2
#define N_CMD 9

typedef enum
{
    CMDS,
    CMDL
} CommandType;

typedef enum
{
    NO_CMD = -1,
    UNKNOWN,
    EXIT,
    NEXT,
    LEFT,
    RIGHT,
    BACK,
    TAKE,
    DROP
} CommandCode;

typedef struct _Command Command;

/**
 * @brief It creates a new command, allocating memory and initializing it
 * @author Violeta y Rafa
 *
 * @return a new command, initialized
 */
Command *command_create();

/**
 * @brief It destroys a command, freeing the allocated memory
 * @author Violeta y Rafa
 *
 * @param command a pointer to the command
 * @return OK, if everything goes well, or ERROR if there was some mistake
 */
Status command_destroy(Command *command);

/**
 * @brief It sets the code of a command
 * @author Violeta y Rafa
 *
 * @param command a pointer to the command
 * @param code the code of the command
 * @return OK, if everything goes well, or ERROR if there was some mistake
 */
Status command_set_code(Command *command, CommandCode code);

/**
 * @brief It gets the code of an command
 * @author Violeta y Rafa
 *
 * @param command a pointer to the command
 * @return the command's code
 */
CommandCode command_get_code(Command *command);

/**
 * @brief It gets the user's input
 * @author Violeta y Rafa
 *
 * @param command a pointer to the command
 * @return OK, if everything goes well, or ERROR if there was some mistake
 */
Status command_get_user_input(Command *command);

#endif
