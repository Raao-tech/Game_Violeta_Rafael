/**
 * @brief It defines the set module interface
 *
 * @file set.h
 * @author Rafael
 * @version 2
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
 * @brief It checks whether a given ID exists inside the set.
 * @author Rafael
 *
 * @param pset Pointer to the set.
 * @param _id The ID to search for.
 * @return TRUE if the ID is found, FALSE otherwise or if pset is NULL.
 */
Bool    set_contains_id(Set *pset, Id _id);

/**
 * @brief Adds a new ID to the set by reallocating memory for the internal array.
 *        If the ID already exists in the set, it does nothing and returns OK.
 * @author Rafael
 *
 * @param pset Pointer to the set.
 * @param new_id The ID to be added.
 * @return OK if the ID was added successfully or already existed, ERROR if pset is NULL, new_id is NO_ID, or memory fails.
 */
Status  set_add(Set *pset, Id new_id);

/**
 * @brief Removes a specific ID from the set, shifting elements to close the gap and reducing the allocated memory.
 * @author Rafael
 *
 * @param pset Pointer to the set.
 * @param trash_id The ID to be removed.
 * @return OK if removed successfully, ERROR if pset is NULL, trash_id is NO_ID, or the ID was not found.
 */
Status  set_delete_id(Set *pset, Id trash_id);

/**
 * @brief Returns the number of IDs currently stored in the set.
 * @author Rafael
 *
 * @param pset Pointer to the set.
 * @return Number of IDs, or -1 if pset is NULL.
 */
int     set_get_n_ids(Set *pset);

/**
 * @brief Prints the contents of the set to the specified output stream.
 * @author Rafael
 *
 * @param output File pointer (e.g., stdout) where the set will be printed.
 * @param pset Pointer to the set to be printed.
 * @return OK if successful, or ERROR if pset is NULL.
 */
Status  set_print(FILE *output, Set *pset);

#endif