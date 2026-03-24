/**
 * @brief It reads information from files to create the game
 *
 * @file game_reader.c
 * @author Violeta, Rafael and Salvador
 * @version 3.0
 * @date 23-4-2026
 * @copyright GNU Public License
 */

#include "game_reader.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ========== Private: Load spaces from file ========== */
/* Format: #s:id|name|north|east|south|west|gdesc0|gdesc1|gdesc2|gdesc3|gdesc4| */
Status game_load_spaces(Game *game, char *filename){
  FILE *file = NULL;
  char line[WORD_SIZE] = "";
  char name[WORD_SIZE] = "";
  char *toks = NULL;
  Id id = NO_ID, north = NO_ID, east = NO_ID, south = NO_ID, west = NO_ID;
  Space *space = NULL;
  Status status = OK;
  int i;

  if (!game || !filename) return ERROR;

  file = fopen(filename, "r");
  if (file == NULL) return ERROR;

  while (fgets(line, WORD_SIZE, file)){
    if (strncmp("#s:", line, 3) == 0){
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
      printf("Leido space: s:%ld|%s|%ld|%ld|%ld|%ld\n", id, name, north, east, south, west);
#endif

      space = space_create();
      if (space != NULL){
        space_set_id(space, id);
        space_set_name(space, name);
        space_set_north(space, north);
        space_set_east(space, east);
        space_set_south(space, south);
        space_set_west(space, west);

        /* Try to read gdesc lines (optional, may not be present) */
        for (i = 0; i < MAX_LINE; i++){
          toks = strtok(NULL, "|");
          if (toks && toks[0] != '\n' && toks[0] != '\0'){
            space_set_gdesc_line(space, i, toks);
          }
        }

        game_add_space(game, space);
      }
    }
  }

  if (ferror(file)) status = ERROR;

  fclose(file);
  return status;
}

/* ========== Private: Load objects from file ========== */
/* Format: #o:id|name|space_id */
Status game_load_objects(Game *game, char *filename){
  FILE *file = NULL;
  char line[WORD_SIZE] = "";
  char name[WORD_SIZE] = "";
  char *toks = NULL;
  Id id = NO_ID, space_id = NO_ID;
  Object *obj = NULL;
  Space *space = NULL;
  Status status = OK;

  if (!game || !filename) return ERROR;

  file = fopen(filename, "r");
  if (file == NULL) return ERROR;

  while (fgets(line, WORD_SIZE, file)){
    if (strncmp("#o:", line, 3) == 0){
      toks = strtok(line + 3, "|");
      id = atol(toks);
      toks = strtok(NULL, "|");
      strcpy(name, toks);
      toks = strtok(NULL, "|\n");
      space_id = atol(toks);

#ifdef DEBUG
      printf("Leido object: o:%ld|%s|%ld\n", id, name, space_id);
#endif

      obj = obj_create();
      if (obj != NULL){
        obj_set_id(obj, id);
        obj_set_name(obj, name);
        game_add_object(game, obj);

        /* Place object in its space */
        space = game_get_space(game, space_id);
        if (space) space_set_object(space, id);
      }
    }
  }

  if (ferror(file)) status = ERROR;

  fclose(file);
  return status;
}

/* ========== Private: Load characters from file ========== */
/* Format: #c:id|name|gdesc|health|friendly(0/1)|message|space_id */
Status game_load_characters(Game *game, char *filename){
  FILE *file = NULL;
  char line[WORD_SIZE] = "";
  char name[WORD_SIZE] = "";
  char gdesc[WORD_SIZE] = "";
  char message[WORD_SIZE] = "";
  char *toks = NULL;
  Id id = NO_ID, space_id = NO_ID;
  int health = 0, friendly = 1;
  Character *ch = NULL;
  Space *space = NULL;
  Status status = OK;

  if (!game || !filename) return ERROR;

  file = fopen(filename, "r");
  if (file == NULL) return ERROR;

  while (fgets(line, WORD_SIZE, file)){
    if (strncmp("#c:", line, 3) == 0){
      toks = strtok(line + 3, "|");
      id = atol(toks);
      toks = strtok(NULL, "|");
      strcpy(name, toks);
      toks = strtok(NULL, "|");
      strcpy(gdesc, toks);
      toks = strtok(NULL, "|");
      health = atoi(toks);
      toks = strtok(NULL, "|");
      friendly = atoi(toks);
      toks = strtok(NULL, "|");
      strcpy(message, toks);
      toks = strtok(NULL, "|\n");
      space_id = atol(toks);

#ifdef DEBUG
      printf("Leido character: c:%ld|%s|%s|%d|%d|%s|%ld\n", id, name, gdesc, health, friendly, message, space_id);
#endif

      ch = character_create();
      if (ch != NULL){
        character_set_id(ch, id);
        character_set_name(ch, name);
        character_set_gdesc(ch, gdesc);
        character_set_health(ch, health);
        character_set_friendly(ch, friendly ? TRUE : FALSE);
        character_set_message(ch, message);
        game_add_character(game, ch);

        /* Place character in its space */
        space = game_get_space(game, space_id);
        if (space) space_set_character(space, id);
      }
    }
  }

  if (ferror(file)) status = ERROR;

  fclose(file);
  return status;
}

