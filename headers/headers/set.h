/**
 * @brief It defines the set module interface
 *
 * A Set is an abstract data type that stores a dynamic collection
 * of unique identifiers (Id). It does not allow duplicates and
 * does not consider order. Internally it uses a dynamically
 * allocated array of Ids that grows and shrinks with realloc.
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

/**
 * @brief Set ADT (opaque type)
 *
 * The internal structure contains:
 *   - Id *ids   : dynamically allocated array of identifiers
 *   - int n_ids : current number of identifiers stored
 */
typedef struct _Set Set;

/**
 * @brief It creates a new empty set, initializing the ID array to NULL and the counter to 0.
 * @author Rafael
 *
 * @return Pointer to the new set, or NULL if memory allocation fails.
 */
Set* set_creat ();

/**
 * @brief It destroys a set, freeing the internal ID array and the set structure itself.
 * @author Rafael
 *
 * @param pset Pointer to the set to be destroyed.
 * @return OK if everything goes well, or ERROR if the pointer is NULL.
 */
Status set_destroy (Set* pset);

/**
 * @brief It checks whether a given ID exists inside the set.
 * @author Rafael
 *
 * Iterates through the internal array comparing each stored Id
 * with the reference Id provided.
 *
 * @param pset Pointer to the set.
 * @param ref_id The ID to search for.
 * @return TRUE if the ID is found, FALSE otherwise or if pset is NULL.
 */
Bool set_contains_id (Set* pset, Id ref_id);

/**
 * @brief Adds a new ID to the set by reallocating memory for the internal array.
 * @author Rafael
 *
 * If the ID already exists in the set, it does nothing and returns OK
 * (a set does not allow duplicates). Otherwise, it grows the array by
 * one position using realloc and places the new Id at the end.
 *
 * @param pset Pointer to the set.
 * @param new_id The ID to be added.
 * @return OK if the ID was added successfully or already existed,
 *         ERROR if pset is NULL, new_id is NO_ID, or memory allocation fails.
 */
Status set_add (Set* pset, Id new_id);

/**
 * @brief Removes a specific ID from the set.
 * @author Rafael
 *
 * Finds the position of trash_id in the array, shifts all subsequent
 * elements one position to the left to close the gap, and then reduces
 * the allocated memory by one position using realloc.
 *
 * @param pset Pointer to the set.
 * @param trash_id The ID to be removed.
 * @return OK if removed successfully, ERROR if pset is NULL,
 *         trash_id is NO_ID, or the ID was not found in the set.
 */
Status set_delete_id (Set* pset, Id trash_id);

/**
 * @brief Returns the number of IDs currently stored in the set.
 * @author Rafael
 *
 * @param pset Pointer to the set.
 * @return Number of IDs (>= 0), or ERROR_MAIN (-1) if pset is NULL.
 */
int set_get_n_ids (Set* pset);

/**
 * @brief Returns the ID at a given position in the set.
 * @author Rafael
 *
 * This function allows external modules to iterate over the contents
 * of the set without breaking encapsulation. The position is a
 * zero-based index into the internal array (0 to n_ids - 1).
 *
 * @param pset Pointer to the set.
 * @param position Index in the internal array (0 to n_ids - 1).
 * @return The Id at that position, or NO_ID if out of range or pset is NULL.
 */
Id set_get_id_at (Set* pset, int position);

/**
 * @brief Prints the contents of the set to the specified output stream.
 * @author Rafael
 *
 * Displays each stored Id with its position number. Useful for
 * debugging purposes.
 *
 * @param output File pointer (e.g., stdout) where the set will be printed.
 * @param pset Pointer to the set to be printed.
 * @return OK if successful, or ERROR if pset is NULL.
 */
Status set_print (FILE* output, Set* pset);

#endif