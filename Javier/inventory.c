/**
 * @brief It implements the inventory module
 *
 * @file inventory.c
 * @author Violeta, Rafael y Salvador
 * @version 2
 * @date 08-04-2026
 * @copyright GNU Public License
 */

#include "inventory.h"
#include "set.h"
#include "types.h"

/**
 * @brief Inventory
 *
 * This struct stores a collection of object IDs with a maximum
 * capacity.  Internally it uses a Set to ensure uniqueness.
 */
struct _Inventory
{
    Set* objs;    /*!< Set of object IDs stored in the inventory */
    int max_objs; /*!< Maximum number of objects allowed */
};

/* ================================================================== */
/*                     CREATE / DESTROY                               */
/* ================================================================== */

Inventory*
inventory_create ()
{
    Inventory* newInventory = NULL;

    newInventory            = (Inventory*)calloc (1, sizeof (Inventory));
    if (newInventory == NULL) return NULL;

    /* FIX: actually create the Set so it's not NULL */
    newInventory->objs = set_creat ();
    if (newInventory->objs == NULL)
        {
            free (newInventory);
            return NULL;
        }

    newInventory->max_objs = 0;

    return newInventory;
}

Status
inventory_destroy (Inventory* inventory)
{
    if (!inventory) return ERROR;

    /* FIX: free the internal Set first */
    if (inventory->objs) { set_destroy (inventory->objs); }

    free (inventory);
    return OK;
}

/* ================================================================== */
/*                    ADD / DELETE / CONTAINS                          */
/* ================================================================== */

Status
inventory_add (Inventory* inventory, Id new_id)
{
    if (inventory == NULL || new_id == NO_ID) return ERROR;

    /* Check capacity before adding */
    if (set_get_n_ids (inventory->objs) >= inventory->max_objs) return FULL_OF_LOVE;

    return set_add (inventory->objs, new_id);
}

Status
inventory_delete_obj (Inventory* inventory, Id trash_id)
{
    if (!inventory || trash_id == NO_ID) return ERROR;

    return set_delete_id (inventory->objs, trash_id);
}

Bool
inventory_contains_object (Inventory* inventory, Id obj)
{
    if (!inventory || obj == NO_ID) return FALSE;

    return set_contains_id (inventory->objs, obj);
}

/* ================================================================== */
/*                       GETTERS / SETTERS                            */
/* ================================================================== */

Status
inventory_set_max_objs (Inventory* inventory, int max)
{
    if (!inventory || max < 0) return ERROR;
    inventory->max_objs = max;
    return OK;
}

int
inventory_get_max_objs (Inventory* inventory)
{
    if (!inventory) return -1;
    return inventory->max_objs;
}

int
inventory_get_n_ids (Inventory* inventory)
{
    if (!inventory) return -1;
    return set_get_n_ids (inventory->objs);
}

Id
inventory_get_id_at (Inventory* inventory, int position)
{
    if (!inventory || position < 0 || position >= inventory_get_n_ids (inventory)) return NO_ID;

    return set_get_id_at (inventory->objs, position);
}

/* ================================================================== */
/*                            PRINT                                   */
/* ================================================================== */

Status inventory_print (Inventory* inventory)
{
    if (!inventory) return ERROR;

    printf (stdout, "\n--- Inventory ---\n");
    Status status = set_print (stdout, inventory->objs);
    printf (stdout, " ->max_objs: %i;\n", inventory->max_objs);
    return status;
}