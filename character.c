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


#define TAM 6

/**
 * @brief create
 *
 * This struct stores all the information of a create.
 */
struct _Character
{
  Entity *e_character;
  Bool friendly;
};

Character *character_create()
{
  Character *newCharacter = NULL;

  if(!entity_create(newCharacter->e_character)){
    return NULL;
  }

  newCharacter->friendly = TRUE;

  return newCharacter;
}




/*
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

  if (!create || create->id == NO_ID)
  {
    return ERROR;
  }


  fprintf(stdout, "--> Character %ld: %s)\n", create->id, create->name);

  return OK;
}

*/
 
 
 
