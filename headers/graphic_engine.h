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


/**
 * @brief It creates a graphic engine, allocating memory and initializing it
 * @author Profesores PPROG
 *
 * @return a graphic engine, initialized
 */
Graphic_engine* graphic_engine_create();

/**
 * @brief It destroys a graphic engine, freeing the allocated memory
 * @author Profesores PPROG
 *
 * @param ge a pointer to the graphic engine
 */
void graphic_engine_destroy(Graphic_engine *ge);

/**
 * @brief It paints the game's graphics
 * @author Profesores PPROG
 * 
 * @param ge pointer to the graphic engine
 * @param game pointer to game
 */
void graphic_engine_paint_game(Graphic_engine *ge, Game *game);

#endif
