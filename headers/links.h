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
Links*       link_create();
Status       link_destroy(Links *link);


/* ============== (set/get)  Id ====================== */
Status       link_set_id(Links *link, Id id_new);
Id           link_get_id(Links *link);


/* ============== (set/get)  Name ====================== */
Status       link_set_name(Links *link, char *name);
char*        link_get_name(Links *link);


/* ============== (set/get) status ====================== */
Status       link_set_status(Links *link, Bool status);
Bool         link_get_status(Links *link);


/* ============== (set/get) origin/destination ====================== */
Status       link_set_origin_id(Links *link, Id id_orig);
Id           link_get_origin_id(Links *link);

Status       link_set_destination_id(Links *link, Id id_dest);
Id           link_get_destination_id(Links *link);


/* ============== (set/get) status (dest_to_origin/ origin_to_dest) ====================== */
Status       link_set_open_dest_to_origin(Links *link, Bool status);
Bool         link_get_open_dest_to_origin(Links *link);

Status       link_set_open_origin_to_dest(Links *link, Bool status);
Bool         link_get_open_origin_to_dest(Links *link);


/* ============== (set/get/oppsite) direction ====================== */
Status       link_set_direction(Links *link, Direction dir);
Direction    link_get_direction(Links *link);
Direction    link_get_oppsite_direction(Links *link);


/* ============== PRINT ====================== */
Status       link_print(Links *link, FILE* output);


#endif