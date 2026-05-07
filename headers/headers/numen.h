/**
 * @brief It defines the numen module interface
 *
 * @file Numens.h
 * @author Rafa
 * @version 0
 * @date 18-04-2026
 * @copyright GNU Public License
 */

#ifndef NUMEN_H
#define NUMEN_H

#include "character.h"
#include "skills.h"
#include "types.h"

typedef struct _Numen Numen;

/*======== (Create/Destroy) Numen ================*/
/**
 * @brief It creates a new numen, allocating memory and initializing it
 * @author Rafael, Javier, Salvador and Violeta
 *
 * @return a new numen, initialized, or NULL if memory fails
 */
Numen* numen_create ();
/**
 * @brief It destroys a numen, freeing the allocated memory
 * @author Rafael, Javier, Salvador and Violeta
 *
 * @param numen a pointer to the numen
 * @return OK, if everything goes well, or ERROR if there was some mistake
 */
Status numen_destroy (Numen* numen);

/*======== (set/get) name (char*) ================*/
/**
 * @brief It sets the name of a numen
 * @author Rafael, Javier, Salvador and Violeta
 *
 * @param numen a pointer to the numen
 * @param name the name of the numen
 * @return OK, if everything goes well, or ERROR if there was some mistake
 */
Status numen_set_name (Numen* numen, char* name);
/**
 * @brief It gets a copy of the name of a numen (caller must free it)
 * @author Rafael, Javier, Salvador and Violeta
 *
 * @param numen a pointer to the numen
 * @return a copy of the name, or NULL if numen is NULL
 */
char* numen_get_name (Numen* numen);
/**
 * @brief It checks whether the numen has the given name
 * @author Rafael, Javier, Salvador and Violeta
 *
 * @param numen a pointer to the numen
 * @param name the name to compare against
 * @return TRUE if the names match, FALSE otherwise or if any parameter is NULL
 */
Bool numen_has_name (Numen* numen, char* name);

/*======== (set/get) is_errante (Bool) ================*/
/**
 * @brief It checks if the numen is errant
 * @author Rafael, Javier, Salvador and Violeta
 *
 * @param numen a pointer to the numen
 * @return TRUE if the numen is errant, FALSE otherwise
 */
Bool numen_is_errant (Numen* numen);

/*======== (set/get) position ================*/
/**
 * @brief It sets the position of a numen
 * @author Rafael, Javier, Salvador and Violeta
 *
 * @param cha a pointer to the numen
 * @param x the x value to set
 * @param y the y value to set
 * @return OK, if everything goes well, or ERROR if there was some mistake
 */
Status numen_set_position (Numen* numen, int x, int y);
/**
 * @brief It gets the position value of a numen
 * @author Rafael, Javier, Salvador and Violeta
 *
 * @param numen a pointer to the numen
 * @return the position value, or NO_POS in the x and y value if numen is NULL
 */
Position numen_get_position (Numen* numen);

/**
 * @brief It sets the x position of a numen
 * @author Rafael, Javier, Salvador and Violeta
 *
 * @param numen a pointer to the numen
 * @param x the x value to set
 * @return OK, if everything goes well, or ERROR if there was some mistake
 */
Status numen_set_pos_x (Numen* numen, int pos_x); /* tiene que estar entre los límites x */
/**
 * @brief It gets the x position value of a numen
 * @author Rafael, Javier, Salvador and Violeta
 *
 * @param numen a pointer to the numen
 * @return the x position value, or NO_POS if numen is NULL
 */
int numen_get_pos_x (Numen* numen);

/**
 * @brief It sets the y position of a numen
 * @author Rafael, Javier, Salvador and Violeta
 *
 * @param numen a pointer to the numen
 * @param y the y value to set
 * @return OK, if everything goes well, or ERROR if there was some mistake
 */
Status numen_set_pos_y (Numen* numen, int pos_y); /* tiene que estar entre los límites y */
/**
 * @brief It gets the y position value of a numen
 * @author Rafael, Javier, Salvador and Violeta
 *
 * @param numen a pointer to the numen
 * @return the y position value, or NO_POS if numen is NULL
 */
int numen_get_pos_y (Numen* numen);

/*======== (set/get) gdesc() ================*/
/**
 * @brief It sets the graphic description of a numen
 * @author Rafael, Javier, Salvador and Violeta
 *
 * @param numen a pointer to the numen
 * @param desc a string with the graphic description (max 6 chars for F4)
 * @return OK, if everything goes well, or ERROR if there was some mistake
 */
Status numen_set_gdesc (Numen* numen, char* gdesc); /*Será una línea de texto*/
/**
 * @brief It gets a copy of the graphic description of a numen (caller must free it)
 * @author Rafael, Javier, Salvador and Violeta
 *
 * @param numen a pointer to the numen
 * @return a copy of the gdesc, or NULL if numen is NULL
 */
char* numen_get_gdesc (Numen* numen);

/*======== (set/get) health ================*/
/**
 * @brief It sets the health of a numen
 * @author Rafael, Javier, Salvador and Violeta
 *
 * @param numen a pointer to the numen
 * @param health the health points to set
 * @return OK, if everything goes well, or ERROR if out of bounds
 */
