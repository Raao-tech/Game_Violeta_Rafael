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
struct _Object
{
  Id id;                    /*!< Id number of the object, it must be unique */
  char name[WORD_SIZE + 1]; /*!< Name of the object */
};

/* Functions to create or destroy */
Object *obj_create(Id id, char *name)
{
  Object *newObj = NULL;

  /* Error control */
  if (id == NO_ID || name == NULL)
  {
    return NULL;
  }

  newObj = (Object *)calloc(1, sizeof(Object));
  if (newObj == NULL)
  {
    return NULL;
  }

  /* Initialization of an empty obj*/
  newObj->id = id;
  strcpy(newObj->name, name);

  return newObj;
}

Status obj_destroy(Object *obj)
{
  if (obj == NULL)
  {
    return ERROR;
  }

  free(obj);
  return OK;
}

/* Functions set properties*/
Status obj_set_name(Object *obj, char *name)
{
  if (!obj || !name)
  {
    return ERROR;
  }

  if (strcpy(obj->name, name) == NULL)
  {
    return ERROR;
  }
  return OK;
}
Status obj_set_id(Object *obj, Id id)
{
  if (!obj || id == NO_ID)
  {
    return ERROR;
  }
  obj->id = id;

  return OK;
}

/* functios to gets info */
char *obj_get_name(Object *obj)
{
  if (!obj)
  {
    return NULL;
  }
  return obj->name;
}
Id obj_get_id(Object *obj)
{

  if (obj == NULL)
  {
    return NO_ID;
  }

  return obj->id;
}

/* functios to prints */
Status obj_print_name(Object *obj)
{
  /* Error Control */
  if (!obj || obj->name[0] == '\0')
  {
    return ERROR;
  }

  /* 1. Print the id and the name of the object */
  fprintf(stdout, "--> Object of Name : %s\n", obj->name);

  return OK;
}
Status obj_print_id(Object *obj)
{
  /* Error Control */
  if (!obj || obj->id == NO_ID)
  {
    return ERROR;
  }

  /* 1. Print the id and the name of the object */
  fprintf(stdout, "--> Object of ID : %ld\n", obj->id);

  return OK;
}
Status obj_print(Object *obj)
{
  /* Error Control */
  if (!obj || obj->id == NO_ID)
  {
    return ERROR;
  }

  /* 1. Print the id and the name of the object */
  fprintf(stdout, "--> Object (Id: %ld; Name: %s)\n", obj->id, obj->name);

  return OK;
}