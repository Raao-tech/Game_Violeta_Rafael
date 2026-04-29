/**
 * @brief It defines the numen module interface
 *
 * @file Numens.h
 * @author Rafa and Javier Jarque
 * @version 1.2
 * @date 18-04-2026
 * @copyright GNU Public License
 */

#ifndef NUMEN_H
#define NUMEN_H

#include "types.h"
#include "character.h"
#include "skills.h"

typedef struct _Numen Numen;

/*======== (Create/Destroy) Numen ================*/
/**
 * @brief It creates a new numen, allocating memory and initializing it
 * @author Violeta y Rafa
 *
 * @return a new numen, initialized, or NULL if memory fails
 */
Numen* numen_create();         

/**
 * @brief It destroys a numen, freeing the allocated memory
 * @author Violeta y Rafa
 *
 * @param nuemn a pointer to the numen
 * @return OK, if everything goes well, or ERROR if there was some mistake
 */            
Status numen_destroy(Numen* numen);

/*======== (set/get) name (char*) ================*/
/**
 * @brief It sets the name in the numen
 * @author Violeta y Rafa
 *
 * @param nuemn a pointer to the numen
 * @param name a pointer to the name
 * @return OK, if everything goes well, or ERROR if there was some mistake
 */          
Status numen_set_name(Numen* numen, char* name);

/**
 * @brief It gets the name of the numen
 * @author Violeta y Rafa
 *
 * @param id the id of the numen
 * @return the name of the nuemen
 */
char*  numen_get_name(Numen* numen);

/**
 * @brief It gets if the numen has a name or not
 * @author Violeta y Rafa
 *
 * @param id the id of the numen
 * @param name the name to compare
 * @return a bool with TRUE if the numen has the name or FALSE if not
 */
Bool numen_has_name(Numen *numen, char* name);

/*======== (set/get) is_errante (Bool) ================*/
/**
 * @brief It gets if the numen is errant or not
 * @author Violeta y Rafa
 *
 * @param id the id of the numen
 * @return a bool with TRUE if the numen is errant or FALSE if not
 */
Bool numen_is_errant(Numen* numen);

/*======== (set/get) position ================*/
/**
 * @brief It sets the position in the numen
 * @author Violeta y Rafa
 *
 * @param nuemn a pointer to the numen
 * @param x the new value of x
 * @param y the new value of y
 * @return OK, if everything goes well, or ERROR if there was some mistake
 */     
Status numen_set_position(Numen *numen, int x, int y);

/**
 * @brief It sets the x position in the numen
 * @author Violeta y Rafa
 *
 * @param nuemn a pointer to the numen
 * @param x the new value of x
 * @return OK, if everything goes well, or ERROR if there was some mistake
 */   
Status numen_set_pos_x(Numen* numen, int pos_x);

/**
 * @brief It gets the x value of the numen
 * @author Violeta y Rafa
 *
 * @param id the id of the numen
 * @return the x value of the nuemen
 */
int    numen_get_pos_x(Numen* numen);

/**
 * @brief It sets the y position in the numen
 * @author Violeta y Rafa
 *
 * @param nuemn a pointer to the numen
 * @param y the new value of y
 * @return OK, if everything goes well, or ERROR if there was some mistake
 */   
Status numen_set_pos_y(Numen* numen, int pos_y);

/**
 * @brief It gets the y value of the numen
 * @author Violeta y Rafa
 *
 * @param id the id of the numen
 * @return the y value of the nuemen
 */
int    numen_get_pos_y(Numen* numen);

/*======== (set/get) gdesc() ================*/
/**
 * @brief It sets the gdesc in the numen
 * @author Violeta y Rafa
 *
 * @param nuemn a pointer to the numen
 * @param gdesc a pointer to the gdesc
 * @return OK, if everything goes well, or ERROR if there was some mistake
 */   
Status numen_set_gdesc(Numen* numen, char* gdesc);

/**
 * @brief It gets the gdesc of the numen
 * @author Violeta y Rafa
 *
 * @param id the id of the numen
 * @return the gdesc of the nuemen
 */
char*  numen_get_gdesc(Numen* numen);

/*======== (set/get) health ================*/
/**
 * @brief It sets the health in the numen
 * @author Violeta y Rafa
 *
 * @param nuemn a pointer to the numen
 * @param health the new health of the numen
 * @return OK, if everything goes well, or ERROR if there was some mistake
 */   
Status numen_set_health(Numen* numen, int health);

