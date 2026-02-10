/**
 * @brief It defines the textual graphic engine interface
 *
 * @file graphic_engine.h
 * @author Profesores PPROG
 * @version 0
 * @date 27-01-2025
 * @copyright GNU Public License
 */

#ifndef GRAPHIC_ENGINE_H
#define GRAPHIC_ENGINE_H

#include "game.h"

typedef struct _Graphic_engine Graphic_engine;

Graphic_engine *graphic_engine_create();

void graphic_engine_destroy(Graphic_engine *ge);

void graphic_engine_paint_game(Graphic_engine *ge, Game *game);

#endif
