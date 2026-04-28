/**
 * @brief It defines the object module interface
 *
 * @file object.h
 * @author Violeta, Rafa and Salvador
 * @version 3
 * @date 28-04-2025
 * @copyright GNU Public License
 */
#ifndef OBJECT_H
#define OBJECT_H

#include "command.h"
#include "entity.h"
#include "player.h"
#include "space.h"
#include "types.h"

typedef struct _Object Object;

/**
 * @brief It creates a new object, allocating memory and initializing it
 * @author Violeta y Rafa
 *
 * @return a new object, initialized
 */
Object* obj_create ();

/**
 * @brief It destroys an object, freeing the allocated memory
 * @author Violeta y Rafa
 *
 * @param obj a pointer to the object
 * @return OK, if everything goes well, or ERROR if there was some mistake
 */
Status obj_destroy (Object* obj);

/* =============Name============= */
/**
 * @brief It sets the name of an object
 * @author Violeta y Rafa
 *
 * @param obj a pointer to the object
 * @param name the name of the object
 * @return OK, if everything goes well, or ERROR if there was some mistake
 */
Status obj_set_name (Object* obj, char* name);

/**
 * @brief It checks whether the object has the given name
 * @author Violeta y Rafa
 *
 * @param obj a pointer to the object
 * @param name the name to compare against
 * @return TRUE if the names match, FALSE otherwise or if any parameter is NULL
 */
Bool obj_has_name (Object* obj, char* name);

/**
 * @brief It gets the name of an object
 * @author Violeta y Rafa
 *
 * @param obj a pointer to the object
 * @return a string with the name of the object
 */
char* obj_get_name (Object* obj);
/**
 * @brief It sets the description of an object
 * @author Javier Jarque
 *
 * @param obj a pointer to the object
 * @param description the description of the object
 * @return OK, if everything goes well, or ERROR if there was some mistake
 */
Status obj_set_description (Object* obj, char* description);
/**
 * @brief It checks whether the object has the given description
 * @author Javier Jarque
 *
 * @param obj a pointer to the object
 * @param description the description to compare against
 * @return TRUE if the descriptions match, FALSE otherwise or if any parameter is NULL
 */
Bool obj_has_description (Object* obj, char* description);
/**
 * @brief It gets the description of an object
 * @author Javier Jarque
 *
 * @param obj a pointer to the object
 * @return a string with the description of the object
 */
char* obj_get_description (Object* obj);

/* =============ID============= */
/**
 * @brief It sets the id of an object
 * @author Violeta y Rafa
 *
 * @param obj a pointer to the object
 * @param id the id to set
 * @return OK  if all good, ERROR if pointer is NULL or other case not good
 */
Status obj_set_id (Object* obj, Id id);
/**
 * @brief It gets the id of an object
 * @author Violeta y Rafa
 *
 * @param obj a pointer to the object
 * @return the id of object
 */
Id obj_get_id (Object* obj);

/* ========== Gdesc ========== */
/**
 * @brief It sets the graphic description of an object
 * @author Salvador
 *
 * @param obj a pointer to the object
 * @param gdesc the graphic description to set
 * @return OK  if all good, ERROR if pointer is NULL or other case not good
 */
Status obj_set_gdesc (Object* obj, char* gdesc);

/**
 * @brief It gets the graphic description of an object
 * @author Salvador
 *
 * @param obj a pointer to the object
 * @return a string with the graphic description of the object
 */
char* obj_get_gdesc (Object* obj);

/* ========== Position ========== */
/**
 * @brief It sets the position x and y of the object
 * @author Rafael and Violeta
 *
 * @param obj a pointer to the object
 * @param x x position to be set
 * @param y y position to be set
 * @return OK if position set properly, ERROR otherwise
 */
Status obj_set_position (Object* obj, int x, int y);

/**
 * @brief It gets the position x of the object
 * @author Rafael and Violeta
 *
 * @param obj a pointer to the object
 * @return position x of the object, -1 otherwise
 */
int obj_get_pos_x (Object* obj);

/**
 * @brief It gets the position y of the object
 * @author Rafael and Violeta
 *
 * @param obj a pointer to the object
 * @return position y of the object, -1 otherwise
 */
int obj_get_pos_y (Object* obj);

/* =============Health============= */
/**
 * @brief It sets the health of an object
 * @author Javier Jarque
 *
 * @param obj a pointer to the object
 * @param health the health to set
 * @return OK  if all good, ERROR if pointer is NULL or other case not good
 */
Status obj_set_health (Object* obj, int health);

int obj_get_health (Object* obj);

/**
 * @brief It gets the health of an object
 * @author Salvador
 *
 * @param obj a pointer to the object
 * @return the health of the object, 0 in case of mistake
 */
int obj_get_health (Object* obj);

/* =============Movable============= */
/**
 * @brief It sets the boolean movable of an object
 * @author Salvador
 *
 * @param obj a pointer to the object
 * @param movable the movable value to set
 * @return OK  if all good, ERROR if pointer is NULL or other case not good
 */
Status obj_set_movable (Object* obj, Bool movable);
/**
 * @brief It gets the movable status of an object
 * @author Salvador
 *
 * @param obj a pointer to the object
 * @return the movable status of the object, FALSE in case of mistake
 */
Bool obj_get_movable (Object* obj);

/* =============Open============= */
/**
 * @brief It sets the Id of the link that this object can open
 * @author Salvador
 *
 * @param obj a pointer to the object
 * @param open the Id of the link to set
 * @return OK  if all good, ERROR if pointer is NULL or other case not good
 */
Status obj_set_open (Object* obj, Id open);

/**
 * @brief It gets the Id of the link that this object can open
 * @author Salvador
 *
 * @param obj a pointer to the object
 * @return the Id of the link that this object can open, NO_ID in case of mistake
 */
Id obj_get_open (Object* obj);

/* =============Dependency============= */

/**
 * @brief It sets the Id of the object that this object depends on
 * @author Salvador
 *
 * @param obj a pointer to the object
 * @param dependency the Id of the dependent object to set
 * @return OK  if all good, ERROR if pointer is NULL or other case not good
 */
Status obj_set_dependency (Object* obj, Id dependency);

/**
 * @brief It gets the Id of the object that this object depends on
 * @author Salvador
 *
 * @param obj a pointer to the object
 * @return the Id of the object that this object depends on, NO_ID in case of mistake
 */
Id obj_get_dependency (Object* obj);

/**
 * @brief It gets the int of the stats
 * @author Salvador
 *
 * @param obj a pointer to the object
 * @return the Id of the object that this object depends on, NO_ID in case of mistake
 */
Status obj_set_stats (Object* obj, int speed, int health, int energy, int attack);

Status obj_set_health (Object* obj, int health);
int obj_get_health (Object* obj);

Status obj_set_speed (Object* obj, int speed);
int obj_get_speed (Object* obj);

Status obj_set_attack (Object* obj, int attack);
int obj_get_attack (Object* obj);

Status obj_set_energy (Object* obj, int energy);
int obj_get_energy (Object* obj);

/* ========== Consumable ========== */
Bool obj_get_consumable (Object* obj);
Status obj_set_consumable (Object* obj, Bool consumable);

/**
 * @brief It prints the object information
 * @author Violeta y Rafa
 *
 * @param obj a pointer to the object
 * @return OK, if everything goes well, or ERROR if there was some mistake
 */
Status obj_print (Object* obj);

#endif