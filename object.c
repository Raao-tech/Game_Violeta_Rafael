/**
 * @brief  It implements the object struct
 *
 * @file object.c
 * @author Violeta y Rafael
 * @version 0
 * @date 04-02-2025
 * @copyright GNU Public License
 */

#include "object.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief obj
 *
 * This struct stores all the information of a obj.
 */
struct _Object {
  Id id;                    /*!< Id number of the object, it must be unique */
  char name[WORD_SIZE + 1]; /*!< Name of the object */
};


Object* obj_create(Id id, char* name) {
  Object* newObj = NULL;

  /* Error control */
  if (id == NO_ID || !name) return NULL;

  newObj = (Object*)calloc(1, sizeof(Object));
  if (newObj == NULL) {
    return NULL;
  }

  /* Initialization of an empty obj*/
  newObj->id = id;
  strcpy(newObj->name, name);

  return newObj;
}

Status obj_destroy(Object* obj) {
  if (!obj) {
    return ERROR;
  }

  free(obj);
  return OK;
}


Status obj_set_name(Object* obj, char* name) {
  if (!obj || !name) {
    return ERROR;
  }

  if (!strcpy(obj->name, name)) {
    return ERROR;
  }
  return OK;
}

const char* obj_get_name(Object* obj) {
  if (!obj) {
    return NULL;
  }
  return obj->name;
}

Status obj_print(Object* obj) {
  /* Error Control */
  if (!obj || obj->id == NO_ID) {
    return ERROR;
  }

  /* 1. Print the id and the name of the space */
  fprintf(stdout, "--> Object (Id: %ld; Name: %s)\n", obj->id, obj->name);

  return OK;
}