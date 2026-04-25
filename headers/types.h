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

/*Dimensiones de la pantalla del graphic Engine*/
#define WIDHT_SCREEN 950
#define HIGHT_SCREEN 650

/* Consts to numbers */
#define WORD_SIZE 1000
#define NO_ID -1
#define ERROR_MAIN -1
#define ERROR_GAME -1
#define ERROR_POSITION -9999

/*file of data for default*/
#define	F_DATA_N		"new_game.dat"
#define	F_DATA_S		"./Save_old_games/"

/*Name of game*/
#define	TITLE		"Atlantic Quest"
/*Number of FPS*/
#define	FPS	60



/* Colors to prints */
#define ROJO        "\033[0;31m"
#define VERDE       "\033[0;32m"
#define AMARILLO    "\033[0;33m"
#define AZUL        "\033[0;34m"
#define MORADO      "\033[0;35m"
#define RESET       "\033[0m"

/*<! Max_heald of skills*/
#define MAX_HELD_SKILLS 4
#define PLAYER 0

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
    ERROR_inspect,
    ERROR_use,
    ERROR_open,
    ERROR_save,
    ERROR_load,
    ERROR_recruit,
    ERROR_kick
} Status;

typedef enum
{
    N,
    S,
    E,
    W,
    U /*Unknown*/
} Direction;


/**
 * @brief Estadísticas de un entity
 */
typedef struct _Stats Stats;

/**
 * @brief Posición de un entity
 */
typedef struct _Position Position;

#define  MIN_LIFE 0
#define  MIN_ATTACK 0
#define  MIN_ENGY 0
#define  MAX_LIFE 10
#define  MAX_ENGY 100
#define  MAX_ATTACK 10
#define  ERROR_LIFE -1
#define  ERROR_ATTACK -1
#define  ERROR_ENGY -1


#endif
