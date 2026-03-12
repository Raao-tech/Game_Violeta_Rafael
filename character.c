/**
 * @brief It implements the create struct
 *
 * @file character.c
 * @author Violeta y Rafael
 * @version 0
 * @date 04-02-2025
 * @copyright GNU Public License
 */

#include "character.h"


#define TAM 6

/**
 * @brief character
 *
 * This struct stores all the information of a create.
 */
struct _Character{
  Entity *e_character;
  Bool friendly;
};

/* create or destroy */
Character *character_create(){
  /* Creamos el espacio para el character */
  Character *newCharacter = (Character *) malloc(sizeof(Character));
  if(!newCharacter) return NULL;


  /* Creamos el espacio para entity de character */
  newCharacter->e_character = entity_create();
  if(!newCharacter->e_character){
    free(newCharacter);
    return NULL;
  };

  newCharacter->friendly = TRUE;

  return newCharacter;
}

Status character_destroy(Character *character)
{
  if (!character){
    return ERROR;
  }

  entity_destroy(character->e_character);
  free(character);

  return OK;
}

/* id */
Status  character_set_id(Character *character, Id new_id){
  if(!character) return ERROR;

  return  entity_set_id(character->e_character, new_id);
}

Id  character_get_id(Character *character){
  if(!character) return NO_ID;

  return  entity_get_id(character->e_character);
}

/* name */
Status character_set_name(Character *character, char *name){
  if (!character || !name) return ERROR;

  if (entity_set_name(character->e_character, name) == ERROR) return ERROR;

  return OK;
}

char *character_get_name(Character *character){
  if (!character || !character->e_character) return NULL;

  /* Ya entity_get_name hace un strdup del name que devuelve */
  return entity_get_name(character->e_character);
}

/* friendly */
Status character_set_friendly(Character *character, Bool value){

  if (!character) return ERROR;

  character->friendly=value;

  return OK;
}

Bool character_get_friendly(Character *character){
  if (!character) return TRUE;

  return character->friendly;
}


/*
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
 
 
 
