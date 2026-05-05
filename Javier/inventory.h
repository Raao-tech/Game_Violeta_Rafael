/**
 * @brief It defines the inventory module interface
 *
 * @file inventory.h
 * @author Violeta, Rafael y Salvador
 * @version 2
 * @date 08-04-2026
 * @copyright GNU Public License
 */

#ifndef INVENTORY_H
#define INVENTORY_H

#include "types.h"

typedef struct _Inventory Inventory;

/**
 * @brief Creates a new empty inventory
 * @author Violeta
 *
 * Allocates memory for the Inventory struct, creates an internal
 * Set for object IDs, and initializes max_objs to 0.
 *
 * @return pointer to the new inventory, or NULL if allocation fails
 */
Inventory* inventory_create ();

/**
 * @brief Destroys an inventory, freeing all allocated memory
 * @author Violeta
 *
 * @param inventory pointer to the inventory to destroy
 * @return OK if successful, ERROR if inventory is NULL
 */
Status inventory_destroy (Inventory* inventory);

/**
 * @brief Adds an object ID to the inventory
 * @author Rafael
 *
 * @param inventory pointer to the inventory
 * @param new_id the ID to add
 * @return OK if added, FULL_OF_LOVE if at capacity, ERROR if NULL/NO_ID
 */
Status inventory_add (Inventory* inventory, Id new_id);

/**
 * @brief Removes an object ID from the inventory
 * @author Rafael
 *
 * @param inventory pointer to the inventory
 * @param trash_id the ID to remove
 * @return OK if removed, ERROR if not found or NULL
 */
Status inventory_delete_obj (Inventory* inventory, Id trash_id);

/**
 * @brief Checks whether the inventory contains a specific object ID
 * @author Salvador
 *
 * @param inventory pointer to the inventory
 * @param obj the ID to search for (value, NOT pointer)
 * @return TRUE if found, FALSE otherwise
 */
Bool inventory_contains_object (Inventory* inventory, Id obj);

/**
 * @brief Sets the maximum number of objects the inventory can hold
 * @author Rafael
 *
 * @param inventory pointer to the inventory
 * @param max the maximum capacity (must be >= 0)
 * @return OK if successful, ERROR if inventory is NULL or max < 0
 */
Status inventory_set_max_objs (Inventory* inventory, int max);

/**
 * @brief Gets the maximum capacity of the inventory
 * @author Rafael
 *
 * @param inventory pointer to the inventory
 * @return max capacity, or -1 if inventory is NULL
 */
int inventory_get_max_objs (Inventory* inventory);

/**
 * @brief Gets the current number of objects in the inventory
 * @author Violeta
 *
 * @param inventory pointer to the inventory
 * @return number of objects, or -1 if inventory is NULL
 */
int inventory_get_n_ids (Inventory* inventory);

/**
 * @brief Gets the ID at a specific position in the inventory
 * @author Salvador
 *
 * @param inventory pointer to the inventory
 * @param position zero-based index
 * @return the ID at that position, or NO_ID if out of range
 */
Id inventory_get_id_at (Inventory* inventory, int position);

/**
 * @brief Prints the inventory contents to stdout
 * @author Javier Jarque
 *
 * @param inventory pointer to the inventory
 * @return OK if successful, ERROR if inventory is NULL
 */
Status inventory_print (Inventory* inventory);

#endif