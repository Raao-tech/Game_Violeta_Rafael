/**
 * @brief It defines the screen interface
 *
 * @file libscreen.h
 * @author Profesores PPROG
 * @version 3.5
 * @date 26-01-2024
 * @copyright GNU Public License
 */

#ifndef LIBSCREEN_H
#define LIBSCREEN_H

typedef struct _Area Area;

/**
 * @brief It creates a new screen
 * @author Profesores PPROG
 *
 * This function should be called at the beginning of the program,
 *  so the complete screen is allocated before starting defining areas.
 * @param rows the number of rows that will have the full screen
 * @param columns the number of columns that will have the full screen
 */
void screen_init(int rows, int columns);

/**
 * @brief It destroys a new screen area
 * @author Profesores PPROG
 *
 * As it frees the screen, it must be called at the end of the program,
 *  once the areas created have been freed.
 */
void screen_destroy();

/**
 * @brief It paints in the terminal the actual screen composition
 * @author Profesores PPROG
 *
 * This function should be called when some updates
 *  in the screen want to be shown.
 */
void screen_paint();

/**
 * @brief It creates a new area inside a screen
 * @author Profesores PPROG
 *
 * screen_area_init allocates memory for a new area
 *  and initializes its members.
 * @param x the x-coordinate of the up-left corner of the area
 * @param y the x-coordinate of the up-left corner of the area
 * @param width the width of the area
 * @param height the height of the area
 * @return a new area, initialized
 */
Area* screen_area_init(int x, int y, int width, int height);

/**
 * @brief It destroys a new screen area
 * @author Profesores PPROG
 *
 * This function should be called once the area is not needed anymore,
 *  before ending the programme.
 * @param area the area to be freed
 */
void screen_area_destroy(Area* area);

/**
 * @brief It cleares an area, eraising all its content
 * @author Profesores PPROG
 *
 * This function should be called for earaising all the information in an area,
 *  before introducing a new state of it.
 * @param area the area to be cleared
 */
void screen_area_clear(Area* area);

/**
 * @brief It resets the cursor of an area
 * @author Profesores PPROG
 *
 * This function reset the cursor to the up-left corner of the area.
 * @param area the involved area
 */
void screen_area_reset_cursor(Area* area);

/**
 * @brief It introduces some information inside an area
 * @author Profesores PPROG
 *
 * This function sets the string that will be shown in an area.
 *  Each string introduced will be a line in the specified area.
 * @param area the area to be modified
 * @param str a string that contains the information to be included in a particular area
 */
void screen_area_puts(Area* area, char* str);

#endif
