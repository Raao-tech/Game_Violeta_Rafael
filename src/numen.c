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
  Skills*	skills[MAX_HELD_SKILLS];
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
Bool numen_set_is_errant(Numen* numen)
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
  return character_get_friendly(numen->c_numen);
}


Status numen_print(Numen *numen) 
{
  if (!numen) return ERROR;
  return character_print(numen->c_numen);
}