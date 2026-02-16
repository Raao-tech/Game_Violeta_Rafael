#include "game_reader.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ID_PLAYER 1
#define ID_OBJECT 11
#define NAME_OBJC "TNT"

Status game_load_spaces(Game *game, char *filename) {
  FILE *file = NULL;
  char line[WORD_SIZE] = "";
  char name[WORD_SIZE] = "";
  char *toks = NULL;
  Id id = NO_ID, north = NO_ID, east = NO_ID, south = NO_ID, west = NO_ID;
  Space *space = NULL;
  Status status = OK;

  if (!filename) {
    return ERROR;
  }

  file = fopen(filename, "r");
  if (file == NULL) {
    return ERROR;
  }

  while (fgets(line, WORD_SIZE, file)) {
    if (strncmp("#s:", line, 3) == 0) {
      toks = strtok(line + 3, "|");
      id = atol(toks);
      toks = strtok(NULL, "|");
      strcpy(name, toks);
      toks = strtok(NULL, "|");
      north = atol(toks);
      toks = strtok(NULL, "|");
      east = atol(toks);
      toks = strtok(NULL, "|");
      south = atol(toks);
      toks = strtok(NULL, "|");
      west = atol(toks);
#ifdef DEBUG
      printf("Leido: s:%ld|%s|%ld|%ld|%ld|%ld\n", id, name, north, east, south, west);
#endif
      space = space_create(id);
      if (space != NULL) {
        space_set_name(space, name);
        space_set_north(space, north);
        space_set_east(space, east);
        space_set_south(space, south);
        space_set_west(space, west);
        game_add_space(game, space);
      }
    }
  }

  if (ferror(file)) {
    status = ERROR;
  }

  fclose(file);

  return status;
}


Status game_load_player(Game *game, Id  id_player){
  if(game == NULL || id_player == NO_ID){
    return ERROR;
  }

  if(game_create_player(game,id_player) == ERROR){
    return ERROR;
  }

  /*set location of the player*/
  return game_set_player_location(game, game_get_space_id_at(game, 0));
}

Status game_load_object(Game *game, Id  id_object, char *name){
  if(game == NULL || id_object == NO_ID || name == NULL){
    return ERROR;
  }

  if(game_create_object(game,id_object, name) == ERROR){
    return ERROR;
  }

  /*set location of the object*/
  return game_set_object_location(game, game_get_space_id_at(game, 0)); 
}
Status game_create_from_file(Game *game, char *filename) {
  if (game_create(game) == ERROR) {
    return ERROR;
  }

  if(game_load_spaces(game, filename) == ERROR){
    return ERROR;
  }

  if (game_load_player(game, ID_PLAYER) == ERROR) {
    return ERROR;
  }


  if (game_load_object(game, ID_OBJECT, NAME_OBJC) == ERROR) {
    return ERROR;
  }


  return OK;
}





