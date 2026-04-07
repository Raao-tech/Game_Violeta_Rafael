/**
 * @brief It implements the links struct
 *
 * @file link.c
 * @author Violeta
 * @version 0
 * @date 04-02-2025
 * @copyright GNU Public License
 */

 #include "links.h"

 struct _Links{
    char*       name; 
    Direction   direction; 
    Id          id; 
    Id          origin; 
    Id          destiny; 
    Bool      	status_orig;
	Bool		status_dest;
};


Links*       link_create()
{   
    Links*  newlink = (Links *)malloc(sizeof(Links));
    if(!newlink) return NULL;

    newlink->name        = NULL;
    newlink->destiny     = NO_ID;
    newlink->origin      = NO_ID;
    newlink->id          = NO_ID;
    newlink->status_dest = TRUE;
    newlink->status_orig = TRUE;

    return newlink;
}

Status       link_destroy(Links *link)
{
    if(!link) return ERROR;

    if(link->name)
    {
        free(link->name);
        link->name = NULL;
    }
    free(link);
    return OK;
}

/* ============== (set/get)  ====================== */

Status       link_set_id(Links *link, Id id_new)
{
    if(!link || id_new == NO_ID) return ERROR;

    link->id = id_new;
    return  OK;
}
Id           link_get_id(Links *link)
{
    if(!link) return NO_ID;

    return link->id;  
}


/* ============== (set/get)  ====================== */

Status       link_set_name(Links *link, char *name)
{

    if(!link || !name) return ERROR;

    /* Si existe name, borra */
    if(link->name)
    {
        free(link->name);
        link->name = NULL;
    }

    /*Reservamos espacio para el nuevo name */
    int str_len = strlen(name);
    link->name = (char *)calloc(str_len + 1, sizeof(char));
    if (!link->name) return ERROR;

    /* Copiamos name en link.name, el espacio sobrante será rellenado por '\0' */
    strncpy(link->name, name, str_len);

    return  OK;
}
char*        link_get_name(Links *link)
{
    if(!link || !link->name) return NULL;

    return strdup(link->name);
}


/* ============== (set/get) status (dest_to_origin/ origin_to_dest) ====================== */

Status       link_set_open_dest_to_origin(Links *link, Bool status)
{
    if(!link) return ERROR;
    link->status_orig = status;
    return OK;
}
Bool         link_get_open_dest_to_origin(Links *link)
{
     if(!link) return FALSE;
    return link->status_orig;   
}

Status       link_set_open_origin_to_dest(Links *link, Bool status)
{
    if(!link) return ERROR;
    link->status_dest = status;
    return OK;
}
Bool         link_get_open_origin_to_dest(Links *link)
{
    if(!link) return FALSE;
    return link->status_dest; 
}

/* ============== (set/get) direction ====================== */

Status       link_set_direction(Links *link, Direction dir)
{
    if(!link || dir== U) return ERROR;
    link->direction = dir;
    return OK;
}
Direction    link_get_direction(Links *link)
{
    if(!link) return U;
    return link->direction;
}

/* oppsited direction */
Direction    link_get_oppsite_direction(Links *link)
{
    if(!link) return U;
    //esto se puede optimizar tla vez usando congruencias, pero  a falta de una idea funcional lo dejare asi
    if(link->direction == N) return S;
    if(link->direction == S) return N;
    if(link->direction == E) return W;
    if(link->direction == W) return E;
    
    return U;
}

/* ============== (set/get) origin/destination ====================== */

Status       link_set_origin_id(Links *link, Id id_orig)
{
    if(!link || id_orig == NO_ID) return ERROR;
    link->origin = id_orig;
    return OK;
}
Id           link_get_origin_id(Links *link)
{
    if(!link) return NO_ID;
    return link->origin;
}

Status       link_set_destination_id(Links *link, Id id_dest)
{
    if(!link || id_dest == NO_ID) return ERROR;
    link->destiny = id_dest;
    return OK;
}
Id           link_get_destination_id(Links *link)
{
    if(!link) return NO_ID;
    return link->destiny;
}



/* ============== PRINT ====================== */

Status       link_print(Links *link, FILE* output)
{
    if(!output){
		printf( ROJO "ERROR: " RESET "There is nothing on the output \n");
		return	ERROR;
	}else if(!link){
		fprintf( output,ROJO "ERROR: " RESET "There is nothing on the pointer to Link\n");
		return	ERROR;
	}

	
	Id			id_link = link_get_id(link);
    Id          id_orig = link_get_origin_id(link);
    Id          id_dest = link_get_destination_id(link);
	Bool		open_orig = link_get_open_dest_to_origin(link);
	Bool		open_dest = link_get_open_origin_to_dest(link);
    Direction   dir = link_get_direction(link); 
    Direction   dir_opposite = link_get_oppsite_direction(link);
	char*		name= link_get_name(link);

	/*DEBUG*/
	short result = 0;

	if(dir == U || dir_opposite == U){
		free(name);
		fprintf(output, "There is a problem whit dir or dir_opp\n");
		result = 1;
	}
	if(id_link == NO_ID  ){
		free(name);
		fprintf(output, "There is a problem whit Id of the link\n");
		result = 1;
	}
	if(id_dest == NO_ID || id_orig == NO_ID){
		free(name);
		fprintf(output, "There is a problem whit ids of orig or dest\n");
		result = 1;
	}
	if(!name ){
		free(name);
		fprintf(output, "There is a problem whit name\n");
		result = 1;
	}
	if(result) return ERROR;
 	/*PRINT ALL GOOD*/


    fprintf(output,"=============== "ROJO" LINK "RESET"==================\n");
	fprintf(output,"==\t\t>>NAME LINK    : %10s\t\t=\n", name);
	fprintf(output,"==\t\t>>ID   LINK    : %4ld\t\t=\n", id_link);
	fprintf(output,"==\t\t>>ID ORIGIN    : %4ld\t\t=\n", id_orig);
    fprintf(output,"==\t\t>>ID DESTINI   : %4ld\t\t=\n", id_dest);
    fprintf(output,"==\t\t>>DIRECTION    : %4d\t\t=\n", dir);
    fprintf(output,"==\t\t>>OPEN DEST?   : %4d\t\t=\n", open_dest);
    fprintf(output,"==\t\t>>DIRECTION OP : %4d\t\t=\n", dir_opposite);
    fprintf(output,"==\t\t>>OPEN ORIG?   : %4d\t\t=\n", open_orig);
    fprintf(output,"===============================================\n");



	free(name);
	return OK;
}

 