/**
 * @brief It implements the space module
 *
 * @file space.c
 * @author Profesores PPROG
 * @version 0
 * @date 24-01-2026
 * @copyright GNU Public License
 */

#include "space.h"

/**
 * @brief Space
 *
 * This struct stores all the information of a space.
 */
struct _Space{
  Id    id;                             /*!< Id number of the space, it must be unique */
  char  name[WORD_SIZE + 1];            /*!< Name of the space */
  char  gdesc[MAX_LINE][MAX_CHAR + 1];  /* Descripcion, cada linea con MAX_CHAR caracteres + 1 '\0' */
  Id    north;                          /*!< Id of the space at the north */
  Id    south;                          /*!< Id of the space at the south */
  Id    east;                           /*!< Id of the space at the east */
  Id    west;                           /*!< Id of the space at the west */
  Set   *objs_id;                       /*  Set of objets*/
  Set   *characters_id;                 /* !<Character */
};

/* create or destroy */
Space *space_create(){
  Space *newSpace = NULL;
  int i;

  /* Error control */
  newSpace = (Space *)calloc(1, sizeof(Space));
  if (!newSpace) return NULL;

  /* Initialization of an empty space*/
  newSpace->id = NO_ID;
  newSpace->name[0] = '\0';
  newSpace->north = NO_ID;
  newSpace->south = NO_ID;
  newSpace->east = NO_ID;
  newSpace->west = NO_ID;

  /*init gdesc to NULL*/
  for (i = 0; i < MAX_LINE; i++){
    newSpace->gdesc[i][0] = '\0';
  }
  
  /*init objet  */
  newSpace->objs_id = set_creat();
  if( !newSpace->objs_id) {
    free(newSpace);
    return NULL; 
  }

  /*init characteres*/
  newSpace->characters_id = set_creat();
  if( !newSpace->characters_id) {
    free(newSpace->objs_id);
    free(newSpace);
    return NULL; 
  }


  return newSpace;
}
Status space_destroy(Space *space){
  if (!space) return ERROR;
  
  if(set_destroy(space->characters_id) == ERROR) return ERROR;
  if(set_destroy(space->objs_id) == ERROR) return ERROR;
  
  free(space);
  return OK;
}


/* id */
Status space_set_id(Space *space, Id new_id){
  if(!space) return ERROR;

  space->id = new_id;
  return OK;
}
Id space_get_id(Space *space){
  if (!space) return NO_ID;
  
  return space->id;
}

/* name */
Status space_set_name(Space *space, char *name){
  if (!space || !name) return ERROR;

  if (!strcpy(space->name, name)) return ERROR;
  
  return OK;
}
const char *space_get_name(Space *space){
  if (!space) return NULL;
  
  return space->name;
}


/* objs */
Status space_set_object(Space *space, Id new_id){
  if (!space) return ERROR;

  return set_add(space->objs_id, new_id);
}
Bool space_contains_object(Space *space, Id id_obj){
  if (!space) return FALSE;
 
  return  set_contains_id(space->objs_id, id_obj);
}
Status  space_remove_object(Space *space, Id id_obj){
  if(!space) return ERROR;
  return set_delete_id(space->objs_id, id_obj);
}
int    space_get_n_objects(Space *space){
  if(!space) return ERROR_MAIN;
  return  set_get_n_ids(space->objs_id);
}


/* characters */
Status  space_set_character(Space *space, Id new_id){
  if(!space) return ERROR;
  return set_add(space->characters_id, new_id);
}
Bool    space_contains_character(Space *space, Id id_chara){
  if(!space) return FALSE;
  return  set_contains_id(space->characters_id, id_chara);
}
Status  space_remove_character(Space *space, Id id_chara){
  if(!space) return ERROR;
  return  set_delete_id(space->characters_id, id_chara);
}
int    space_get_n_characters(Space *space){
  if(!space) return ERROR_MAIN;
  return  set_get_n_ids(space->characters_id);
}


