/**
 * @brief It implements the object struct
 *
 * @file object.c
 * @author Violeta, Rafael and Javier Jarque
 * @version 2
 * @date 08-04-2026
 * @copyright GNU Public License
 */

#include "object.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Object
 *
 * This struct stores all the information of an object.
 */
struct _Object {
  Id   id;                            /*!< Id number of the object, it must be unique */
  char name[WORD_SIZE + 1];           /*!< Name of the object */
  char description[WORD_SIZE + 1];    /*!< Description of the object (for inspect command) */
  /*
   * BUG FIX: The original code was missing the semicolon after
   * description[WORD_SIZE + 1].  The line read:
   *
   *   char description[WORD_SIZE + 1]  (no semicolon here)
   *
   * This is a syntax error that prevents compilation.
   */
};

/* ========== Create / Destroy ========== */

Object *obj_create() {
  Object *newObj = NULL;

  newObj = (Object *)calloc(1, sizeof(Object));
  if (newObj == NULL) return NULL;

  newObj->id = NO_ID;
  newObj->name[0] = '\0';
  newObj->description[0] = '\0';  /* FIX: initialize description too */

  return newObj;
}

Status obj_destroy(Object *obj) {
  if (!obj) return ERROR;
  free(obj);
  return OK;
}

/* ========== Id ========== */

Status obj_set_id(Object *obj, Id id) {
  if (!obj) return ERROR;
  obj->id = id;
  return OK;
}

Id obj_get_id(Object *obj) {
  if (obj == NULL) return NO_ID;
  return obj->id;
}

/* ========== Name ========== */

Status obj_set_name(Object *obj, char *name) {
  if (!obj || !name) return ERROR;
  strncpy(obj->name, name, WORD_SIZE);
  obj->name[WORD_SIZE] = '\0';
  return OK;
}

Bool obj_has_name(Object *obj, char *name) {
  if (!obj || !name) return FALSE;
  return (strcmp(obj->name, name) == 0) ? TRUE : FALSE;
}

char *obj_get_name(Object *obj) {
  if (!obj) return NULL;
  return obj->name;
}

/* ========== Description ========== */

Status obj_set_description(Object *obj, char *description) {
  if (!obj || !description) return ERROR;
  strncpy(obj->description, description, WORD_SIZE);
  obj->description[WORD_SIZE] = '\0';
  return OK;
}

Bool obj_has_description(Object *obj, char *description) {
  if (!obj || !description) return FALSE;
  return (strcmp(obj->description, description) == 0) ? TRUE : FALSE;
}

char *obj_get_description(Object *obj) {
  if (!obj) return NULL;
  return obj->description;
}

/* ========== Print ========== */

Status obj_print(Object *obj) {
  if (!obj) return ERROR;

  fprintf(stdout, "--> Object (Id: %ld; Name: %s; Desc: %s)\n",
          obj->id, obj->name, obj->description);

  return OK;
}