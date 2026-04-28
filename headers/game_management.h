/**
 * @brief It reads information from files to define the game
 *
 * @file game_reader.h
 * @author Profesores PPROG, Violeta, Rafa y Salvador
 * @version 3
 * @date 20-04-2025
 * @copyright GNU Public License
 */

#ifndef GAME_MANAGEMENT_H
#define GAME_MANAGEMENT_H

#include "game.h"

/**
 * @brief It creates a game and loads all data from a file
 * @author Violeta y Rafa
 *
 * @param game pointer to pointer to the game (will be allocated)
 * @param filename string with the name of the data file
 * @return OK, if everything goes well, or ERROR if there was some mistake
 */
Status game_management_create_from_file (Game** game, char* filename);

/**
 * @brief It saves the current game in a new .dat
 * @author Salvador
 *
 * @param game pointer to pointer to the game
 * @return OK, if everything goes well, or ERROR if there was some mistake
 */
Status game_management_save_file (Game** game);

#endif