/* gDescripcion */
Status  space_set_gdesc_line(Space *space, int line, char* desc){
  int tam_desc;
  if(!space || !desc) return ERROR;
  if(line < 0 || line >= MAX_LINE)  return ERROR;
  if((tam_desc = strlen(desc)) >= MAX_CHAR) return  ERROR;
  
  strncpy(space->gdesc[line], desc, MAX_CHAR);
  space->gdesc[line][MAX_CHAR] = '\0'; //el array esta compuesto por MAX_CHAR elementos, no más
  return  OK;

}
Status  space_print_gdesc(FILE *output, Space *space){
  if(!output || !space) return ERROR;
  for (int i = 0; i < MAX_LINE; i++){
    if(fprintf(output,"%s\n",space->gdesc[i]) < 0) return ERROR;
  }

  return OK;
}
char*   space_get_gdesc(Space *space, int line){
  if(!space) return NULL;
  if(line < 0 || line >= MAX_LINE)  return NULL;

  return  space->gdesc[line];
}


/* neitghboors */
  /* north */
Status space_set_north(Space *space, Id id){
  if (!space || id == NO_ID) return ERROR;
  space->north = id;
  return OK;
}
Id space_get_north(Space *space){
  if (!space) return NO_ID;
  
  return space->north;
}
  /* South */
Status space_set_south(Space *space, Id id){
  if (!space || id == NO_ID) return ERROR;
  space->south = id;
  return OK;
}
Id space_get_south(Space *space){
  if (!space) return NO_ID;
  return space->south;
}

  /* East */
Status space_set_east(Space *space, Id id){
  if (!space || id == NO_ID) return ERROR; 
  space->east = id;
  return OK;
}
Id space_get_east(Space *space){
  if (!space) return NO_ID;
  return space->east;
}

  /* West */
Status space_set_west(Space *space, Id id){
  if (!space || id == NO_ID) return ERROR;
  space->west = id;
  return OK;
}
Id space_get_west(Space *space){
  if (!space) return NO_ID;
  return space->west;
}


/* Print */
Status space_print(Space *space){
  Id idaux = NO_ID;

  /* Error Control */
  if (!space) return ERROR;

  /* 1. Print the id and the name of the space */
  fprintf(stdout, "--> Space (Id: %ld; Name: %s)\n", space->id, space->name);



  /* 2. For each direction, print its link */
  idaux = space_get_north(space);
  if (idaux != NO_ID) 
    fprintf(stdout, "---> North link: %ld.\n", idaux);
  else
    fprintf(stdout, "---> No north link.\n");
  
  idaux = space_get_south(space);
  if (idaux != NO_ID)
    fprintf(stdout, "---> South link: %ld.\n", idaux);
  else
    fprintf(stdout, "---> No south link.\n");

  idaux = space_get_east(space);
  if (idaux != NO_ID)
    fprintf(stdout, "---> East link: %ld.\n", idaux);
  else
    fprintf(stdout, "---> No east link.\n");
  
  idaux = space_get_west(space);
  if (idaux != NO_ID)
    fprintf(stdout, "---> West link: %ld.\n", idaux);
  else
    fprintf(stdout, "---> No west link.\n");
  


  /* 3. Print if there is one or more objects in the space or not */
  if (space_get_n_objects(space) > 0){
    fprintf(stdout, "---> Objects in the space:\n");
    set_print(stdout, space->objs_id);
  }else{
    fprintf(stdout, "---> No object in the space.\n");
  }

  /* 4. Print if there is one or more charst in the space or not */
  if (space_get_n_characters(space) > 0){
    fprintf(stdout, "---> Chararacters in the space:\n");
    set_print(stdout, space->characters_id);
  }else{
    fprintf(stdout, "---> There are not characters in the space.\n");
  }

  /* 4. Print if gdescription*/
  if(space_print_gdesc(stdout, space) == ERROR) return ERROR;

  return OK;
}
