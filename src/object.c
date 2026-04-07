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
struct _Object{
  Id id;                    /*!< Id number of the object, it must be unique */
  char name[WORD_SIZE + 1]; /*!< Name of the object */
};

/* create or destroy */
Object *obj_create(){
  Object *newObj = NULL;
  newObj = (Object *)calloc(1, sizeof(Object));
  if (newObj == NULL) return NULL;

  /* Initialization of an empty obj*/
  newObj->id = NO_ID;
  newObj->name[0] = '\0'; 

  return newObj;
}
Status obj_destroy(Object *obj){
  if (!obj) return ERROR;
  free(obj);
  return OK;
}

/* id */
Status obj_set_id(Object *obj, Id id){
  if (!obj) return ERROR;
  obj->id = id;
  return OK;
}
Id obj_get_id(Object *obj){
  if (obj == NULL) return NO_ID;
  return obj->id;
}


/* name */
Status obj_set_name(Object *obj, char *name){
  if (!obj || !name) return ERROR;
  strncpy(obj->name, name, WORD_SIZE);
  obj->name[WORD_SIZE] = '\0';

  return OK;
}
Bool obj_has_name(Object *obj, char *name){
    if(!obj || !name) return FALSE;
    return (strcmp(obj->name, name) == 0) ? TRUE : FALSE;
}
char *obj_get_name(Object *obj){
  if (!obj) return NULL;
  return obj->name;
}



/* Print */
Status obj_print(Object *obj){
  /* Error Control */
  if (!obj) return ERROR;
  
  /* Print the id and the name of the object */
  fprintf(stdout, "--> Object (Id: %ld; Name: %s)\n", obj->id, obj->name);

  return OK;
}