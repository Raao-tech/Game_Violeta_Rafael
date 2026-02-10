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

#include "game.h"


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


Status obj_destroy(Object* obj);


Status obj_set_name(Object* obj, char* name);


const char* obj_get_name(Object* obj);


Status obj_print(Object* obj);

#endif