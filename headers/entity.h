/**
 * @brief It defines the character module interface
 *
 * @file character.h
 * @author Violeta y Rafa
 * @version 0
 * @date 27-01-2025
 * @copyright GNU Public License
 */

#ifndef ENTITY_H
#define ENTITY_H

#include "types.h"

typedef struct _Entity Entity;

/**
 * @brief It creates a new character, allocating memory and initializing it
 * @author Violeta y Rafa
 *
 * @param id the identification number for the new character
 * @param name the name for the new character
 * @return a new character, initialized
 */
Entity *entity_create();


#endif