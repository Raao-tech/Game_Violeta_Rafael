/**
 * @brief It implements the set structure
 *
 * @file set.c
 * @author Profesores PPROG
 * @version 0
 * @date 27-01-2025
 * @copyright GNU Public License
 */

#include "stdio.h"
#include "stdlib.h"
#include "set.h"


struct _Set{
    Id*  ids;
    int  n_ids;
};

/* create and destroy */
Set*    set_creat(){
    Set*    newset = NULL;

    if((newset= (Set*)malloc(sizeof(Set))) == NULL){
        return  NULL;
    }
    newset->n_ids = 0;
	newset->ids = NULL;

    return newset;
}

Status  set_destroy(Set *pset){

    int i;

    if(!pset ){
        return ERROR;
    }

    if((pset)->ids){
        for ( i = 0; i < (pset)->n_ids; i++){
           (pset)->ids[i] = NO_ID;
        }
        free((pset)->ids);
        (pset)->ids = NULL;
    }

    (pset)->n_ids = 0;

    free((pset));

    return OK;
}


Bool	set_is_empty(Set *pset){

	if(pset == NULL) return	FALSE;

	if(pset->n_ids <= 0) return TRUE;

	return FALSE;
}
Bool	set_contains_id(Set *pset, Id _id){
	int	i;
	if(!pset) return ERROR;

	for (i = 0; i < pset->n_ids; i++){
		if(pset->ids[i] == _id)	return TRUE;
	}

	return FALSE;
}

/* add and pop */
Status	set_add(Set* pset, Id	 new_id){

	/* If pset or new_id don't exist  */
	if(pset == NULL || new_id == NO_ID){
		return	ERROR;
	}

	//agregamos un espacio en el conjutno de objetos
	Id* pset_temp = (Id*)realloc((void *)pset->ids, (sizeof(Id) * (pset->n_ids + 1)) );
	
	if(pset_temp == NULL){
		return ERROR;
	}
	pset_temp[pset->n_ids] = new_id;
	pset->n_ids++; 

	pset->ids = pset_temp;

	return	OK;
}

Id	set_pop(Set* pset){
	Id id_pop;

	/* Manejo de errores */
	if(!pset || pset->n_ids == 0){
		return NO_ID;
	}
	if(set_is_empty(pset) == TRUE){
		return	NO_ID;
	}


	/* Guardamos el valor de id */
	id_pop = pset->ids[(pset->n_ids)-1]; // se asigna el valor de id (ultimo eleemntos)
	pset->ids[(pset->n_ids) - 1] = NO_ID;
	
	if(pset->n_ids == 1){   //Liberamos el array ids si queda un solo elemento
		free(pset->ids);
		pset->ids = NULL;
		pset->n_ids = 0;
		return id_pop;
	}

	/* Reducimos el array de ids de pset*/
	Id* pset_temp = (Id*)realloc((void *)pset->ids, (sizeof(Id) * (pset->n_ids -1)));
	if(pset_temp){
		pset->ids = pset_temp;
	}

	pset->n_ids--;
	return (id_pop);
}


/* print */
Status	set_print(FILE* output, Set*	pset){
	if(!pset){
		fprintf(output, ROJO "ERROR: " RESET "There is nothing on the Set of ids\n");
		return	ERROR;
	}

	fprintf(output, "**********The ids presents:**********\n");
	for (int i = 0; i < pset->n_ids; i++){
	fprintf(output, "\tn_Id (%d):\t" ROJO "%ld" RESET"\n", (i+1), pset->ids[i]);
	}
	fprintf(output, "**********END of the Ids*************\n");

	return OK;
}