Status numen_set_health (Numen* numen, int health); /* tiene que ser mayor o igual a 0 siempre, sí fuera a ser negativo, se mantiene en 0 */
/**
 * @brief It gets the health of a numen
 * @author Rafael, Javier, Salvador and Violeta
 *
 * @param numen a pointer to the numen
 * @return the health value, or ERROR_LIFE if numen is NULL
 */
int numen_get_health (Numen* numen);

/*======== (set/get) attack ================*/
/**
 * @brief It sets the speed of the numen that this numen is following
 * @author Rafael, Javier, Salvador and Violeta
 *
 * @param numen a pointer to the numen
 * @return OK, if everything goes well, or ERROR if out of bounds
 */
Status numen_set_attack (Numen* numen, int attack);
/**
 * @brief It gets the speed of the numen that this numen is following
 * @author Rafael, Javier, Salvador and Violeta
 *
 * @param numen a pointer to the numen
 * @return the speed of the numen being followed, or 0 if numen is NULL
 */
int numen_get_attack (Numen* numen);

/*======== (set/get) energy ================*/
/**
 * @brief It sets the energy value of a numen
 * @author Rafael, Javier, Salvador and Violeta
 *
 * @param numen a pointer to the numen
 * @param energy the energy value to set
 * @return OK, if everything goes well, or ERROR if there was some mistake
 */
Status numen_set_energy (Numen* numen, int energy);
/**
 * @brief It gets the energy value of a numen
 * @author Rafael, Javier, Salvador and Violeta
 *
 * @param numen a pointer to the numen
 * @return the energy value, or ERROR_ENGY if numen is NULL
 */
int numen_get_energy (Numen* numen);

/*======== (set/get) speed ================*/
/**
 * @brief It sets the speed of the numen that this numen is following
 * @author Rafael, Javier, Salvador and Violeta
 *
 * @param numen a pointer to the numen
 * @return OK, if everything goes well, or ERROR if out of bounds
 */
Status numen_set_speed (Numen* numen, int speed);
/**
 * @brief It gets the speed of the numen that this numen is following
 * @author Rafael, Javier, Salvador and Violeta
 *
 * @param numen a pointer to the numen
 * @return the speed of the numen being followed, or 0 if numen is NULL
 */
int numen_get_speed (Numen* numen);

/*======== (set/get) skills ================*/
/**
 * @brief It set the skill skill of the numen
 * @author Rafael, Javier, Salvador and Violeta
 *
 * @param numen a pointer to the numen
 * @param skill_id the id of the new skill
 * @return OK, if everything goes well, or ERROR if out of bounds
 */
Status  numen_add_skill (Numen* numen, Id skill_id);
/**
 * @brief It check if the numen has the skill or not
 * @author Rafael, Javier, Salvador and Violeta
 *
 * @param numen a pointer to the numen
 * @param skill_id the id of the new skill to shearch
 * @return TRUE if the skill exists, FALSE if not
 */
Bool  numen_get_skill_by_id (Numen* numen, Id skill_id);
/**
 * @brief It gets the id of a skill with the index
 * @author Rafael, Javier, Salvador and Violeta
 *
 * @param numen a pointer to the numen
 * @param skill_indx the index of the new skill to shearch
 * @return The id of the skill, NO_SKILL otherwise
 */
Skills_id   numen_get_skill_by_index (Numen* numen, int skill_indx);

/*======== (set/get) following ================*/
/**
 * @brief It set the following of the numen
 * @author Rafael, Javier, Salvador and Violeta
 *
 * @param numen a pointer to the numen
 * @param following the id of the following of the numen
 * @return OK, if everything goes well, or ERROR if out of bounds
 */
Status numen_set_following (Numen* numen, Id following);
/**
 * @brief It gets the id of the following of the numen
 * @author Rafael, Javier, Salvador and Violeta
 *
 * @param numen a pointer to the numen
 * @return The id of the following, NO_ID otherwise
 */
Id numen_get_following (Numen* numen);

/*======= (set/get) Id ========================*/
/**
 * @brief It set the id of the numen
 * @author Rafael, Javier, Salvador and Violeta
 *
 * @param numen a pointer to the numen
 * @param new_id the new id of the numen to set
 * @return OK, if everything goes well, or ERROR if out of bounds
 */
Status numen_set_id (Numen* numen, Id new_id);
/**
 * @brief It gets the id of a numen
 * @author Rafael, Javier, Salvador and Violeta
 *
 * @param numen a pointer to the numen
 * @return the numen's id, or NO_ID if numen is NULL
 */
Id numen_get_id (Numen* numen);

/*====== (set/get) corrupt ===================*/
/**
 * @brief It set the corrupt of the numen
 * @author Rafael, Javier, Salvador and Violeta
 *
 * @param numen a pointer to the numen
 * @param corrupt if the numen is corrupt or not
 * @return OK, if everything goes well, or ERROR if out of bounds
 */
Status numen_set_corrupt (Numen* numen, Bool corrupt);
/**
 * @brief It checks if the numen is corrupt
 * @author Rafael, Javier, Salvador and Violeta
 *
 * @param numen a pointer to the numen
 * @return TRUE if the numen is corrupt, FALSE otherwise
 */
Bool numen_get_corrupt (Numen* numen);


Status numen_print (Numen* numen);
#endif /* NUMEN_H */