/**
 * @brief It implements the object struct
 *
 * @file object.c
 * @author Violeta, Rafael, Javier Jarque and Salvador Alcalá
 * @version 3
 * @date 18-04-2026
 * @copyright GNU Public License
 */

#include "object.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Object
 *
 * This struct stores all the information of an object.
 */
struct _Object
{
    Entity* e_obj;
    Bool movable;              /*!< Whether the object can be taken or not */
    Id open;                   /*!< Id of the link that this object can open (for use command) */
    Id dependency;             /*!< Id of the object that this object depends on (for use
                                  command) */
    char gdesc[WORD_SIZE + 1]; /*!< Graphic description of the object */
    Bool consumable;           /*!< If the object could be consumable or not */
    Effect  effect;
};

/* ========== Create / Destroy ========== */

Object*
obj_create ()
{
    Object* newObj = NULL;

    newObj         = (Object*)calloc (1, sizeof (Object));
    if (newObj == NULL) return NULL;

    newObj->e_obj = entity_create ();
    if (!newObj->e_obj) {free (newObj); return NULL;}
    
    newObj->movable    = FALSE;
    newObj->open       = NO_ID;
    newObj->dependency = NO_ID;
    newObj->consumable = FALSE;
    newObj->gdesc[0]   = '\0';
    newObj->effect      = NO_EFECT;
    return newObj;
}

Status
obj_destroy (Object* obj)
{
    if (obj->e_obj) free (obj->e_obj);
    if (obj) free (obj);
    return OK;
}

/* ========== Id ========== */

Status
obj_set_id (Object* obj, Id id)
{
    if (!obj) return ERROR;
    return entity_set_id (obj->e_obj, id);
}

Id
obj_get_id (Object* obj)
{
    if (!obj) return NO_ID;
    return entity_get_id (obj->e_obj);
}

/* ========== Name ========== */

Status
obj_set_name (Object* obj, char* name)
{
    if (!obj || !name) return ERROR;
    return entity_set_name (obj->e_obj, name);
}

Bool
obj_has_name (Object* obj, char* name)
{
    if (!obj || !name) return FALSE;
    return entity_has_name (obj->e_obj, name);
}

char*
obj_get_name (Object* obj)
{
    if (!obj) return NULL;
    return entity_get_name (obj->e_obj);
}

/* ========== Description ========== */

Status
obj_set_description (Object* obj, char* description)
{
    if (!obj || !description) return ERROR;
    return entity_set_message (obj->e_obj, description);
}

char*
obj_get_description (Object* obj)
{
    if (!obj) return NULL;
    return entity_get_message (obj->e_obj);
}
/* ========== Gdesc ========== */

Status
obj_set_gdesc (Object* obj, char* gdesc)
{
    if (!obj || !gdesc) return ERROR;
    return entity_set_gdesc (obj->e_obj, gdesc);
}

char*
obj_get_gdesc (Object* obj)
{
    if (!obj) return NULL;
    return entity_get_gdesc (obj->e_obj);
}

/* ========== Position ========== */

Status
obj_set_position (Object* obj, int x, int y)
{
    if (!obj) return ERROR;
    return entity_set_position (obj->e_obj, x, y);
}

Position
obj_get_position (Object* obj)
{
    Position obj_pos;
    obj_pos.pos_x = NO_POS;
    obj_pos.pos_y = NO_POS;

    if (!obj) return obj_pos;
    return entity_get_position (obj->e_obj);
}

int
obj_get_pos_x (Object* obj)
{
    if (!obj) return NO_POS;
    return entity_get_pos_x (obj->e_obj);
}

int
obj_get_pos_y (Object* obj)
{
    if (!obj) return NO_POS;
    return entity_get_pos_y (obj->e_obj);
}

/* ========== Movable ========== */

Status
obj_set_movable (Object* obj, Bool movable)
{
    if (!obj) return ERROR;
    obj->movable = movable;
    return OK;
}

Bool
obj_get_movable (Object* obj)
{
    if (!obj) return FALSE;
    return obj->movable;
}

