/**
 * @brief It implements the object struct
 *
 * @file object.c
 * @author Violeta, Rafael, Javier Jarque and Salvador Alcalá
 * @version 3
 * @date 18-04-2026
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
  int health;                        /*!< Health of the object (for use command) */
  Bool movable;                       /*!< Whether the object can be taken or not */
  Id open;                            /*!< Id of the link that this object can open (for use command) */
  Id dependency;                      /*!< Id of the object that this object depends on (for use command) */
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
  newObj->health = 0;
  newObj->movable = FALSE;
  newObj->open = NO_ID;
  newObj->dependency = NO_ID;
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
/* ========== Health ========== */
Status obj_set_health(Object *obj, int health) {
  if (!obj) return ERROR;
  obj->health = health;
  return OK;
}

int obj_get_health(Object *obj) {
  if (!obj) return 0;
  return obj->health;
}
/* ========== Movable ========== */

Status obj_set_movable(Object *obj, Bool movable) {
  if (!obj) return ERROR;
  obj->movable = movable;
  return OK;
}

Bool obj_get_movable(Object *obj) {
  if (!obj) return FALSE;
  return obj->movable;
}

/* ========== Open ========== */
Status obj_set_open(Object *obj, Id open) {
  if (!obj) return ERROR;
  obj->open = open;
  return OK;
}

Id obj_get_open(Object *obj) {
  if (!obj) return NO_ID;
  return obj->open;
}

/* ========== Dependency ========== */

Status obj_set_dependency(Object *obj, Id dependency) {
  if (!obj) return ERROR;
  obj->dependency = dependency;
  return OK;
}

Id obj_get_dependency(Object *obj) {
  if (!obj) return NO_ID;
  return obj->dependency;
}

/* ========== Print ========== */

Status obj_print(Object *obj) {
  if (!obj) return ERROR;

  fprintf(stdout, "--> Object (Id: %ld; Name: %s; Desc: %s; Health: %d; Movable: %s; Open: %ld; Dependency: %ld)\n",
          obj->id, obj->name, obj->description, obj->health, obj->movable ? "Yes" : "No", obj->open == NO_ID ? "None": obj->open, obj->dependency == NO_ID ? "None": obj->dependency);

  return OK;
}