/**
 * @brief It defines the set module interface
 *
 * @file set.h
 * @author Rafael
 * @version 0
 * @date 28-02-2026
 * @copyright GNU Public License
 */

#ifndef SET_H
#define SET_H

#include "types.h"

typedef struct _Set Set;


/**
 * @brief It create a set generic, the allocated memory to all ids save on
 * @author Rafa
 *
 * @param n_max the max ids 
 * @return OK, if everything goes well, or ERROR if there was some mistake
 */
Set*    set_creat(int   n_max);

/**
 * @brief It destroy a set generic, and freeng allocated memory 
 * @author Rafa
 *
 * @param n_max the max ids 
 * @return OK, if everything goes well, or ERROR if there was some mistake
 */
Status    set_destroy(Set** ppset);

/**
 * @brief This function checks whether or not there are IDs in the stack 
 * @author Rafa
 *
 * @param pset Pointer to object with the stack of the ids
 * @return TRUE if is empty, FALSE when there are elements or the pointer is NULL.
 */
Bool	set_is_empty(Set *pset);


#endif