/**
 * @brief It gets the health of the numen
 * @author Violeta y Rafa
 *
 * @param id the id of the numen
 * @return the health of the numen
 */
int    numen_get_health(Numen* numen);

/*======== (set/get) attack ================*/
/**
 * @brief It sets the attack in the numen
 * @author Violeta y Rafa
 *
 * @param nuemn a pointer to the numen
 * @param attack the new attack of the numen
 * @return OK, if everything goes well, or ERROR if there was some mistake
 */  
Status numen_set_attack(Numen* numen, int attack);

/**
 * @brief It gets the attack of the numen
 * @author Violeta y Rafa
 *
 * @param id the id of the numen
 * @return the attack of the numen
 */
int    numen_get_attack(Numen* numen);

/*======== (set/get) energy ================*/
/**
 * @brief It sets the energy in the numen
 * @author Violeta y Rafa
 *
 * @param nuemn a pointer to the numen
 * @param energy the new energy of the numen
 * @return OK, if everything goes well, or ERROR if there was some mistake
 */
Status numen_set_energy(Numen* numen, int energy);

/**
 * @brief It gets the energy of the numen
 * @author Violeta y Rafa
 *
 * @param id the id of the numen
 * @return the energy of the numen
 */
int    numen_get_energy(Numen* numen);

/*======== (set/get) speed ================*/
/**
 * @brief It sets the speed in the numen
 * @author Violeta y Rafa
 *
 * @param nuemn a pointer to the numen
 * @param speed the new speed of the numen
 * @return OK, if everything goes well, or ERROR if there was some mistake
 */
Status numen_set_speed(Numen* numen, int speed);

/**
 * @brief It gets the speed of the numen
 * @author Violeta y Rafa
 *
 * @param speed the speed of the numen
 * @return the speed of the numen
 */
int    numen_get_speed(Numen* numen);

/*======== (set/get) skills ================*/
/**
 * @brief It sets the id in the numen
 * @author Violeta y Rafa
 *
 * @param nuemn a pointer to the numen
 * @param skill_id the id new skill of the numen
 * @return OK, if everything goes well, or ERROR if there was some mistake
 */
Status numen_set_skill(Numen* numen, Id skill_id); 

/**
 * @brief It gets the id of the numen
 * @author Violeta y Rafa
 *
 * @param id the id of the numen
 * @return the id of the skill of the numen
 */
Id    numen_get_skill(Numen* numen, Id skill_id);

/*======== (set/get) following ================*/
/**
 * @brief It sets the id of the new following in the numen
 * @author Violeta y Rafa
 *
 * @param nuemn a pointer to the numen
 * @param following the id of the new following numen
 * @return OK, if everything goes well, or ERROR if there was some mistake
 */
Status numen_set_following(Numen* numen, Id following);

/**
 * @brief It gets the id of the following numen
 * @author Violeta y Rafa
 *
 * @param id the id of the numen
 * @return the id of the following numen
 */
Id   numen_get_following(Numen* numen);

/*======= (set/get) Id ========================*/
/**
 * @brief It gets the id of the numen
 * @author Violeta y Rafa
 *
 * @param id the id of the numen
 * @param new_id the new id of the numen
 * @return OK, if everything goes well, or ERROR if there was some mistake
 */
Status numen_set_id(Numen *numen, Id new_id);

/**
 * @brief It gets the id of the numen
 * @author Violeta y Rafa
 *
 * @param id the id of the numen
 * 
 * @return the id of the numen
 */
Id     numen_get_id(Numen *numen);

/*====== (set/get) corrupt ===================*/
/**
 * @brief It sets ig the game is corrupt or not
 * @author Violeta y Rafa
 *
 * @param id the id of the numen
 * @param corrupt if the numen is corrupt or not
 * @return OK, if everything goes well, or ERROR if there was some mistake
 */
Status numen_set_corrupt(Numen *numen, Bool corrupt);

/**
 * @brief It gets if the numen is corrupt or not
 * @author Violeta y Rafa
 *
 * @param id the id of the numen
 * 
 * @return a bool with if the numen is corrupt or not
 */
Bool numen_get_corrupt(Numen *numen);

/**
 * @brief It prints the full numen state to stdout for debugging
 * @author Violeta, Rafa and Salvador
 *
 * Shows all players (with turn), spaces, objects, characters, links,
 * and whether the numen is finished.
 *
 * @param numen a pointer to the numen
 * @return OK, if everything goes well, or ERROR if there was some mistake
 */
Status numen_print(Numen *numen);
#endif /* NUMEN_H */