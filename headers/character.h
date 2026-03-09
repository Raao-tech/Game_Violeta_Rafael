/**
 * @brief It defines the character module interface
 *
 * @file character.h
 * @author Violeta y Rafa
 * @version 0
 * @date 27-01-2025
 * @copyright GNU Public License
 */

#ifndef CHARACTER_H
#define CHARACTER_H

#include "entity.h"


typedef struct _Character Character;

/**
 * @brief It creates a new character, allocating memory and initializing it
 * @author Violeta y Rafa
 *
 * @param id the identification number for the new character
 * @param name the name for the new character
 * @return a new character, initialized
 */
Character *character_create();

/**
 * @brief It destroys a character, freeing the allocated memory
 * @author Violeta y Rafa
 *
 * @param character a pointer to the character
 * @return OK, if everything goes well, or ERROR if there was some mistake
 */
Status character_destroy(Character *character);

/**
 * @brief It sets the name of a character
 * @author Violeta y Rafa
 *
 * @param character a pointer to the character
 * @param name the name of the character
 * @return OK, if everything goes well, or ERROR if there was some mistake
 */
Status character_set_name(Character *character, char *name);

/**
 * @brief It sets the relationship status of a character
 * @author Violeta y Rafa
 *
 * @param character a pointer to the character
 * @param value boolean value to see if it's friendly or unfriendly
 * @return OK, if everything goes well, or ERROR if there was some mistake
 */
Status character_set_friendly(Character *character, Bool value);


/**
 * @brief It gets the name of a character
 * @author Violeta y Rafa
 *
 * @param character a pointer to the character
 * @return a string with the name of the character
 */
char *character_get_name(Character *character);


/**
 * @brief It gets the relationship status of a character
 * @author Violeta y Rafa
 *
 * @param character a pointer to the character
 * @return the boolean value that shows if a character is frindly or unfriendly
 */
Bool character_get_friendly(Character *character);


#endif