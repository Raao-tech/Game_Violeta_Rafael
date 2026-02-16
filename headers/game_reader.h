/**
 * @brief       It reads information from files to define the game's interface
 *
 * @file        game_reader.h
 * @author      Profesores PPROG
 * @version     0
 * @date        27-01-2025
 * @copyright   GNU Public License
 */


#ifndef GAME_READER_H
#define GAME_READER_H

#include "game.h"

/**
 * @brief It reads the file's information and creates the map
 * @author Violeta y Rafa
 *
 * @param game a pointer to the game
 * @param filename string with the name of the file
 * @return OK, if everything goes well, or ERROR if there was some mistake
 */
Status game_create_from_file(Game *game, char *filename);

#endif