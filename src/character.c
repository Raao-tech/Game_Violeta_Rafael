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
Bool character_has_name(Character *character, char *name){
    char *char_name = NULL;
    Bool result = FALSE;
    if(!character || !name) return FALSE;
    char_name = character_get_name(character);
    if(!char_name) return FALSE;
    result = (strcmp(char_name, name) == 0) ? TRUE : FALSE;
    free(char_name);
    return result;
}
char *character_get_name(Character *character){
  if (!character || !character->e_character) return NULL;

  /* Ya entity_get_name hace un strdup del name que devuelve */
  return entity_get_name(character->e_character);
}


/* gDescription */
Status    character_set_gdesc(Character *character, char* desc){
  if(!character || !desc) return ERROR;
  return entity_set_gdesc(character->e_character, desc);
}
char      *character_get_gdesc(Character *character){
  if(!character) return NULL;
  return  entity_get_gdesc(character->e_character);
}



/* message*/
Status    character_set_message(Character *character, char* messg){
  if(!character || !messg) return ERROR;
  return entity_set_message(character->e_character, messg);
}
char      *character_get_message(Character *character){
  if(!character) return NULL;
  return  entity_get_message(character->e_character);
}
   

/* health */
Status character_set_health(Character *character, int life){
  if(!character) return ERROR;
  return entity_set_health(character->e_character, life);
}
int character_get_health(Character *character){
  if(!character) return ERROR_LIFE;
  return entity_get_health(character->e_character);
}

/* attack */
Status character_set_attack(Character *character, int value){
  if(!character) return ERROR;
  return entity_set_attack(character->e_character, value);
}
int character_get_attack(Character *character){
  if(!character) return ERROR_ATTACK;
  return entity_get_attack(character->e_character);
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




/* ================ PRINT ============================ */

Status character_print(Character *character){
  char *name = NULL;
  char *gdesc = NULL;
  char *message = NULL;

  if(!character) return ERROR;

  name = character_get_name(character);
  gdesc = character_get_gdesc(character);
  message = character_get_message(character);

  fprintf(stdout, "\n--- Character ---\n");
  fprintf(stdout, "|| ID:       %ld\n", character_get_id(character));
  fprintf(stdout, "|| NAME:     %s\n", name ? name : "N/A");
  fprintf(stdout, "|| GDESC:    %s\n", gdesc ? gdesc : "N/A");
  fprintf(stdout, "|| HEALTH:   %d\n", character_get_health(character));
  fprintf(stdout, "|| FRIENDLY: %s\n", character_get_friendly(character) == TRUE ? "YES" : "NO");
  fprintf(stdout, "|| MESSAGE:  %s\n", message ? message : "N/A");
  fprintf(stdout, "-----------------\n");

  if(name) free(name);
  if(gdesc) free(gdesc);
  if(message) free(message);

  return OK;
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
 
 
 
