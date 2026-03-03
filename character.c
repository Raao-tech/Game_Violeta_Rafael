/**
 * @brief It implements the create struct
 *
 * @file create.c
 * @author Violeta y Rafael
 * @version 0
 * @date 04-02-2025
 * @copyright GNU Public License
 */

#include "character.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM 6

/**
 * @brief create
 *
 * This struct stores all the information of a create.
 */
struct _Character
{
  Id id;                    /*!< Id number of the object, it must be unique */
  char name[WORD_SIZE + 1]; /*!< Name of the object */
  char gdesc[TAM];
  int health;
  Bool friendly;
};

Character *character_create(Id id)
{
  Character *newcreate = NULL;

  /* Error control */
  if (id == NO_ID)
  {
    return NULL;
  }

  newcreate = (Character *)malloc(sizeof(Character));
  if (newcreate == NULL)
  {
    return NULL;
  }

  /* Initialization of an empty create*/
  newcreate->id = id;
  newcreate->id_space_location = NO_ID;
  newcreate->id_object_take = NO_ID;
  newcreate->name[0] = '\0';

  return newcreate;
}

/* functions to sets properties and assets of the create */
Status create_destroy(Character *create)
{
  if (!create)
  {
    return ERROR;
  }

  free(create);
  return OK;
}

Status create_set_obj(Character *create, Id id_obj)
{
  if (create == NULL)
  {
    return ERROR;
  }

  create->id_object_take = id_obj;
  return OK;
}

Status create_set_space(Character *create, Id id_space)
{
  if (create == NULL || id_space == NO_ID)
  {
    return ERROR;
  }

  create->id_space_location = id_space;
  return OK;
}

Status create_set_name(Character *create, char *name)
{
  if (!create || !name)
  {
    return ERROR;
  }

  if (!strcpy(create->name, name))
  {
    return ERROR;
  }
  return OK;
}

/* functions to know properties and assets of the create */
char *create_get_name(Character *create)
{
  if (!create)
  {
    return NULL;
  }
  return create->name;
}

Id create_get_space(Character *create)
{
  if (create == NULL)
  {
    return NO_ID;
  }
  return create->id_space_location;
}

Id create_get_obj(Character *create)
{
  if (create == NULL)
  {
    return NO_ID;
  }
  return create->id_object_take;
}

Status create_print(Character *create)
{
  /* Error Control */
  if (!create || create->id == NO_ID)
  {
    return ERROR;
  }

  /* 1. Print the id and the name of the create */
  fprintf(stdout, "--> Character %ld: %s)\n", create->id, create->name);

  return OK;
}