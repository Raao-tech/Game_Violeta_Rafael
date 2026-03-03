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

#include "types.h"
#include "object.h"

typedef struct _Character Character;

/**
 * @brief It creates a new character, allocating memory and initializing it
 * @author Violeta y Rafa
 *
 * @param id the identification number for the new character
 * @param name the name for the new character
 * @return a new character, initialized
 */
Character *character_create(Id id);

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
 * @brief It sets the id of the obcjet of the character
 * @author Violeta y Rafa
 *
 * @param character a pointer to the character
 * @param id_obj a Id of the object of the character
 * @return OK, if object is on the character, or ERROR if there  was not  some object
 */
Status character_set_obj(Character *character, Id id_obj);

/**
 * @brief It sets location of character
 * @author Violeta y Rafa
 *
 * @param character a pointer to the character
 * @param id_space a space's id of location of character
 * @return OK, if everything goes well, or ERROR if there was some mistake
 */
Status character_set_space(Character *character, Id id_space);

/**
 * @brief It gets the name of a character
 * @author Violeta y Rafa
 *
 * @param character a pointer to the character
 * @return a string with the name of the character
 */
char *character_get_name(Character *character);

/**
 * @brief It gets the id of the space where the character is
 * @author Violeta y Rafa
 *
 * @param character a pointer to the character
 * @return a id of the space of the character
 */
Id character_get_space(Character *character);

/**
 * @brief It gets the id of object of a character
 * @author Violeta y Rafa
 *
 * @param character a pointer to the character
 * @return id of the object on the character
 */
Id character_get_obj(Character *character);

/**
 * @brief It prints the character information
 * @author Violeta y Rafa
 *
 * @param character a pointer to the character
 * @return OK, if everything goes well, or ERROR if there was some mistake
 */
Status character_print(Character *character);

#endif