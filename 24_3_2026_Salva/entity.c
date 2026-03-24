/**
 * @brief It implements the entity struct
 *
 * @file character.c
 * @author Violeta
 * @version 0
 * @date 04-02-2025
 * @copyright GNU Public License
 */

#include "entity.h"


/**
 * @brief Entity
 *
 * This struct stores all the information of a Entity.
 */
struct _Entity{
  Id    id;                   /*!< Id number of the object, it must be unique */
  int   health;
  int   attack;
  char  *gdesc;
  char  *message;
  char  *name;
};

/* create or destroy */
Entity *entity_create(){
  Entity *newEntity = NULL;

  newEntity = (Entity *)malloc(sizeof(Entity));
  if (newEntity == NULL) {return NULL;}

  /* Initialization of an empty character*/
  newEntity->id = NO_ID;
  newEntity->gdesc = NULL;
  newEntity->name = NULL;
  newEntity->message = NULL;
  newEntity->health = 0;
  newEntity->attack = 0;
  
  return newEntity;
}
Status entity_destroy(Entity *entity){
  if (!entity) return ERROR;

  free(entity->gdesc);
  free(entity->name);
  free(entity->message);
  free(entity);

  return OK;
}


/* id */
Status  entity_set_id(Entity *entity, Id new_id){
  if(!entity) return ERROR;

  entity->id = new_id;
  return OK;
}
Id  entity_get_id(Entity *entity){
  if(!entity) return ERROR;

  return entity->id;
}


/* name */
Status  entity_set_name(Entity *entity, char*  name){
  int   length_name;

  if(!entity || !name){
    return  ERROR;
  }

  /* Calculamos el tamaño del name sin el \0  */
  length_name = strlen(name);

  /*  Si existe name, liberalo*/
  if(entity->name){
    free  (entity->name);
    entity->name = NULL;
  }

  /* Reservamos memoria (4 + 1 = 5 bytes para "Hola\0") */
  entity->name = (char *) calloc(length_name + 1, sizeof(char));

  /* VALIDAR la reserva de memoria antes de copiar */
  if (entity->name == NULL) {
    return ERROR;
  }

  /* Copiamos (strcpy copia los caracteres y el \0 automáticamente) */
  strcpy(entity->name, name);

  return OK;
}
char*  entity_get_name(Entity *entity){
  if(!entity || !entity->name){
    return NULL;
  }

  return  strdup(entity->name);
}

Bool entity_has_name(Entity *entity, char *name){
    char *entity_name = NULL;
    Bool result = FALSE;
    if(!entity|| !name) return FALSE;
    entity_name = entity_get_name(entity);
    if(!entity_name) return FALSE;
    result = (strcmp(entity, name) == 0) ? TRUE : FALSE;
    free(entity_name);
    return result;
}

/* gdesc */
Status  entity_set_gdesc(Entity *entity, char*  desc){
  int   length_desc;
  if(!entity || !desc) return  ERROR;


  /* Calculamos el tamaño del desc sin el \0  */
  length_desc = strlen(desc);

  /*  Si existe gdesc, liberalo.  Limpiamos nuestro anterior array*/
  if(entity->gdesc){
    free  (entity->gdesc);
    entity->gdesc = NULL;
  }

  /* Reservamos memoria (4 + 1 = 5 bytes para "Hola\0") */
  entity->gdesc = (char *) calloc(length_desc + 1, sizeof(char));

  /* VALIDAR la reserva de memoria antes de copiar */
  if (entity->gdesc == NULL) return ERROR; 

  /* Copiamos (strcpy copia los caracteres y el \0 automáticamente) */
  strcpy(entity->gdesc, desc);

  return OK;
}
char*  entity_get_gdesc(Entity *entity){
  if(!entity || !entity->gdesc){
    return NULL;
  }

  return  strdup(entity->gdesc);
}


/* message */
Status  entity_set_message(Entity *entity, char*  message){
  int   length_message;

  if(!entity || !message){
    return  ERROR;
  }

  /* Calculamos el tamaño del message sin el \0  */
  length_message = strlen(message);

  /*  Si existe message, liberalo*/
  if(entity->message){
    free  (entity->message);
    entity->message = NULL;
  }

  /* Reservamos memoria (4 + 1 = 5 bytes para "Hola\0") */
  entity->message = (char *) calloc(length_message + 1, sizeof(char));

  /* VALIDAR la reserva de memoria antes de copiar */
  if (entity->message == NULL) {
    return ERROR;
  }

  /* Copiamos (strcpy copia los caracteres y el \0 automáticamente) */
  strcpy(entity->message, message);

  return OK;
}
char*  entity_get_message(Entity *entity){
  if(!entity || !entity->message){
    return NULL;
  }

  return  strdup(entity->message);
}


/* health */
Status  entity_set_health(Entity *entity, int value){
  if(!entity || value < MIN_LIFE || value > MAX_LIFE) return  ERROR;
  
  entity->health = value;
  return OK;
}
int entity_get_health(Entity *entity){
  if(!entity) return ERROR_LIFE;

  return  entity->health;
}


/* attack */
Status  entity_set_attack(Entity *entity, int value){
  if(!entity || value < MIN_ATTACK) return  ERROR;
  
  entity->attack = value;
  return OK;
}
int entity_get_attack(Entity *entity){
  if(!entity) return ERROR_ATTACK;

  return  entity->attack;
}