/**
 * @brief It implements the space module
 *
 * @file space.c
 * @author Profesores PPROG, Violeta y Rafa
 * @version 3
 * @date 08-04-2026
 * @copyright GNU Public License
 */

#include "space.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Space
 *
 * This struct stores all the information of a space.
 * In I3, the neighbour Ids (north, south, east, west) have been
 * removed — topology is now handled entirely by the Link module.
 */
struct _Space {
  Id    id;                             /*!< Id number of the space, must be unique */
  char  name[WORD_SIZE + 1];            /*!< Name of the space */
  char  gdesc[MAX_LINE][MAX_CHAR + 1];  /*!< Graphic description, 5 lines x 9 chars */
  Set  *objs_id;                        /*!< Set of object IDs in this space */
  Set  *characters_id;                  /*!< Set of character IDs in this space */
  Bool  discovered;                     /*!< Has this space been visited by a player? */
};

/* ========== Create / Destroy ========== */

Space *space_create() {
  Space *newSpace = NULL;
  int i;

  newSpace = (Space *)calloc(1, sizeof(Space));
  if (!newSpace) return NULL;

  newSpace->id = NO_ID;
  newSpace->name[0] = '\0';
  newSpace->discovered = FALSE;

  for (i = 0; i < MAX_LINE; i++) {
    newSpace->gdesc[i][0] = '\0';
  }

  newSpace->objs_id = set_creat();
  if (!newSpace->objs_id) {
    free(newSpace);
    return NULL;
  }

  newSpace->characters_id = set_creat();
  if (!newSpace->characters_id) {
    set_destroy(newSpace->objs_id);
    free(newSpace);
    return NULL;
  }

  return newSpace;
}

Status space_destroy(Space *space) {
  if (!space) return ERROR;

  /*
   * We must destroy the Sets before freeing the Space struct.
   * set_destroy handles NULL gracefully (returns ERROR but no crash).
   */
  set_destroy(space->characters_id);
  set_destroy(space->objs_id);

  free(space);
  return OK;
}

/* ========== Id ========== */

Status space_set_id(Space *space, Id new_id) {
  if (!space) return ERROR;
  space->id = new_id;
  return OK;
}

Id space_get_id(Space *space) {
  if (!space) return NO_ID;
  return space->id;
}

/* ========== Name ========== */

Status space_set_name(Space *space, char *name) {
  if (!space || !name) return ERROR;

  strncpy(space->name, name, WORD_SIZE);
  space->name[WORD_SIZE] = '\0';

  return OK;
}

const char *space_get_name(Space *space) {
  if (!space) return NULL;
  return space->name;
}

/* ========== Objects ========== */

Status space_set_object(Space *space, Id new_id) {
  if (!space) return ERROR;
  return set_add(space->objs_id, new_id);
}

Bool space_contains_object(Space *space, Id id_obj) {
  if (!space) return FALSE;
  return set_contains_id(space->objs_id, id_obj);
}

Status space_remove_object(Space *space, Id id_obj) {
  if (!space) return ERROR;
  return set_delete_id(space->objs_id, id_obj);
}

int space_get_n_objects(Space *space) {
  if (!space) return ERROR_MAIN;
  return set_get_n_ids(space->objs_id);
}

/*
 * NEW: Allows graphic_engine to iterate over objects in a space
 * without breaking Set encapsulation.
 */
Id space_get_object_id_at(Space *space, int position) {
  if (!space) return NO_ID;
  return set_get_id_at(space->objs_id, position);
}

/* ========== Characters ========== */

Status space_set_character(Space *space, Id new_id) {
  if (!space) return ERROR;
  return set_add(space->characters_id, new_id);
}

Bool space_contains_character(Space *space, Id id_chara) {
  if (!space) return FALSE;
  return set_contains_id(space->characters_id, id_chara);
}

Status space_remove_character(Space *space, Id id_chara) {
  if (!space) return ERROR;
  return set_delete_id(space->characters_id, id_chara);
}

int space_get_n_characters(Space *space) {
  if (!space) return ERROR_MAIN;
  return set_get_n_ids(space->characters_id);
}

/* ========== Discovered ========== */

Status space_set_discovered(Space *space, Bool value) {
  if (!space) return ERROR;
  space->discovered = value;
  return OK;
}

Bool space_get_discovered(Space *space) {
  if (!space) return FALSE;
  return space->discovered;
}

/* ========== Graphic description ========== */

Status space_set_gdesc_line(Space *space, int line, char *desc) {
  if (!space || !desc) return ERROR;
  if (line < 0 || line >= MAX_LINE) return ERROR;
  if ((int)strlen(desc) > MAX_CHAR) return ERROR;

  strncpy(space->gdesc[line], desc, MAX_CHAR);
  space->gdesc[line][MAX_CHAR] = '\0';
  return OK;
}

Status space_print_gdesc(FILE *output, Space *space) {
  int i;
  if (!output || !space) return ERROR;

  for (i = 0; i < MAX_LINE; i++) {
    if (fprintf(output, "%s\n", space->gdesc[i]) < 0) return ERROR;
  }

  return OK;
}

char *space_get_gdesc(Space *space, int line) {
  if (!space) return NULL;
  if (line < 0 || line >= MAX_LINE) return NULL;
  return space->gdesc[line];
}

/* ========== Print ========== */

Status space_print(Space *space) {
  if (!space) return ERROR;

  fprintf(stdout, "--> Space (Id: %ld; Name: %s; Discovered: %s)\n",
          space->id, space->name,
          space->discovered == TRUE ? "YES" : "NO");

  /* Objects */
  if (space_get_n_objects(space) > 0) {
    fprintf(stdout, "---> Objects in the space:\n");
    set_print(stdout, space->objs_id);
  } else {
    fprintf(stdout, "---> No objects in the space.\n");
  }

  /* Characters */
  if (space_get_n_characters(space) > 0) {
    fprintf(stdout, "---> Characters in the space:\n");
    set_print(stdout, space->characters_id);
  } else {
    fprintf(stdout, "---> No characters in the space.\n");
  }

  /* Graphic description */
  space_print_gdesc(stdout, space);

  return OK;
}