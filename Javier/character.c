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

/**
 * @brief character
 *
 * This struct stores all the information of a character.
 */
struct _Character
{
    Entity* e_character; /*<! It stores the entity*/
    Bool friendly;       /*<! It stores hether the character is friendly or not*/
    Id following;        /*<! It stores the id of the character that this character is
                            following*/
};

/* create or destroy */
Character*
character_create ()
{
    /* Creamos el espacio para el character */
    Character* newCharacter = (Character*)malloc (sizeof (Character));
    if (!newCharacter) return NULL;

    /* Creamos el espacio para entity de character */
    newCharacter->e_character = entity_create ();
    if (!newCharacter->e_character)
        {
            free (newCharacter);
            return NULL;
        };

    newCharacter->friendly  = TRUE;
    newCharacter->following = NO_ID;

    return newCharacter;
}
Status
character_destroy (Character* character)
{
    if (!character) return ERROR;
    entity_destroy (character->e_character);
    free (character);

    return OK;
}

/* id */
Status
character_set_id (Character* character, Id new_id)
{
    if (!character) return ERROR;

    return entity_set_id (character->e_character, new_id);
}
Id
character_get_id (Character* character)
{
    if (!character) return NO_ID;

    return entity_get_id (character->e_character);
}

/* name */
Status
character_set_name (Character* character, char* name)
{
    if (!character || !name) return ERROR;

    if (entity_set_name (character->e_character, name) == ERROR) return ERROR;

    return OK;
}
Bool
character_has_name (Character* character, char* name)
{
    if (!character || !name) return FALSE;

    return entity_has_name (character->e_character, name);
}
char*
character_get_name (Character* character)
{
    if (!character || !character->e_character) return NULL;

    /* Ya entity_get_name hace un strdup del name que devuelve */
    return entity_get_name (character->e_character);
}

/* gDescription */
Status
character_set_gdesc (Character* character, char* desc)
{
    if (!character || !desc) return ERROR;
    return entity_set_gdesc (character->e_character, desc);
}
char*
character_get_gdesc (Character* character)
{
    if (!character) return NULL;
    return entity_get_gdesc (character->e_character);
}

/* message*/
Status
character_set_message (Character* character, char* messg)
{
    if (!character || !messg) return ERROR;
    return entity_set_message (character->e_character, messg);
}
char*
character_get_message (Character* character)
{
    if (!character) return NULL;
    return entity_get_message (character->e_character);
}

/* health */
Status
character_set_health (Character* character, int life)
{
    if (!character) return ERROR;
    return entity_set_health (character->e_character, life);
}
int
character_get_health (Character* character)
{
    if (!character) return ERROR_LIFE;
    return entity_get_health (character->e_character);
}

/* attack */
Status
character_set_attack (Character* character, int attack)
{
    if (!character) return ERROR;
    return entity_set_attack (character->e_character, attack);
}
int
character_get_attack (Character* character)
{
    if (!character) return ERROR_ATTACK;
    return entity_get_attack (character->e_character);
}

/* energy */
Status
character_set_energy (Character* character, int energy)
{
    if (!character) return ERROR;
    return entity_set_energy (character->e_character, energy);
}
int
character_get_energy (Character* character)
{
    if (!character) return ERROR_ENGY;
    return entity_get_energy (character->e_character);
}

/* Speed */
Status
character_set_speed (Character* character, int speed)
{
    if (!character) return ERROR;
    return entity_set_speed (character->e_character, speed);
}
int
character_get_speed (Character* character)
{
    if (!character) return 0;
    return entity_get_speed (character->e_character);
}

/* friendly */
Status
character_set_friendly (Character* character, Bool value)
{
    if (!character) return ERROR;
    character->friendly = value;
    return OK;
}
Bool
character_get_friendly (Character* character)
{
    if (!character) return TRUE;

    return character->friendly;
}

/* following */
Status
character_set_following (Character* character, Id id_following)
{
    if (!character) return ERROR;
    character->following = id_following;
    return OK;
}
Id
character_get_following (Character* character)
{
    if (!character) return NO_ID;
    return character->following;
}



/*======== (set/get) position ================*/
Status
character_set_position (Character* cha, int x, int y)
{
    if (!cha) return ERROR;
    if (x < 0 || x >= WIDHT_SCREEN || y < 0 || y >= HIGHT_SCREEN) return ERROR;
    return entity_set_position (cha->e_character, x, y);
}

Position
character_get_position (Character* cha)
{
    Position pos;
    pos.pos_x = NO_POS;
    pos.pos_y = NO_POS;

    if (!cha) return pos;
    return entity_get_position (cha->e_character);
}

Status
character_set_pos_x (Character* cha, int pos_x)
{
    if (!cha) return ERROR;
    return entity_set_pos_x (cha->e_character, pos_x);
}

int
character_get_pos_x (Character* cha)
{
    if (!cha) return NO_POS;
    return entity_get_pos_x (cha->e_character);
}

Status
character_set_pos_y (Character* cha, int pos_y)
{
    if (!cha) return ERROR;
    return entity_set_pos_y (cha->e_character, pos_y);  
}

int
character_get_pos_y (Character* cha)
{
    if (!cha) return NO_POS;
    return entity_get_pos_y (cha->e_character);
}











/* ================ PRINT ============================ */

Status character_print (Character* character)
{
    Entity* e_character;
    Bool friendly; 
    Id following;    
    
    Status status;
    
    if (!character) return ERROR;

    e_character = character->e_character;
    friendly = character->friendly;
    following = character->following;

    printf (stdout, "\n--- Character ---\n");
    status = entity_print (e_character);
    if (status == ERROR) return ERROR;
    if (friendly == FALSE) printf (stdout, "-> Friendly: FALSE");
     else printf (stdout, "-> Friendly: TRUE;\n");
    printf (stdout, "-> Id: %ld;\n", following);

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
