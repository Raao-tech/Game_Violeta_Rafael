/**
 * @brief It implements the create struct
 *
 * @file character.c
 * @author Violeta, Rafael y Salvador
 * @version 3
 * @date 16-04-2025
 * @copyright GNU Public License
 */

#include "character.h"


#define TAM 6
#define  MIN_LIFE 0
#define  MIN_ATTACK 0
#define  MIN_ENGY 0
#define  MAX_LIFE 10
#define  ERROR_LIFE -1
#define  ERROR_ATTACK -1
#define  ERROR_ENGY -1

/**
 * @brief Estadísticas de un character
 */
typedef struct {
  int health;   /*!< Salud: puede ser positiva o negativa */
  int attack;   /*!< Poder de ataque */
  int energy;   /*!< Puntos de energía o maná */
} Stats;

/**
 * @brief character
 *
 * This struct stores all the information of a character.
 */
struct _Character{
  Entity *e_character;    /*<! It stores the entity*/
  Bool friendly;          /*<! It stores hether the character is friendly or not*/
  Id following;           /*<! It stores the id of the character that this character is following*/
  Stats stats;            /*<! It stores the stats of a character (health, attack, energy)*/
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
  newCharacter->following = NO_ID;
  newCharacter->stats.attack = MIN_ATTACK;
  newCharacter->stats.energy = MIN_ENGY;
  newCharacter->stats.health = MIN_LIFE;

  return newCharacter;
}
Status character_destroy(Character *character)
{
  if (!character) return ERROR;
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
      if(!character || !name) return FALSE;
 
      return entity_has_name(character->e_character, name);
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
  if(life < MIN_LIFE){
    character->stats.health =MIN_LIFE;
    return OK;
  }
  character->stats.health =life;
  return OK;
}
int character_get_health(Character *character){
  if(!character) return ERROR_LIFE;
  return character->stats.health;
}

/* attack */
Status character_set_attack(Character *character, int attack){
  if(!character) return ERROR;
    if(attack< MIN_ATTACK){
    character->stats.attack =MIN_ATTACK;
    return OK;
  }
  character->stats.attack = attack;
  return OK;
}
int character_get_attack(Character *character){
  if(!character) return ERROR_ATTACK;
  return character->stats.attack;
}

/* energy */
Status character_set_energy(Character* character, int energy){
  if(!character) return ERROR;
  if(energy < MIN_ENGY){
    character->stats.energy =MIN_ENGY;
    return OK;
  }
  character->stats.energy =energy;
  return OK
}
int character_get_energy(Character *character){
  if(!character) return ERROR_ENGY;
  return character->stats.energy;
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

/* following */
Status character_set_following(Character *character, Id id_following)
{
  if(!character) return ERROR;
  character->following=id_following;
  return OK;
}

Id character_get_following(Character *character)
{
  if(!character) return NO_ID;
  return character->following;
}

/* ================ PRINT ============================ */

Status character_print(Character *character){
  char *name =      NULL;
  char *gdesc =     NULL;
  char *message =   NULL;
  Id    following = NO_ID;
  int   health =    0;
  int   attack =    0;
  int   energy =    0;

  if(!character) return ERROR;

  name = character_get_name(character);
  gdesc = character_get_gdesc(character);
  message = character_get_message(character);
  following = character_get_following(character);
  health = character_get_health(character);
  attack = character_get_attack(character);
  energy = character_get_energy(character);


  fprintf(stdout, "\n--- Character ---\n");
  fprintf(stdout, "|| ID:       %ld\n", character_get_id(character));
  fprintf(stdout, "|| NAME:     %s\n", name ? name : "N/A");
  fprintf(stdout, "|| GDESC:    %s\n", gdesc ? gdesc : "N/A");
  fprintf(stdout, "|| HEALTH:   %d\n", health);
  fprintf(stdout, "|| ATTACK:   %d\n", attack);
  fprintf(stdout, "|| HEALTH:   %d\n", energy);
  fprintf(stdout, "|| FRIENDLY: %s\n", character_get_friendly(character) == TRUE ? "YES" : "NO");
  fprintf(stdout, "|| MESSAGE:  %s\n", message ? message : "N/A");
  fprintf(stdout, "|| FOLLOWING:  %ld\n", following!=NO_ID ? following : NO_ID);
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
 
 
 
