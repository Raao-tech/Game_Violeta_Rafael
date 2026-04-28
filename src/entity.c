/**
 * @brief It implements the entity struct
 *
 * @file entity.c
 * @author Violeta, Rafa y Salvador
 * @version 2
 * @date 24-03-2026
 * @copyright GNU Public License
 */

#include "entity.h"
#include <stdlib.h>
#include <string.h>

/*
================================================================
  STRUCTS PRIVADOS
================================================================
*/

struct _Stats
{
    int health; /*!< Salud */
    int attack; /*!< Poder de ataque */
    int energy; /*!< Puntos de energía */
    int speed;  /*!< Velocidad */
};

struct _Position
{
    int pos_x; /*!< Posición x en el space */
    int pos_y; /*!< Posición y en el space */
};

struct _Entity
{
    Id id;             /*!< Id única */
    char* gdesc;       /*!< Descripción gráfica */
    char* message;     /*!< Mensaje asociado */
    char* name;        /*!< Nombre */
    Stats stats;       /*!< Estadística */
    Position position; /*!< Posicion (int x , int y)*/
};

/*
================================================================
  CREATE / DESTROY
================================================================
*/

Entity*
entity_create (void)
{
    Entity* newEntity = (Entity*)malloc (sizeof (Entity));
    if (!newEntity) return NULL;

    newEntity->id             = NO_ID;
    newEntity->gdesc          = NULL;
    newEntity->name           = NULL;
    newEntity->message        = NULL;
    newEntity->position.pos_x = 0;
    newEntity->position.pos_y = 0;
    newEntity->stats.attack   = MIN_ATTACK;
    newEntity->stats.speed    = 0;
    newEntity->stats.health   = MIN_LIFE;
    newEntity->stats.energy   = MIN_ENGY;

    return newEntity;
}

Status
entity_destroy (Entity* entity)
{
    if (!entity) return ERROR;

    if (entity->gdesc) free (entity->gdesc);
    if (entity->name) free (entity->name);
    if (entity->message) free (entity->message);
    free (entity);

    return OK;
}

/* ================================================================
   ID
   ================================================================ */

Status
entity_set_id (Entity* entity, Id new_id)
{
    if (!entity) return ERROR;
    entity->id = new_id;
    return OK;
}

Id
entity_get_id (Entity* entity)
{
    if (!entity) return NO_ID;
    return entity->id;
}

/*
================================================================
  NAME
================================================================
*/

Status
entity_set_name (Entity* entity, char* name)
{
    int length_name;
    if (!entity || !name) return ERROR;

    length_name = strlen (name);

    if (entity->name)
        {
            free (entity->name);
            entity->name = NULL;
        }

    entity->name = (char*)calloc (length_name + 1, sizeof (char));
    if (!entity->name) return ERROR;

    strcpy (entity->name, name);
    return OK;
}

char*
entity_get_name (Entity* entity)
{
    if (!entity || !entity->name) return NULL;
    return strdup (entity->name);
}

Bool
entity_has_name (Entity* entity, char* name)
{
    char* entity_name = NULL;
    Bool result       = FALSE;

    if (!entity || !name) return FALSE;

    entity_name = entity_get_name (entity);
    if (!entity_name) return FALSE;

    result = (strcmp (entity_name, name) == 0) ? TRUE : FALSE;
    free (entity_name);
    return result;
}

/*
================================================================
  GDESC
================================================================
*/

Status
entity_set_gdesc (Entity* entity, char* desc)
{
    int length_desc;
    if (!entity || !desc) return ERROR;

    length_desc = strlen (desc);

    if (entity->gdesc)
        {
            free (entity->gdesc);
            entity->gdesc = NULL;
        }

    entity->gdesc = (char*)calloc (length_desc + 1, sizeof (char));
    if (!entity->gdesc) return ERROR;

    strcpy (entity->gdesc, desc);
    return OK;
}

char*
entity_get_gdesc (Entity* entity)
{
    if (!entity || !entity->gdesc) return NULL;
    return strdup (entity->gdesc);
}

/*
================================================================
  MESSAGE
================================================================
*/

Status
entity_set_message (Entity* entity, char* message)
{
    int length_message;
    if (!entity || !message) return ERROR;

    length_message = strlen (message);

    if (entity->message)
        {
            free (entity->message);
            entity->message = NULL;
        }

    entity->message = (char*)calloc (length_message + 1, sizeof (char));
    if (!entity->message) return ERROR;

    strcpy (entity->message, message);
    return OK;
}

char*
entity_get_message (Entity* entity)
{
    if (!entity || !entity->message) return NULL;
    return strdup (entity->message);
}

/*
================================================================
  STATS
================================================================ */

Status
entity_set_health (Entity* entity, int health)
{
    if (!entity) return ERROR;
    entity->stats.health = (health < MIN_LIFE) ? MIN_LIFE : health;
    return OK;
}
int
entity_get_health (Entity* entity)
{
    if (!entity) return ERROR_LIFE;
    return entity->stats.health;
}

Status
entity_set_speed (Entity* entity, int speed)
{
    if (!entity) return ERROR;
    entity->stats.speed = speed;
    return OK;
}
int
entity_get_speed (Entity* entity)
{
    if (!entity) return 0;
    return entity->stats.speed;
}

Status
entity_set_energy (Entity* entity, int energy)
{
    if (!entity) return ERROR;
    entity->stats.energy = (energy < MIN_ENGY) ? MIN_ENGY : energy;
    return OK;
}
int
entity_get_energy (Entity* entity)
{
    if (!entity) return ERROR_ENGY;
    return entity->stats.energy;
}

Status
entity_set_attack (Entity* entity, int attack)
{
    if (!entity) return ERROR;
    entity->stats.attack = (attack < MIN_ATTACK) ? MIN_ATTACK : attack;
    return OK;
}
int
entity_get_attack (Entity* entity)
{
    if (!entity) return ERROR_ATTACK;
    return entity->stats.attack;
}

Status
entity_set_stats (Entity* entity, int energy, int health, int speed, int attack)
{
    if (!entity) return ERROR;
    entity->stats.energy = (energy < MIN_ENGY) ? MIN_ENGY : energy;
    entity->stats.health = (health < MIN_LIFE) ? MIN_LIFE : health;
    entity->stats.attack = (attack < MIN_ATTACK) ? MIN_ATTACK : attack;
    entity->stats.speed  = speed;
    return OK;
}

/*
================================================================
  POSITION
================================================================
*/

Status
entity_set_position (Entity* entity, int x, int y)
{
    if (!entity) return ERROR;
    entity->position.pos_x = x;
    entity->position.pos_y = y;
    return OK;
}

int
entity_get_pos_x (Entity* entity)
{
    if (!entity) return 0;
    return entity->position.pos_x;
}

int
entity_get_pos_y (Entity* entity)
{
    if (!entity) return 0;
    return entity->position.pos_y;
}