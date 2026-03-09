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

/**
 * @brief It destroys a entity, freeing the allocated memory
 * @author Violeta y Rafa
 *
 * @param entity a pointer to the entity
 * @return OK, if everything goes well, or ERROR if there was some mistake
 */
Status entity_destroy(Entity *entity);

/**
 * @brief It sets the name of a entity
 * @author Violeta y Rafa
 *
 * @param entity a pointer to the entity
 * @param name the name of the entity
 * @return OK, if everything goes well, or ERROR if there was some mistake
 */
Status entity_set_name(Entity *entity, char *name);


/**
 * @brief It gets the name of a entity
 * @author Violeta y Rafa
 *
 * @param entity a pointer to the entity
 * @return a string with the name of the entity
 */
char *entity_get_name(Entity *entity);


#endif