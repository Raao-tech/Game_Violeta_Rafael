/**
 * @brief It defines the textual graphic engine interface
 *
 * @file graphic_engine.h
 * @author Profesores PPROG, Javier Jarque, Rafael
 * @version 3
 * @date 08-04-2026
 * @copyright GNU Public License
 */

#ifndef GRAPHIC_ENGINE_H
#define GRAPHIC_ENGINE_H

#include "game.h"


#define POS_X_INIT 0
#define POS_Y_INIT 0
#define MARGIN_WIDHT 125
#define MARGIN_HIGHT 100
/**
 * @brief Graphic_engine ADT (opaque type)
 *
 * Contains pointers to the screen areas (map, descript, banner,
 * help, feedback) allocated by libscreen.
 */
typedef struct _Graphic_engine Graphic_engine;

/**
 * @brief It creates a graphic engine, allocating memory and initializing it
 * @author Profesores PPROG
 *
 * Calls screen_init to set up the terminal screen, then creates
 * all five areas (map, description, banner, help, feedback).
 *
 * @return a graphic engine, initialized, or NULL if something fails
 */
Graphic_engine *graphic_engine_create();

/**
 * @brief It destroys a graphic engine, freeing the allocated memory
 * @author Profesores PPROG
 *
 * Destroys all screen areas and calls screen_destroy.
 *
 * @param ge a pointer to the graphic engine
 */
void graphic_engine_destroy(Graphic_engine *ge);

/**
 * @brief It paints the full game state onto the terminal
 * @author Profesores PPROG, Javier Jarque, Rafael
 *
 * Renders the map (current space + neighbours via links),
 * the description panel (objects, characters, players, messages),
 * the banner (active player name/turn), the help bar, and the
 * feedback line with the last command result.
 *
 * Only shows full information (gdesc, objects, characters) for
 * spaces that have been discovered (F12).
 *
 * @param ge pointer to the graphic engine
 * @param game pointer to the game state
 */
void graphic_engine_paint_game(Graphic_engine *ge, Game *game);

/**
 * @brief It paints the  main menu of game
 * @author Rafael
 *
 * This show the main menu to the player with the option 
 *  New Game
 *  Load Game
 *  Exit 
 *
 * @param ge pointer to the graphic engine
 * @param game pointer to the game state
 */
void graphic_engine_menu_init(Graphic_engine *ge, Game *game);

#endif