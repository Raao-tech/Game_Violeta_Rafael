/**
 * @brief It implements the object struct
 *
 * @file numen.c
 * @author Rafael
 * @version 0
 * @date 18-04-2026
 * @copyright GNU Public License
 */

#include "numen.h"
struct _Numen
{
  Character* c_numen; 
  Skills_id	skills[NUM_SKILLS];
};

/*========= (Create/Destroy) Numen ================*/
Numen* numen_create()
{
  Numen* new_numen = (Numen*)malloc(sizeof(Numen));
  if(!new_numen) return NULL;
  new_numen->c_numen = character_create();
  if(!new_numen->c_numen){
    free(new_numen);
    return NULL;
  }
  return new_numen;
}                  
Status numen_destroy(Numen* numen)
{
  if(numen){
    if(numen->c_numen) character_destroy(numen->c_numen);
    free(numen);
  } 
  return OK;
}

/*======== (Set/Get) Id ===========================*/
Status numen_set_id(Numen *numen, Id new_id) {
  if (!numen) return ERROR;
  return character_set_id(numen->c_numen, new_id);
}
Id numen_get_id(Numen *numen) {
  if (!numen) return NO_ID;
  return character_get_id(numen->c_numen);
}

/*======== (Set/Get/Has) Name ===========================*/
Status numen_set_name(Numen* numen, char* name)
{
	if(!numen || !name) return ERROR;
	return character_set_name(numen->c_numen, name);
}
char* numen_get_name(Numen* numen)
{
	if(!numen) return NULL;
	return character_get_name(numen->c_numen);
}
Bool numen_has_name(Numen *numen, char* name) {
  if (!numen || !name) return FALSE;
  return character_has_name(numen->c_numen, name);
}

/*======== (set/get) skills ================*/
Status numen_add_skill(Numen* numen, Id skill_id)
{
  if (!numen || skill_id < 0) return ERROR;
  for (int i = 0; i < MAX_HELD_SKILLS; i++) {
    if (numen->skills[i] == NO_SKILL) {
      numen->skills[i] = skill_id;
      return OK;
    }
  }
  return ERROR;
} 
Id    numen_get_skill_by_id(Numen* numen, Id skill_id)
{
  if (!numen || skill_id < 0) return NO_ID;
  for (int i = 0; i < MAX_HELD_SKILLS; i++) {
    if (numen->skills[i] == skill_id) {
      return skill_id;
    }
  }
  return NO_ID;
}
Skills_id numen_get_skill_by_index(Numen* numen, int skill_indx)
{
  if (!numen || skill_indx < 0 || skill_indx >= MAX_HELD_SKILLS) return NO_SKILL;
  return numen->skills[skill_indx];
} 

/*======== (Set/Get/ is errant?) following ===========================*/
Status numen_set_following(Numen* numen, Id following){
  if(!numen) return ERROR;
  return character_set_following(numen->c_numen, following);
}
Id   numen_get_following(Numen* numen)
{
  if(!numen) return NO_ID;
  return character_get_following(numen->c_numen);
}
Bool numen_is_errant(Numen* numen)
{
	if(!numen) return FALSE;
	return ((character_get_following(numen->c_numen) == NO_ID) ? TRUE : FALSE);
}

Status numen_set_corrupt(Numen *numen, Bool corrupt){
  if(!numen) return ERROR;
  return character_set_friendly(numen->c_numen, corrupt);
}
Bool numen_get_corrupt(Numen *numen){
  if(!numen) return FALSE;
  return (character_get_friendly(numen->c_numen) == TRUE ? FALSE : TRUE);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*======== (set/get) position ================*/
Status numen_set_pos_x(Numen* numen, int pos_x); // tiene que estar entre los límites x 
int    numen_get_pos_x(Numen* numen);

Status numen_set_pos_y(Numen* numen, int pos_y); // tiene que estar entre los límites y
int    numen_get_pos_y(Numen* numen);

/*======== (set/get) gdesc() ================*/
Status numen_set_gdesc(Numen* numen, char* gdesc); // Será una línea de texto
char*  numen_get_gdesc(Numen* numen);

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
Status character_set_attack(Character *character, int attack){
  if(!character) return ERROR;
  return entity_set_attack(character->e_character, attack);
}
int character_get_attack(Character *character){
  if(!character) return ERROR_ATTACK;
  return entity_get_attack(character->e_character);
}

/* energy */
Status character_set_energy(Character* character, int energy){
  if(!character) return ERROR;
  return entity_set_energy(character->e_character, energy);
}
int character_get_energy(Character *character){
  if(!character) return ERROR_ENGY;
  return entity_get_energy(character->e_character);
}

/* Speed */
Status character_set_speed(Character* character, int speed){
  if(!character) return ERROR;
  return entity_set_speed(character->e_character, speed);
}
int character_get_speed(Character *character){
  if(!character) return 0;
  return entity_get_speed(character->e_character);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Status numen_print(Numen *numen) 
{
  if (!numen) return ERROR;
  return character_print(numen->c_numen);
}