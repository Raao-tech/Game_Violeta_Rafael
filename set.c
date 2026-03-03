#include "stdio.h"
#include "stdlib.h"
#include "set.h"

struct _Set{
    Id*  ids;
    int  n_ids;
};


Set*    set_creat(int   n_max){
    Set*    newset = NULL;
    int i;

    if(n_max <0) {
        return NULL;
    }

    if((newset= (Set*)malloc(sizeof(Set))) == NULL){
        return  NULL;
    }
    newset->n_ids = n_max;
    
    if((newset->ids = (Id*)malloc(sizeof(Id)*(newset->n_ids))) == NULL){
        return  NULL;
    }

    for(i = 0; i < newset->n_ids; i++){
        newset->ids[i] = NO_ID;
    }

    return newset;
}

Status  set_destroy(Set **ppset){

    int i;

    if(!ppset || !(*ppset)){
        return ERROR;
    }

    if((*ppset)->ids){
        for ( i = 0; i < (*ppset)->n_ids; i++){
           (*ppset)->ids[i] = NO_ID;
        }
        free((*ppset)->ids);
        (*ppset)->ids = NULL;
    }

    (*ppset)->n_ids = 0;

    free((*ppset));
    *ppset = NULL;

    return OK;
}

Bool	set_is_empty(Set *pset){

    int i;

	if(pset == NULL){
		return	FALSE;
	}

	if(pset->n_ids <= 0){
		return TRUE;
	}

	return FALSE;
}

/* Bool	set_is_full(Set *pset){

    int i;

	if(pset == NULL){
		return TRUE;
	}

    for (i = 0; i < ; i++){
        
    }
    


	return TRUE;
} */
Status	set_add(Set* pset, Id	 new_id){

	/* If pset or new_id don't exist  */
	if(pset == NULL || new_id == NO_ID){
		return	ERROR;
	}

	if(set_is_full(pset) == TRUE){
		Id* pset_realloc = (Id*)realloc((void *)pset,2);
		
		if(pset_realloc == NULL){
			return ERROR;
		}

	}

}