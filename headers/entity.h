#ifndef ENTITY_H
#define ENTITY_H

#include "set.h"
#include "skills.h"
#include "types.h"

typedef struct _Stats Stats;
typedef struct _Position Position;
typedef struct _Entity Entity;

Entity* entity_create (void);
Status entity_destroy (Entity* entity);

Status entity_set_id (Entity* entity, Id new_id);
Id entity_get_id (Entity* entity);

Status entity_set_name (Entity* entity, char* name);
char* entity_get_name (Entity* entity);
Bool entity_has_name (Entity* entity, char* name);

Status entity_set_gdesc (Entity* entity, char* desc);
char* entity_get_gdesc (Entity* entity);

Status entity_set_message (Entity* entity, char* message);
char* entity_get_message (Entity* entity);

Status entity_set_health (Entity* entity, int health);
int entity_get_health (Entity* entity);

Status entity_set_speed (Entity* entity, int speed);
int entity_get_speed (Entity* entity);

Status entity_set_energy (Entity* entity, int energy);
int entity_get_energy (Entity* entity);

Status entity_set_attack (Entity* entity, int attack);
int entity_get_attack (Entity* entity);


/*==========================   POSITION (Set/Get) =======================*/
Status      entity_set_position (Entity* entity, int x, int y);
Position    entity_get_position (Entity* entity);

Status      entity_set_pos_x (Entity* entity, int x);
int         entity_get_pos_x (Entity* entity);

Status      entity_set_pos_y (Entity* entity, int y);
int         entity_get_pos_y (Entity* entity);


/*==========Set et all stats  and position =====================*/



Status entity_set_stats (Entity* entity, int energy, int health, int speed, int attack);

#endif