/* ========== Open ========== */
Status
obj_set_open (Object* obj, Id open)
{
    if (!obj) return ERROR;
    obj->open = open;
    return OK;
}

Id
obj_get_open (Object* obj)
{
    if (!obj) return NO_ID;
    return obj->open;
}

/* ========== Dependency ========== */

Status
obj_set_dependency (Object* obj, Id dependency)
{
    if (!obj) return ERROR;
    obj->dependency = dependency;
    return OK;
}

Id
obj_get_dependency (Object* obj)
{
    if (!obj) return NO_ID;
    return obj->dependency;
}

/*
 * Effects in Players or character (Numens)
 */
Status
obj_set_effect (Object* obj, Effect effect)
{
    if(!obj) return ERROR;
    obj->effect = effect;
    return OK;
}

Effect
obj_get_effect (Object* obj)
{
    if(!obj) return NO_EFECT;
    return obj->effect;
}

/* ======== STATS =============*/

Status
obj_set_stats (Object* obj, int speed, int health, int energy, int attack)
{
    if (!obj) return ERROR;
    entity_set_speed (obj->e_obj, speed);

    if (entity_set_attack (obj->e_obj, attack) == ERROR) return ERROR;
    if (entity_set_health (obj->e_obj, health) == ERROR) return ERROR;
    if (entity_set_attack (obj->e_obj, energy) == ERROR) return ERROR;
    return OK;
}
/*Health add*/
Status
obj_set_health (Object* obj, int health)
{
    if (!obj) return ERROR;
    return entity_set_health (obj->e_obj, health);
}
int
obj_get_health (Object* obj)
{
    if (!obj) return ERROR;
    return entity_get_health (obj->e_obj);
}
/*Speed add*/
Status
obj_set_speed (Object* obj, int speed)
{
    if (!obj) return ERROR;
    return entity_set_speed (obj->e_obj, speed);
}
int
obj_get_speed (Object* obj)
{
    if (!obj) return ERROR;
    return entity_get_speed (obj->e_obj);
}
/*Attack add*/
Status
obj_set_attack (Object* obj, int attack)
{
    if (!obj) return ERROR;
    return entity_set_attack (obj->e_obj, attack);
}
int
obj_get_attack (Object* obj)
{
    if (!obj) return ERROR;
    return entity_get_attack (obj->e_obj);
}
/*Speed add*/
Status
obj_set_energy (Object* obj, int energy)
{
    if (!obj) return ERROR;
    return entity_set_energy (obj->e_obj, energy);
}
int
obj_get_energy (Object* obj)
{
    if (!obj) return ERROR;
    return entity_get_energy (obj->e_obj);
}

/* ========== Consumable ========== */

Bool
obj_get_consumable (Object* obj)
{
    if (!obj) return -1;
    return obj->consumable;
}

Status
obj_set_consumable (Object* obj, Bool consumable)
{
    if (!obj) return ERROR;
    obj->consumable = consumable;
    return OK;
}

/* ========== Print ========== */

Status obj_print (Object* obj)
{
    Entity* e_obj;
    Bool movable;
    Id open;
    Id dependency;
    char gdesc[WORD_SIZE + 1];
    Bool consumable;
    Effect  effect;

    Status status;

    if (!obj) return ERROR;

    e_obj = obj->e_obj;
    movable = obj->movable;
    open = obj->open;
    dependency = obj->dependency;
    strcpy (gdesc, obj->gdesc);
    consumable = obj->consumable;
    effect = obj->effect;

    printf (stdout, "\n--- Object ---\n");

    status = entity_print(e_obj);
    if (status == ERROR) return ERROR;
    
    if (movable == TRUE) printf (stdout, " ->Movable: TRUE;\n");
    else printf (stdout, " ->Movable: FALSE;\n");
    printf (stdout, " ->Open id: %ld;\n", open);
    printf (stdout, " ->Dependency id: %ld;\n", dependency);
    printf (stdout, " ->Gdesc: %s;\n", gdesc);
    if (consumable == TRUE) printf (stdout, " ->Consumable: TRUE;\n");
    else printf (stdout, " ->Consumable: FALSE;\n");



    if (e_obj) free (e_obj);
    return OK;
}