/* ========== Private: Load player ========== */
Status game_load_player(Game *game, char *filename)
{
    FILE *file = NULL;
    Player *p = NULL;
    Id id, player_space;
    char name[WORD_SIZE];
    char *toks;
    char *gdesc = NULL;
    char line[WORD_SIZE] = "";
    int health = 0;

    /* Error control */
    if (!filename)
    {
        return ERROR;
    }
    if (!(file = fopen(filename, "r")))
    {
        return ERROR;
    }

    /* Reading and loading player */
    while (fgets(line, WORD_SIZE, file))
    {
        if (strncmp(line, "#p:", 3) == 0)
        {
            toks = strtok(line + 3, "|");
            if (!toks)
            {
                fclose(file);
                return ERROR;
            }
            id = atol(toks);
            toks = strtok(NULL, "|");
            if (!toks)
            {
                fclose(file);
                return ERROR;
            }
            strncpy(name, toks, WORD_SIZE - 1);
            name[WORD_SIZE - 1] = '\0';
            toks = strtok(NULL, "|");
            if (!toks)
            {
                fclose(file);
                return ERROR;
            }
            player_space = atol(toks);
            toks = strtok(NULL, "|");
            if (!toks)
            {
                fclose(file);
                return ERROR;
            }

            if (strlen(toks) < 1)
            {
                fclose(file);
                return ERROR;
            }
            gdesc = (char *)calloc(strlen(toks) + 1, sizeof(char));
            if (!gdesc)
            {
                fclose(file);
                return ERROR;
            }
            strcpy(gdesc, toks);

            toks = strtok(NULL, "|");
            if (!toks)
            {
                free(gdesc);
                fclose(file);
                return ERROR;
            }
            health = atoi(toks);

            if (health < 1)
            {
                free(gdesc);
                fclose(file);
                return ERROR;
            }

            p = player_create();
            if (!p)
            {
                free(gdesc);
                fclose(file);
                return ERROR;
            }

            if (player_set_id(p, id)==ERROR||player_set_name(p, name) == ERROR || player_set_gdesc(p, gdesc) == ERROR || player_set_health(p, health) == ERROR||player_set_location(game, player_space) == ERROR)
            {
                player_destroy(p);
                free(gdesc);
                fclose(file);
                return ERROR;
            }  
            if (game_add_player(game, p) == ERROR)
            {
                player_destroy(p);
                fclose(file);
                return ERROR;
            }
         
        }
    }
    if (ferror(file))
    {
        fclose(file);
        return ERROR;
    }
    fclose(file);
    return OK;
}

/* ========== Public: Create game from file ========== */
Status game_create_from_file(Game **game, char *filename){
  if (!game || !filename) return ERROR;

  *game = game_create();
  if (*game == NULL) return ERROR;

  if (game_load_spaces(*game, filename) == ERROR){
    game_destroy(*game);
    *game = NULL;
    return ERROR;
  }

  if (game_load_objects(*game, filename) == ERROR){
    game_destroy(*game);
    *game = NULL;
    return ERROR;
  }

  if (game_load_characters(*game, filename) == ERROR){
    game_destroy(*game);
    *game = NULL;
    return ERROR;
  }

  if (game_load_player(*game, filename) == ERROR){
    game_destroy(*game);
    *game = NULL;
    return ERROR;
  }

  return OK;
}
