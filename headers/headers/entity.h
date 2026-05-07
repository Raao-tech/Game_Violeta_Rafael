/**
 * @brief It defines the command interpreter interface
 *
 * @file entity.h
 * @author Profesores PPROG, Rafael, Javier, Salvador and Violeta
 * @version 1.3
 * @date 18-04-2026
 * @copyright GNU Public License
 */

#ifndef ENTITY_H
#define ENTITY_H

#include "set.h"
#include "skills.h"
#include "types.h"

typedef struct _Stats Stats;
typedef struct _Position Position;
typedef struct _Entity Entity;

/**
 * @brief It creates a new entity, allocating memory and initializing it
 * @author Rafael, Javier, Salvador and Violeta
 *
 * @return a new entity, initialized, or NULL if memory fails
 */
Entity* entity_create (void);
/**
 * @brief It destroys a entity, freeing the allocated memory
 * @author Rafael, Javier, Salvador and Violeta
 *
 * @param entity a pointer to the entity
 * @return OK, if everything goes well, or ERROR if there was some mistake
 */
Status entity_destroy (Entity* entity);
/**
 * @brief It sets the id of a entity
 * @author Rafael, Javier, Salvador and Violeta
 *
 * @param entity a pointer to the entity
 * @param new_id the new id value
 * @return OK, if everything goes well, or ERROR if there was some mistake
 */
Status entity_set_id (Entity* entity, Id new_id);
/**
 * @brief It gets the id of a entity
 * @author Rafael, Javier, Salvador and Violeta
 *
 * @param entity a pointer to the entity
 * @return the entity's id, or NO_ID if entity is NULL
 */
Id entity_get_id (Entity* entity);

/**
 * @brief It sets the name of a entity
 * @author Rafael, Javier, Salvador and Violeta
 *
 * @param entity a pointer to the entity
 * @param name the name of the entity
 * @return OK, if everything goes well, or ERROR if there was some mistake
 */
Status entity_set_name (Entity* entity, char* name);
/**
 * @brief It gets a copy of the name of a entity (caller must free it)
 * @author Rafael, Javier, Salvador and Violeta
 *
 * @param entity a pointer to the entity
 * @return a copy of the name, or NULL if entity is NULL
 */
char* entity_get_name (Entity* entity);
/**
 * @brief It checks whether the entity has the given name
 * @author Rafael, Javier, Salvador and Violeta
 *
 * @param entity a pointer to the entity
 * @param name the name to compare against
 * @return TRUE if the names match, FALSE otherwise or if any parameter is NULL
 */
Bool entity_has_name (Entity* entity, char* name);

/**
 * @brief It sets the graphic description of a entity
 * @author Rafael, Javier, Salvador and Violeta
 *
 * @param entity a pointer to the entity
 * @param desc a string with the graphic description (max 6 chars for F4)
 * @return OK, if everything goes well, or ERROR if there was some mistake
 */
Status entity_set_gdesc (Entity* entity, char* desc);
/**
 * @brief It gets a copy of the graphic description of a entity (caller must free it)
 * @author Rafael, Javier, Salvador and Violeta
 *
 * @param entity a pointer to the entity
 * @return a copy of the gdesc, or NULL if entity is NULL
 */
char* entity_get_gdesc (Entity* entity);

/**
 * @brief It sets the message of a entity
 * @author Rafael, Javier, Salvador and Violeta
 *
 * @param entity a pointer to the entity
 * @param messg a string with the message
 * @return OK, if everything goes well, or ERROR if there was some mistake
 */
Status entity_set_message (Entity* entity, char* message);
/**
 * @brief It gets a copy of the message of a entity (caller must free it)
 * @author Rafael, Javier, Salvador and Violeta
 *
 * @param entity a pointer to the entity
 * @return a copy of the message, or NULL if entity is NULL
 */
char* entity_get_message (Entity* entity);

/**
 * @brief It sets the health of a entity
 * @author Rafael, Javier, Salvador and Violeta
 *
 * @param entity a pointer to the entity
 * @param health the health points to set
 * @return OK, if everything goes well, or ERROR if out of bounds
 */
Status entity_set_health (Entity* entity, int health);
/**
 * @brief It gets the health of a entity
 * @author Rafael, Javier, Salvador and Violeta
 *
 * @param entity a pointer to the entity
 * @return the health value, or ERROR_LIFE if entity is NULL
 */
