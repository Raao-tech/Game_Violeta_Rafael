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
 * @brief It creates a new set, initializing the ID array to NULL and counter to 0.
 * @author Rafael
 *
 * @return Pointer to the new set, or NULL if memory allocation fails.
 */
Set*    set_creat();

/**
 * @brief It destroys a set, freeing the internal ID array and the set structure itself.
 * @author Rafael
 *
 * @param pset Pointer to the set to be destroyed.
 * @return OK if everything goes well, or ERROR if the pointer is NULL.
 */
Status  set_destroy(Set *pset);

/**
 * @brief This function checks whether or not there are IDs in the set.
 * @author Rafael
 *
 * @param pset Pointer to the set object.
 * @return TRUE if it is empty or NULL, FALSE when there are elements.
 */
Bool	set_is_empty(Set *pset);
/**
 * @brief This function checks whether or not there are IDs in the set.
 * @author Rafael
 *
 * @param pset Pointer to the set object.
 * @param _id Value of id reference.
 * @return TRUE if it is in set, FALSE when there are elements.
 */
Bool	set_contains_id(Set *pset, Id _id);

/**
 * @brief Adds a new ID to the set by reallocating memory for the internal array.
 * @author Rafael
 *
 * @param pset Pointer to the set.
 * @param new_id The ID to be added.
 * @return OK if the ID was added successfully, or ERROR if pset is NULL or memory fails.
 */
Status	set_add(Set* pset, Id new_id);

/**
 * @brief Extracts the last ID added to the set (LIFO) and reduces the allocated memory.
 * @author Rafael
 *
 * @param pset Pointer to the set.
 * @return The extracted ID, or NO_ID if the set is empty or NULL.
 */
Id	set_pop(Set* pset);

/**
 * @brief Prints the contents of the set to the specified output stream.
 * @author Rafael
 *
 * @param output File pointer (e.g., stdout) where the set will be printed.
 * @param pset Pointer to the set to be printed.
 * @return OK if successful, or ERROR if pset is NULL.
 */
Status	set_print(FILE* output, Set* pset);

#endif
