/**
 * @brief It implements the character struct
 *
 * @file character.c
 * @author Violeta y Rafael
 * @version 0
 * @date 04-02-2025
 * @copyright GNU Public License
 */

#include "entity.h"

#define TAM 6

/**
 * @brief character
 *
 * This struct stores all the information of a character.
 */
struct _Entity
{
  Id id;                    /*!< Id number of the object, it must be unique */
  char name[WORD_SIZE + 1]; /*!< Name of the object */
  char gdesc[TAM];
  int health;
  char *message;
};

Entity *entity_create()
{
  Entity *newEntity = NULL;

  newEntity = (Entity *)malloc(sizeof(Entity));
  if (newEntity == NULL)
  {
    return NULL;
  }

  /* Initialization of an empty character*/
  newEntity->id = NO_ID;
  *newEntity->name = '\0';
  *newEntity->gdesc = '\0';
  newEntity->health = 0;
  
  return newEntity;
}




/*
Status create_destroy(Character *character)
{
  if (!character)
  {
    return ERROR;
  }

  free(character);
  return OK;
}

Status create_set_obj(Character *character, Id id_obj)
{
  if (character == NULL)
  {
    return ERROR;
  }

  character->id_object_take = id_obj;
  return OK;
}

Status create_set_space(Character *character, Id id_space)
{
  if (character == NULL || id_space == NO_ID)
  {
    return ERROR;
  }

  character->id_space_location = id_space;
  return OK;
}

Status create_set_name(Character *character, char *name)
{
  if (!character || !name)
  {
    return ERROR;
  }

  if (!strcpy(character->name, name))
  {
    return ERROR;
  }
  return OK;
}

char *create_get_name(Character *character)
{
  if (!character)
  {
    return NULL;
  }
  return character->name;
}

Id create_get_space(Character *character)
{
  if (character == NULL)
  {
    return NO_ID;
  }
  return character->id_space_location;
}

Id create_get_obj(Character *character)
{
  if (character == NULL)
  {
    return NO_ID;
  }
  return character->id_object_take;
}

Status create_print(Character *character)
{

  if (!character || character->id == NO_ID)
  {
    return ERROR;
  }


  fprintf(stdout, "--> Character %ld: %s)\n", character->id, character->name);

  return OK;
}
 */