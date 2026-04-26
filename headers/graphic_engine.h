/**
 * @brief It defines the textual graphic engine interface
 *
 * @file graphic_engine_raylib.h
 * @author Rafael
 * @version 0
 * @date 21-04-2026
 * @copyright GNU Public License
 */

#ifndef GRAPHIC_ENGINE_H
#define GRAPHIC_ENGINE_H

#include "game.h"


/* --- Márgenes del área de juego --- */
#define POS_X_INIT    0
#define POS_Y_INIT    0
#define MARGIN_WIDHT  125
#define MARGIN_HIGHT  100

typedef struct _Graphic_engine Graphic_engine;


typedef enum
{
	OUT_ERR = -1,
    NEW_GAME,
    LOAD_GAME,
    EXIT
}Out;

typedef struct _MenuResult 
{
    Out     menu_out;
    char    data_name[WORD_SIZE+1];
    Id      init_numen;
}MenuResult;

Graphic_engine *graphic_engine_create(void);
void            graphic_engine_destroy(Graphic_engine *ge);

MenuResult		graphic_engine_init(Graphic_engine *ge);
void            graphic_engine_paint_game(Graphic_engine *ge, Game *game);


#endif