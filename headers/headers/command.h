/**
 * @brief It defines the command interpreter interface
 *
 * @file command.h
 * @author Profesores PPROG, Rafael, Javier, Salvador and Violeta
 * @version 5
 * @date 18-04-2025
 * @copyright GNU Public License
 */

#ifndef COMMAND_H
#define COMMAND_H

#include "types.h"

#define N_CMDT 2
#define N_CMD 16

typedef enum
{
    CMDS, /*<! Forma corta: "m", "w", ... */
    CMDL  /*<! Forma larga: "Move", "Walk", ... */
} CommandType;

typedef enum
{
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
    SAVE,
    LOAD,
    RECRUIT,
    KICK
} CommandCode;

typedef struct _Command Command;

/*========== Create / Destroy ===========*/
/**
 * @brief It creates a new command, allocating memory and initializing it
 * @author Rafael, Javier, Salvador and Violeta
 *
 * @return a new command, initialized, or NULL if memory fails
 */
Command* command_create (void);
/**
 * @brief It destroys a character, freeing the allocated memory
 * @author Rafael, Javier, Salvador and Violeta
 *
 * @param character a pointer to the character
 * @return OK, if everything goes well, or ERROR if there was some mistake
 */
Status command_destroy (Command* command);

/*========== Set / Get Code ===========*/
/**
 * @brief It sets the code of a command
 * @author Rafael, Javier, Salvador and Violeta
 *
 * @param command a pointer to the command
 * @param  code the new code
 * @return OK, if everything goes well, or ERROR if there was some mistake
 */
Status command_set_code (Command* command, CommandCode code);
/**
 * @brief It gets the code of a command
 * @author Rafael, Javier, Salvador and Violeta
 *
 * @param command a pointer to the command
 * @return the command code, or NO_CMD if command is NULL
 */
CommandCode command_get_code (Command* command);

/*========== Target ===========*/
/**
 * @brief It gets the target of a command
 * @author Rafael, Javier, Salvador and Violeta
 *
 * @param command a pointer to the command
 * @return the command taget, or NULL if command is NULL
 */
char* command_get_target (Command* command);

/*========== User Input - modo test (stdin) ===========*/
/**
 * @brief It gets the user input to the strdin 
 * @author Rafael, Javier, Salvador and Violeta
 *
 * @note The funtion will be used when the test mode is active, it is recomended use a cmd archive.
 * 
 * @param command a pointer to the command
 * @return OK, if everything goes well, or ERROR if there was some mistake
 */
Status command_get_user_input (Command* command);

/*========== User Input - modo visual (Raylib) ===========*/
/**
 * @brief It gets the user input to the kayboard
 * @author Rafael, Javier, Salvador and Violeta
 *
 * @note The funtion will be used when the test mode is in user mode, it is recomended not to use a cmd archive.
 * 
 * @param command a pointer to the command
 * @return void
 */
void command_raylib_get_user_input (Command* command);

#endif