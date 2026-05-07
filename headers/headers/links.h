/**
 * @brief It defines the TAD links
 *
 * @file links.h
 * @author Rafa
 * @version 0
 * @date 23-03-2026
 * @copyright GNU Public License
 */

#ifndef LINKS_H
#define LINKS_H

#include "types.h"

typedef struct _Links Links;

/* ============== (Create/Destroy)  Link ====================== */
/**
 * @brief It creates a new link, allocating memory and initializing it
 * @author Rafa
 *
 * @return a new link, initialized
 */
Links* link_create ();

/**
 * @brief It destroys a link, freeing the allocated memory
 * @author Rafa
 *
 * @param link a pointer to the object
 * @return OK, if everything goes well, or ERROR if there was some mistake
 */
Status link_destroy (Links* link);

/* ============== (set/get)  Id ====================== */
/**
 * @brief It sets the id of a link
 * @author Rafa
 *
 * @param link a pointer to the link
 * @param id_new the id of the link
 * @return OK, if everything goes well, or ERROR if there was some mistake
 */
Status link_set_id (Links* link, Id id_new);

/**
 * @brief It gets the id of a link
 * @author Rafa
 *
 * @param link a pointer to the link
 * @return the id of link
 */
Id link_get_id (Links* link);

/* ============== (set/get)  Name ====================== */
/**
 * @brief It sets the name of a link
 * @author Rafa
 *
 * @param link a pointer to the link
 * @param name the name of the link
 * @return OK, if everything goes well, or ERROR if there was some mistake
 */
Status link_set_name (Links* link, char* name);

/**
 * @brief It gets the name of a link
 * @author Rafa
 *
 * @param link a pointer to the link
 * @return a string with the name of the link
 */
char* link_get_name (Links* link);

/**
 * @brief It say equal or no your name
 * @author Rafa
 *
 * @param link a pointer to the link
 * @return  TRUE if names are equal, and FALSE and other case
 */
Bool link_has_name (Links* link, char* name);
/* ============== (set/get) status ====================== */
/**
 * @brief It sets the status of a link
 * @author Rafa
 *
 * @param link a pointer to the link
 * @param status the status of the link
 * @return OK, if everything goes well, or ERROR if there was some mistake
 */
Status link_set_status (Links* link, Bool status);
/**
 * @brief It gets the status of a link
 * @author Rafa
 *
 * @param link a pointer to the link
 * @return the status of the link, FALSE in case of error
 */
Bool link_get_status (Links* link);

/* ============== (set/get) origin/destination ====================== */
/**
 * @brief It sets the origin id of a link
 * @author Rafa
 *
 * @param link a pointer to the link
 * @param id_orig the origin id of the link
 * @return OK, if everything goes well, or ERROR if there was some mistake
 */
Status link_set_origin_id (Links* link, Id id_orig);
/**
 * @brief It gets the origin id of a link
 * @author Rafa
 *
 * @param link a pointer to the link
 * @return the origin id of link
 */
Id link_get_origin_id (Links* link);

/**
 * @brief It sets the destination id of a link
 * @author Rafa
 *
 * @param link a pointer to the link
 * @param id_orig the destination id of the link
 * @return OK, if everything goes well, or ERROR if there was some mistake
 */
Status link_set_destination_id (Links* link, Id id_dest);
/**
 * @brief It gets the destination id of a link
 * @author Rafa
 *
 * @param link a pointer to the link
 * @return the destination id of link
 */
Id link_get_destination_id (Links* link);

/* ============== (set/get) status (dest_to_origin/ origin_to_dest) ====================== */
/**
 * @brief It sets whether the link is open from destination to origin
 * @author Rafa
 *
 * @param link a pointer to the link
 * @param status the open status of the link
 * @return OK, if everything goes well, or ERROR if there was some mistake
 */
Status link_set_open_dest_to_origin (Links* link, Bool status);
/**
 * @brief It gets whether the link is open from destination to origin
 * @author Rafa
 *
 * @param link a pointer to the link
 * @return TRUE if open, FALSE otherwise or in case of error
 */
Bool link_get_open_dest_to_origin (Links* link);

/**
 * @brief It sets whether the link is open from origin to destination
 * @author Rafa
 *
 * @param link a pointer to the link
 * @param status the open status of the link
 * @return OK, if everything goes well, or ERROR if there was some mistake
 */
Status link_set_open_origin_to_dest (Links* link, Bool status);
/**
 * @brief It gets whether the link is open from origin to destination
 * @author Rafa
 *
 * @param link a pointer to the link
 * @return TRUE if open, FALSE otherwise or in case of error
 */
Bool link_get_open_origin_to_dest (Links* link);

/* ============== (set/get/opposite) direction ====================== */
/**
 * @brief It sets the direction of a link
 * @author Rafa
 *
 * @param link a pointer to the link
 * @param dir the direction of the link
 * @return OK, if everything goes well, or ERROR if there was some mistake
 */
Status link_set_direction (Links* link, Direction dir);
/**
 * @brief It gets the direction of a link
 * @author Rafa
 *
 * @param link a pointer to the link
 * @return the direction of the link
 */
Direction link_get_direction (Links* link);
/**
 * @brief It gets the opposite direction of a link
 * @author Rafa
 *
 * @param link a pointer to the link
 * @return the opposite direction of the link
 */
Direction link_get_opposite_direction (Links* link);

/* ============== PRINT ====================== */
/**
 * @brief It prints the information of a link
 * @author Rafa
 *
 * @param link a pointer to the link
 * @param output the file where to print
 * @return OK, if everything goes well, or ERROR if there was some mistake
 */
Status link_print (Links* link, FILE* output);

#endif
