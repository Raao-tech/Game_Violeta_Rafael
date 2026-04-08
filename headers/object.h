/**
 * @brief It defines the object module interface
 *
 * @file object.h
 * @author Violeta y Rafa
 * @version 0
 * @date 04-02-2025
 * @copyright GNU Public License
 */
#ifndef OBJECT_H
#define OBJECT_H

#include "command.h"
#include "space.h"
#include "types.h"
#include "player.h"

typedef struct _Object Object;

/**
 * @brief It creates a new object, allocating memory and initializing it
 * @author Violeta y Rafa
 *
 * @return a new object, initialized
 */
Object *obj_create();

/**
 * @brief It destroys an object, freeing the allocated memory
 * @author Violeta y Rafa
 *
 * @param obj a pointer to the object
 * @return OK, if everything goes well, or ERROR if there was some mistake
 */
Status obj_destroy(Object *obj);

 /* =============Name============= */
/**
 * @brief It sets the name of an object
 * @author Violeta y Rafa
 *
 * @param obj a pointer to the object
 * @param name the name of the object
 * @return OK, if everything goes well, or ERROR if there was some mistake
 */
Status obj_set_name(Object *obj, char *name);

/**
 * @brief It checks whether the object has the given name
 * @author Violeta y Rafa
 *
 * @param obj a pointer to the object
 * @param name the name to compare against
 * @return TRUE if the names match, FALSE otherwise or if any parameter is NULL
 */
Bool obj_has_name(Object *obj, char *name);

/**
 * @brief It gets the name of an object
 * @author Violeta y Rafa
 *
 * @param obj a pointer to the object
 * @return a string with the name of the object
 */
char *obj_get_name(Object *obj);
/**
 * @brief It sets the description of an object
 * @author Javier Jarque
 *
 * @param obj a pointer to the object
 * @param description the description of the object
 * @return OK, if everything goes well, or ERROR if there was some mistake
 */
Status obj_set_description(Object *obj, char *description);
/**
 * @brief It checks whether the object has the given description
 * @author Javier Jarque
 *
 * @param obj a pointer to the object
 * @param description the description to compare against
 * @return TRUE if the descriptions match, FALSE otherwise or if any parameter is NULL
 */
Bool obj_has_description(Object *obj, char *description);
/**
 * @brief It gets the description of an object
 * @author Javier Jarque
 *
 * @param obj a pointer to the object
 * @return a string with the description of the object
 */
char *obj_get_description(Object *obj);

 /* =============ID============= */
/**
 * @brief It sets the id of an object
 * @author Violeta y Rafa
 *
 * @param obj a pointer to the object
 * @param id the id to set
 * @return OK  if all good, ERROR if pointer is NULL or other case not good
 */
 Status obj_set_id(Object *obj, Id id);
/**
 * @brief It gets the id of an object
 * @author Violeta y Rafa
 *
 * @param obj a pointer to the object
 * @return the id of object
 */
Id obj_get_id(Object *obj);

/**
 * @brief It prints the object information
 * @author Violeta y Rafa
 *
 * @param obj a pointer to the object
 * @return OK, if everything goes well, or ERROR if there was some mistake
 */
Status obj_print(Object *obj);

#endif