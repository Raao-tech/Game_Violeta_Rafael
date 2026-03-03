#include "stdio.h"
#include "stdlib.h"
#include "set.h"

struct _Set{
    Id*  ids;   /* lista de espacios para los ids a guardar */
    int  n_ids; /* Cantidad de Ids presentes en la lsita*/
};


Set*    set_creat(){
    Set*    newset = NULL;
    int i;

    if((newset= (Set*)malloc(sizeof(Set)*MIN_IDS)) == NULL){
        return  NULL;
    }
    newset->n_ids = 0;
    
    if((newset->ids = (Id*)malloc(sizeof(Id))) == NULL){
        return  NULL;
    }
    
	for ( i = 0; i < MIN_IDS; i++){
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

Bool	set_is_full(Set *pset){

	if(pset == NULL){
		return TRUE;
	}

	//Esta vacio?
	if(set_is_empty(pset) == TRUE){
			return FALSE;
	}

	//Hay algo en ultimo espacio del array?
	if(pset->ids[pset->n_ids - 1] != NO_ID){
			return	TRUE;
	}

	return TRUE;
}

Status  set_expands(Set** pset){

    Id		*realloc_ids = NULL;
	size_t	tam;
    int 	old_n_ids;

    if(pset == NULL || (*pset) == NULL || ((*pset)->ids) == NULL || ((*pset)->n_ids < MIN_IDS)){
        return ERROR;
    }


    old_n_ids = (*pset)->n_ids;
	tam = (size_t)SCALES * (size_t)old_n_ids;
    if((realloc_ids = (Id *)realloc((void *)(*pset)->ids, (tam * sizeof(Id)))) == NULL){
        return ERROR;
    }


    //reasginamo la direccion dek  uevo array realocado en el puntero de ids del pset
    (*pset)->ids = realloc_ids;

    //limpiamos e giualamos a NO_ID lps nuevosd esapcios de la pila.
    for (int i = old_n_ids; i < tam; i++)
	   (*pset)->ids[i] = NO_ID;


    return OK;
}

Status	set_add(Set** pset, Id	 new_id){

    int old_n_ids;


	/* If pset or new_id don't exist  */
	if(pset == NULL || (*pset) == NULL || new_id == NO_ID){
		return	ERROR;
	}

	//Esta full? ---> Realoca y guarda
	if(set_is_full(*pset) == TRUE){
        old_n_ids = (*pset)->ids;
		Id* pset_realloc = (Id*)realloc((void *)old_n_ids,2);
	
		//aumentamos el maximo
		(*pset)->n_ids = old_n_ids * 2;
		if(pset_realloc == NULL){
			return ERROR;
		}
        //Liberamos el puntero anterior
		free((*pset)->ids);

        //reasignamos el nuevo puntero
		(*pset)->ids = pset_realloc;

        //Guardamos el nuevo objeto
		(*pset)->ids[old_n_ids] = new_id;

        //se igualan a NO_ID todos los demas espacios
        for ( int i = old_n_ids + 1; i < (*pset)->n_ids; i++)
        {
            
        }
        


	}


	//si no esta full----> aumenta tamaño y guarda info





}