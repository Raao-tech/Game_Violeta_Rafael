/**
 * @brief It implements the links struct
 *
 * @file links.c
 * @author Violeta y Rafael
 * @version 2
 * @date 08-04-2026
 * @copyright GNU Public License
 */

#include "links.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Links
 *
 * This struct stores a connection between two spaces.
 * The bidirectional model uses two separate open/closed states:
 * one for origin→dest and one for dest→origin.
 */
struct _Links
{
    char* name;          /*!< Name of the link */
    Direction direction; /*!< Direction from origin to dest (N, S, E, W) */
    Id id;               /*!< Unique id of this link */
    Id origin;           /*!< Id of the origin space */
    Id destiny;          /*!< Id of the destination space */
    Bool status_orig;    /*!< Can you go from dest to origin? */
    Bool status_dest;    /*!< Can you go from origin to dest? */
};

/* ========== Create / Destroy ========== */

Links*
link_create ()
{
    Links* newlink = (Links*)malloc (sizeof (Links));
    if (!newlink) return NULL;

    newlink->name        = NULL;
    newlink->destiny     = NO_ID;
    newlink->origin      = NO_ID;
    newlink->id          = NO_ID;
    newlink->direction   = U;
    newlink->status_dest = TRUE;
    newlink->status_orig = TRUE;

    return newlink;
}

Status
link_destroy (Links* link)
{
    if (!link) return ERROR;

    if (link->name)
        {
            free (link->name);
            link->name = NULL;
        }

    free (link);
    return OK;
}

/* ========== Id ========== */

Status
link_set_id (Links* link, Id id_new)
{
    if (!link || id_new == NO_ID) return ERROR;
    link->id = id_new;
    return OK;
}

Id
link_get_id (Links* link)
{
    if (!link) return NO_ID;
    return link->id;
}

/* ========== Name ========== */

Status
link_set_name (Links* link, char* name)
{
    if (!link || !name) return ERROR;

    /* Free previous name if it exists */
    if (link->name)
        {
            free (link->name);
            link->name = NULL;
        }

    link->name = strdup (name);
    if (!link->name) return ERROR;

    return OK;
}

char*
link_get_name (Links* link)
{
    if (!link || !link->name) return NULL;
    return strdup (link->name);
}
/*Será falso a no ser que coincidan*/
Bool
link_has_name (Links* link, char* name)
{
    Bool is_equal = FALSE;
    if (!link || !name) return is_equal;

    char* name_link = link_get_name (link);

    if (strcmp (name_link, name) == 0) is_equal = TRUE;
    free (name_link);
    return is_equal;
}

/* ========== Generic status (sets/gets BOTH directions) ========== */

/*
 * BUG FIX: These two functions were declared in links.h but
 * had no implementation, causing a linker error if anyone
 * called them.
 *
 * link_set_status sets BOTH directions to the same value.
 * link_get_status returns TRUE only if the primary direction
 * (origin → dest) is open.
 */
Status
link_set_status (Links* link, Bool status)
{
    if (!link) return ERROR;
    link->status_dest = status;
    link->status_orig = status;
    return OK;
}

Bool
link_get_status (Links* link)
{
    if (!link) return FALSE;
    return link->status_dest;
}

/* ========== Bidirectional open/closed ========== */

Status
link_set_open_origin_to_dest (Links* link, Bool status)
{
    if (!link) return ERROR;
    link->status_dest = status;
    return OK;
}

Bool
link_get_open_origin_to_dest (Links* link)
{
    if (!link) return FALSE;
    return link->status_dest;
}

Status
link_set_open_dest_to_origin (Links* link, Bool status)
{
    if (!link) return ERROR;
    link->status_orig = status;
    return OK;
}

Bool
link_get_open_dest_to_origin (Links* link)
{
    if (!link) return FALSE;
    return link->status_orig;
}

/* ========== Direction ========== */

Status
link_set_direction (Links* link, Direction dir)
{
    if (!link || dir == U) return ERROR;
    link->direction = dir;
    return OK;
}

Direction
link_get_direction (Links* link)
{
    if (!link) return U;
    return link->direction;
}

Direction
link_get_opposite_direction (Links* link)
{
    if (!link) return U;

    switch (link->direction)
        {
        case N:
            return S;
        case S:
            return N;
        case E:
            return W;
        case W:
            return E;
        default:
            return U;
        }
}

/* ========== Origin / Destination ========== */

Status
link_set_origin_id (Links* link, Id id_orig)
{
    if (!link || id_orig == NO_ID) return ERROR;
    link->origin = id_orig;
    return OK;
}

Id
link_get_origin_id (Links* link)
{
    if (!link) return NO_ID;
    return link->origin;
}

Status
link_set_destination_id (Links* link, Id id_dest)
{
    if (!link || id_dest == NO_ID) return ERROR;
    link->destiny = id_dest;
    return OK;
}

Id
link_get_destination_id (Links* link)
{
    if (!link) return NO_ID;
    return link->destiny;
}

/* ========== Print ========== */

Status
link_print (Links* link, FILE* output)
{
    char* name = NULL;

    if (!output)
        {
            fprintf (stderr, "ERROR: NULL output stream in link_print\n");
            return ERROR;
        }
    if (!link)
        {
            fprintf (output, "ERROR: NULL link pointer in link_print\n");
            return ERROR;
        }

    name = link_get_name (link);

    fprintf (output, "=============== LINK ==================\n");
    fprintf (output, "  Name       : %s\n", name ? name : "NULL");
    fprintf (output, "  Id         : %ld\n", link->id);
    fprintf (output, "  Origin     : %ld\n", link->origin);
    fprintf (output, "  Destiny    : %ld\n", link->destiny);
    fprintf (output, "  Direction  : %d\n", link->direction);
    fprintf (output, "  Open O→D   : %d\n", link->status_dest);
    fprintf (output, "  Dir Oppos. : %d\n", link_get_opposite_direction (link));
    fprintf (output, "  Open D→O   : %d\n", link->status_orig);
    fprintf (output, "=======================================\n");

    if (name) free (name);

    return OK;
}