/**
 * @brief It defines common types for the whole project
 *
 * @file types.h
 * @author Profesores PROG, Violeta, Rafael and Salvador
 * @version 3.0
 * @date 23-4-2026
 * @copyright GNU Public License
 */


#ifndef TYPES_H
#define TYPES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Consts to numbers */
#define WORD_SIZE 1000
#define NO_ID -1
#define ERROR_MAIN -1
#define ERROR_GAME -1

/* Colors to prints */
#define ROJO        "\033[0;31m"
#define VERDE       "\033[0;32m"
#define AMARILLO    "\033[0;33m"
#define AZUL        "\033[0;34m"
#define MORADO      "\033[0;35m"
#define RESET       "\033[0m"

typedef long Id;

typedef enum
{
    FALSE,
    TRUE
} Bool;

typedef enum
{
    ERROR,
    OK,
    ERROR_Attack,
    ERROR_Chat,
    ERROR_dir,
    ERROR_take,
    ERROR_drop,
    FULL_OF_LOVE,
    ERROR_inspect
} Status;

typedef enum
{
    N,
    S,
    E,
    W,
    U /*Unknown*/
} Direction;

#endif
