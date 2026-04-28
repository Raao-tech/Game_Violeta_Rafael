/**
 * @brief It implements the set structure
 *
 * @file set.c
 * @author Rafael Araujo
 * @version 0
 * @date 27-01-2025
 * @copyright GNU Public License
 */

#include "set.h"
#include "stdio.h"
#include "stdlib.h"

/**
 * @brief set
 *
 * This struct stores all the information of a set.
 */
struct _Set
{
    Id* ids;   /*!< Array of ids */
    int n_ids; /*!< Number of ids stored*/
};

/* create and destroy */
Set*
set_creat ()
{
    Set* newset = NULL;

    if ((newset = (Set*)malloc (sizeof (Set))) == NULL)
        {
            return NULL;
        }
    newset->n_ids = 0;
    newset->ids   = NULL;

    return newset;
}
Status
set_destroy (Set* pset)
{

    int i;

    if (!pset)
        {
            return ERROR;
        }

    if ((pset)->ids)
        {
            for (i = 0; i < (pset)->n_ids; i++)
                {
                    (pset)->ids[i] = NO_ID;
                }
            free ((pset)->ids);
            (pset)->ids = NULL;
        }

    (pset)->n_ids = 0;

    free ((pset));

    return OK;
}

/* private.  empty and reduce */
Bool
set_is_empty (Set* pset)
{

    if (pset == NULL) return FALSE;

    if (pset->n_ids <= 0) return TRUE;

    return FALSE;
}
Status
set_reduce (Set* pset, int dest_address)
{
    int i;
    Id* temp = NULL;
    if (!pset || set_is_empty (pset) == TRUE || !pset->ids || dest_address < 0 || dest_address >= pset->n_ids) return ERROR;

    /* Si somos los únicos */
    if (pset->n_ids == 1)
        {
            free (pset->ids);
            pset->ids   = NULL;
            pset->n_ids = 0;
            return OK;
        }

    /* nos posicionamos en el id inexistente */
    i = dest_address;
    /* Verficiamos que no seamos los últimos */
    while (((i) < pset->n_ids - 1))
        {
            pset->ids[i]     = pset->ids[i + 1]; // movemos el siguiente id a la posicion anterior
            pset->ids[i + 1] = NO_ID;            // el espacio del id lo igualamos a No_id
            i++;
        }

    // reducimos el tamaño del array de oids en una unidad
    temp = (Id*)realloc (pset->ids, sizeof (Id) * (pset->n_ids - 1));
    if (!temp) return ERROR;

    // Igualamos el puntero realocado en la vairable origianl y disminuimos la
    // cnatidad de ids
    pset->ids = temp;
    pset->n_ids--;
    return OK;
}

/* add, delete, contains_id, and get_n_ids */
Bool
set_contains_id (Set* pset, Id ref_id)
{
    int i;
    if (!pset) return FALSE;

    for (i = 0; i < pset->n_ids; i++)
        {
            if (pset->ids[i] == ref_id) return TRUE;
        }

    return FALSE;
}
Status
set_add (Set* pset, Id new_id)
{
    Id* pset_temp = NULL;
    /* If pset or new_id don't exist  */
    if (pset == NULL || new_id == NO_ID) return ERROR;

    if (set_contains_id (pset, new_id) == TRUE) return OK;

    // agregamos un espacio en el conjutno de objetos
    pset_temp = (Id*)realloc ((void*)pset->ids, (sizeof (Id) * (pset->n_ids + 1)));

    if (pset_temp == NULL) return ERROR;

    pset_temp[pset->n_ids] = new_id;
    pset->n_ids++;

    pset->ids = pset_temp;

    return OK;
}
Status
set_delete_id (Set* pset, Id trash_id)
{
    int i;
    if (!pset || trash_id == NO_ID) return ERROR;

    for (i = 0; i < pset->n_ids; i++)
        {
            if (pset->ids[i] == trash_id) return set_reduce (pset, i);
        }

    return ERROR;
}
int
set_get_n_ids (Set* pset)
{
    if (!pset) return ERROR_MAIN;
    return pset->n_ids;
}

/* get id at position (for external iteration) */
Id
set_get_id_at (Set* pset, int position)
{
    if (!pset || position < 0 || position >= pset->n_ids) return NO_ID;
    return pset->ids[position];
}

/* print */
Status
set_print (FILE* output, Set* pset)
{
    if (!pset)
        {
            fprintf (output, ROJO "ERROR: " RESET "There is nothing on the Set of ids\n");
            return ERROR;
        }

    fprintf (output, "**********The ids presents:**********\n");
    for (int i = 0; i < pset->n_ids; i++)
        {
            fprintf (output, "\tn_Id (%d):\t" ROJO "%ld" RESET "\n", (i + 1), pset->ids[i]);
        }
    fprintf (output, "**********END of the Ids*************\n");

    return OK;
}
