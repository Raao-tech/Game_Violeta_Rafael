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


Status inventory_add(Inventory *inventory, Id new_id){
	
	if(inventory == NULL || new_id == NO_ID) return	ERROR;

  if (set_get_n_ids(inventory->objs) >= inventory->max_objs) return FULL_OF_LOVE;

	return	set_add(inventory->objs, new_id);
}

Status	inventory_delete_obj(Inventory *inventory, Id trash_id){

  if(!inventory || trash_id == NO_ID) return ERROR;

	return set_delete_id(inventory->objs, trash_id);
}

/* id */
Status inventory_set_max_objs(Inventory *inventory, int max){
  if (!inventory || max < 0) return ERROR;
  inventory->max_objs = max;
  return OK;
}

int inventory_get_max_objs(Inventory* inventory){

  if (!inventory) return ERROR;
  
  return inventory->max_objs;

}

int inventory_get_n_ids(Inventory *inventory){
  if (!inventory) return -1;

  return set_get_n_ids(inventory->objs);

}

Id	inventory_get_id_at(Inventory *inventory, int position){
	if(!inventory || position < 0 || position >= inventory_get_n_ids(inventory)) return NO_ID;

	return set_get_id_at(inventory, position);
}


/* Print */
Status inventory_print(Inventory *inventory){
  /* Error Control */
  if (!inventory) return ERROR;
  
  return set_print(stdout, inventory->objs);
}