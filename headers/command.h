/**
 * @brief It defines the command interpreter interface
 *
 * @file command.h
 * @author Profesores PPROG, Rafael, Javier and Salvador
 * @version 5
 * @date 18-04-2025
 * @copyright GNU Public License
 */

#ifndef COMMAND_H
#define COMMAND_H

#include "types.h"

#define N_CMDT 2
#define N_CMD  16

typedef enum {
    CMDS,    /*<! Forma corta: "m", "w", ... */
    CMDL     /*<! Forma larga: "Move", "Walk", ... */
} CommandType;

typedef enum {
    NO_CMD = -1,
    UNKNOWN,
    EXIT,
    MOVE,
    WALK,
    TAKE,
    DROP,
    ATTACK,
    CHAT,
    INSPECT,
    USE,
    OPEN,
    SAVE,
    LOAD,
    RECRUIT,
    KICK
} CommandCode;

typedef struct _Command Command;

/*========== Create / Destroy ===========*/
Command    *command_create(void);
Status      command_destroy(Command *command);

/*========== Set / Get Code ===========*/
Status      command_set_code(Command *command, CommandCode code);
CommandCode command_get_code(Command *command);

/*========== Target ===========*/
char       *command_get_target(Command *command);

/*========== User Input - modo test (stdin) ===========*/
Status      command_get_user_input(Command *command);

/*========== User Input - modo visual (Raylib) ===========*/
Status      command_raylib_get_user_input(Command *command);

#endif