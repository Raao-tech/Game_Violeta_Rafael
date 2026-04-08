/**
 * @brief It implements the player struct
 *
 * @file player.c
 * @author Violeta y Rafael
 * @version 0
 * @date 04-02-2025
 * @copyright GNU Public License
 */


#include "types.h"
#include "set.h"
#include "inventory.h"



struct _Inventory{

    Set *objs;
    int max_objs;

};

Inventory *inventory_create(){
  Inventory *newInventory = NULL;
  newInventory = (Inventory *)calloc(1, sizeof(Inventory));
  if (newInventory == NULL) return NULL;

  /* Initialization of an empty inventory*/
  newInventory->objs = NULL;
  newInventory->max_objs = 0;

  return newInventory;
}


Status inventory_destroy(Inventory *inventory){
  if (!inventory) return ERROR;
  free(inventory);
  return OK;
}


Status	inventory_add(Set *pset, Id new_id){
	Id* pset_temp = NULL;
	/* If pset or new_id don't exist  */
	if(pset == NULL || new_id == NO_ID) return	ERROR;

	if( set_contains_id(pset, new_id) == TRUE) return OK;

	//agregamos un espacio en el conjutno de objetos
	pset_temp = (Id*)realloc((void *)pset->ids, (sizeof(Id) * (pset->n_ids + 1)) );
	
	if(pset_temp == NULL) return ERROR;
	
	pset_temp[pset->n_ids] = new_id;
	pset->n_ids++; 

	pset->ids = pset_temp;

	return	OK;
}

Status	set_delete_id(Set *pset, Id trash_id){
	int	i;
	if(!pset || trash_id == NO_ID) return ERROR;

	for (i = 0; i < pset->n_ids; i++){
		if(pset->ids[i] == trash_id) return set_reduce(pset,i);
	}

	return ERROR;
}



/* id */
Status inventory_set_max_objs(Inventory *inventory, int max){
  if (!inventory) return ERROR;
  inventory->max_objs = max;
  return OK;
}
Id inventory_get_id(Inventory *inventory){
  if (obj == NULL) return NO_ID;
  return obj->id;
}


/* name */
Status obj_set_name(Object *obj, char *name){
  if (!obj || !name) return ERROR;
  strncpy(obj->name, name, WORD_SIZE);
  obj->name[WORD_SIZE] = '\0';

  return OK;
}
Bool obj_has_name(Object *obj, char *name){
    if(!obj || !name) return FALSE;
    return (strcmp(obj->name, name) == 0) ? TRUE : FALSE;
}
char *obj_get_name(Object *obj){
  if (!obj) return NULL;
  return obj->name;
}



/* Print */
Status obj_print(Object *obj){
  /* Error Control */
  if (!obj) return ERROR;
  
  /* Print the id and the name of the object */
  fprintf(stdout, "--> Object (Id: %ld; Name: %s)\n", obj->id, obj->name);

  return OK;
}