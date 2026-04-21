/**
 * @brief It defines the entity module interface
 *
 * @file entity.h
 * @author Violeta, Rafa y Salvador
 * @version 1
 * @date 24-03-2026
 * @copyright GNU Public License
 */

#ifndef ENTITY_H
#define ENTITY_H

#include "types.h"
#include "set.h"


/** 
*   @struct it Defines the entity (player, character)
*/
typedef struct _Entity Entity;

/**
 * @brief It creates a new entity, allocating memory and initializing it
 * @author Violeta
 *
 * @return a new entity, initialized
 */
Entity *entity_create();

/**
 * @brief It destroys an entity, freeing the allocated memory
 * @author Violeta
 *
 * @param entity a pointer to the entity
 * @return OK, if everything goes well, or ERROR if there was some mistake
 */
Status entity_destroy(Entity *entity);

/**
 * @brief It sets the id
 * @author Rafael
 *
 * @param entity a pointer to the entity
 * @param new_id a value of the new id
 * @return OK, if everything goes well, or ERROR if there was some mistake
 */
Status  entity_set_id(Entity *entity, Id new_id);

/**
 * @brief It return the id
 * @author Rafael
 *
 * @param entity a pointer to the entity
 * @return the entity's id
 */
Id  entity_get_id(Entity *entity);

/**
 * @brief It sets the name of an entity
 * @author Violeta
 *
 * @param entity a pointer to the entity
 * @param name the name of the entity
 * @return OK, if everything goes well, or ERROR if there was some mistake
 */
Status entity_set_name(Entity *entity, char *name);

/**
 * @brief It gets a copy of the name of an entity
 * @author Violeta
 *
 * @param entity a pointer to the entity
 * @return a copy of the name of the entity (the caller must free it)
 */
char *entity_get_name(Entity *entity);

/**
 * @brief It checks whether the entity has the given name
 * @author Salvador
 *
 * @param entity a pointer to the entity
 * @param name the name to compare against
 * @return TRUE if the names match, FALSE otherwise or if any parameter is NULL
 */
Bool entity_has_name(Entity *entity, char *name);

/**
 * @brief It sets the graphic description of an entity
 * @author Violeta
 *
 * @param entity a pointer to the entity
 * @param desc a string with the new graphic description
 * @return OK, if everything goes well, or ERROR if there was some mistake
 */
Status entity_set_gdesc(Entity *entity, char *desc);

/**
 * @brief It gets a copy of the graphic description of an entity
 * @author Violeta
 *
 * @param entity a pointer to the entity
 * @return a copy of the graphic description (the caller must free it), or NULL if error
 */
char *entity_get_gdesc(Entity *entity);

/**
 * @brief It sets the message of an entity
 * @author Violeta
 *
 * @param entity a pointer to the entity
 * @param message a string with the new message
 * @return OK, if everything goes well, or ERROR if there was some mistake
 */
Status entity_set_message(Entity *entity, char *message);

/**
 * @brief It gets a copy of the message of an entity
 * @author Violeta
 *
 * @param entity a pointer to the entity
 * @return a copy of the message (the caller must free it), or NULL if error
 */
char *entity_get_message(Entity *entity);



#endif
