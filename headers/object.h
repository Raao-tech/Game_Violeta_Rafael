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
 * @param id the identification number for the new object
 * @param name the name for the new object
 * @return a new object, initialized
 */
Object* obj_create(Id id, char* name);

/**
 * @brief It destroys an object, freeing the allocated memory
 * @author Violeta y Rafa
 *
 * @param obj a pointer to the object
 * @return OK, if everything goes well, or ERROR if there was some mistake
 */
Status obj_destroy(Object* obj);

/**
 * @brief It sets the name of an object
 * @author Violeta y Rafa
 *
 * @param obj a pointer to the object
 * @param name the name of the object
 * @return OK, if everything goes well, or ERROR if there was some mistake
 */
Status obj_set_name(Object* obj, char* name);

/**
 * @brief It gets the name of an object
 * @author Violeta y Rafa
 *
 * @param obj a pointer to the object
 * @return a string with the name of the object
 */
char* obj_get_name(Object* obj);

int   obj_get_id(Object* obj);

Status obj_print_name(Object* obj);

Status obj_print_id(Object* obj);


/**
 * @brief It prints the object information
 * @author Violeta y Rafa
 *
 * @param obj a pointer to the object
 * @return OK, if everything goes well, or ERROR if there was some mistake
 */
Status obj_print(Object* obj);






#endif