int entity_get_health (Entity* entity);

/**
 * @brief It sets the speed of the entity that this entity is following
 * @author Rafael, Javier, Salvador and Violeta
 *
 * @param entity a pointer to the entity
 * @return OK, if everything goes well, or ERROR if out of bounds
 */
Status entity_set_speed (Entity* entity, int speed);
/**
 * @brief It gets the speed of the entity that this entity is following
 * @author Rafael, Javier, Salvador and Violeta
 *
 * @param entity a pointer to the entity
 * @return the speed of the entity being followed, or 0 if entity is NULL
 */
int entity_get_speed (Entity* entity);

/**
 * @brief It sets the energy value of a entity
 * @author Rafael, Javier, Salvador and Violeta
 *
 * @param entity a pointer to the entity
 * @param energy the energy value to set
 * @return OK, if everything goes well, or ERROR if there was some mistake
 */
Status entity_set_energy (Entity* entity, int energy);
/**
 * @brief It gets the energy value of a entity
 * @author Rafael, Javier, Salvador and Violeta
 *
 * @param entity a pointer to the entity
 * @return the energy value, or ERROR_ENGY if entity is NULL
 */
int entity_get_energy (Entity* entity);

/**
 * @brief It sets the attack value of a entity
 * @author Rafael, Javier, Salvador and Violeta
 *
 * @param entity a pointer to the entity
 * @param attack the attack value to set
 * @return OK, if everything goes well, or ERROR if there was some mistake
 */
Status entity_set_attack (Entity* entity, int attack);
/**
 * @brief It gets the attack value of a entity
 * @author Rafael, Javier, Salvador and Violeta
 *
 * @param entity a pointer to the entity
 * @return the attack value, or ERROR_ATTACK if entity is NULL
 */
int entity_get_attack (Entity* entity);

/*==========================   POSITION (Set/Get) =======================*/
/**
 * @brief It sets the position of a entity
 * @author Rafael, Javier, Salvador and Violeta
 *
 * @param cha a pointer to the entity
 * @param x the x value to set
 * @param y the y value to set
 * @return OK, if everything goes well, or ERROR if there was some mistake
 */
Status entity_set_position (Entity* entity, int x, int y);
/**
 * @brief It gets the position value of a entity
 * @author Rafael, Javier, Salvador and Violeta
 *
 * @param entity a pointer to the entity
 * @return the position value, or NO_POS in the x and y value if entity is NULL
 */
Position entity_get_position (Entity* entity);

/**
 * @brief It sets the x position of a entity
 * @author Rafael, Javier, Salvador and Violeta
 *
 * @param entity a pointer to the entity
 * @param x the x value to set
 * @return OK, if everything goes well, or ERROR if there was some mistake
 */
Status entity_set_pos_x (Entity* entity, int x);
/**
 * @brief It gets the x position value of a entity
 * @author Rafael, Javier, Salvador and Violeta
 *
 * @param entity a pointer to the entity
 * @return the x position value, or NO_POS if entity is NULL
 */
int entity_get_pos_x (Entity* entity);

/**
 * @brief It sets the y position of a entity
 * @author Rafael, Javier, Salvador and Violeta
 *
 * @param entity a pointer to the entity
 * @param y the y value to set
 * @return OK, if everything goes well, or ERROR if there was some mistake
 */
Status entity_set_pos_y (Entity* entity, int y);
/**
 * @brief It gets the y position value of a entity
 * @author Rafael, Javier, Salvador and Violeta
 *
 * @param entity a pointer to the entity
 * @return the y position value, or NO_POS if entity is NULL
 */
int entity_get_pos_y (Entity* entity);

/*==========Set et all stats  and position =====================*/

/**
 * @brief It sets the stats of a entity
 * @author Rafael, Javier, Salvador and Violeta
 *
 * @param entity a pointer to the entity
 * @param energy the energy value to set
 * @param health the health value to set
 * @param speed the speed value to set
 * @param attack the attack value to set
 * @return OK, if everything goes well, or ERROR if there was some mistake
 */
Status entity_set_stats (Entity* entity, int energy, int health, int speed, int attack);

#endif