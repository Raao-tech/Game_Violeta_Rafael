/**
 * @brief It reads information from files to create the game and saves it when required
 *
 * @file game_management.c
 * @author Violeta, Rafael, Salvador and Javier Jarque
 * @version 3
 * @date 20-04-2026
 * @copyright GNU Public License
 */

#include "game_management.h"
#include "links.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define F_PLAY  "#p:"
#define F_SPAC  "#s:"
#define F_OBJE  "#o:"
#define F_CHAR  "#c:"
#define F_LINK  "#l:"
#define F_NUMEN "#n:"

static Status game_load_spaces(Game *game, char *filename);
static Status game_load_objects(Game *game, char *filename);
static Status game_load_characters(Game *game, char *filename);
static Status game_load_players(Game *game, char *filename);
static Status game_load_numens(Game *game, char *filename);
static Status game_load_links(Game *game, char *filename);


static Status game_load_spaces(Game *game, char *filename)
{
  FILE *file = NULL;
  char line[WORD_SIZE] = "";
  char name[WORD_SIZE] = "";
  char gdesc[WORD_SIZE] = "";
  char OST[WORD_SIZE] = "";
  char *toks = NULL;
  Id id = NO_ID;
  Space *space = NULL;
  Status status = OK;
  int tam_format;

  if (!game || !filename) return ERROR;

  file = fopen(filename, "r");
  if (file == NULL) return ERROR;

  tam_format = strlen(F_SPAC);

  while (fgets(line, WORD_SIZE, file))
  {
    if (strncmp(F_SPAC, line, tam_format) == 0)
    {
      toks = strtok(line + tam_format, "|");
      if (!toks) continue;
      id = atol(toks);

      toks = strtok(NULL, "|");
      if (!toks) continue;
      strcpy(name, toks);

      toks = strtok(NULL, "|");
      if (!toks) strcpy(gdesc, "./img_src/background/default.jpg");
      else       strcpy(gdesc, toks);

      toks = strtok(NULL, "|\n");
      if (toks) strcpy(OST, toks);
      else      OST[0] = '\0';

#ifdef DEBUG
      printf("Leido space: s:%ld|%s\n", id, name);
#endif

      space = space_create();
      if (space != NULL)
      {
        space_set_id(space, id);
        space_set_name(space, name);
        space_set_gdesc(space, gdesc);
        if (OST[0] != '\0') space_set_ost(space, OST);
        game_add_space(game, space);
      }
    }
  }

  if (ferror(file)) status = ERROR;
  fclose(file);
  return status;
}

static Status game_load_objects(Game *game, char *filename)
{
  FILE *file = NULL;
  char line[WORD_SIZE] = "";
  char name[WORD_SIZE] = "";
  char description[WORD_SIZE] = "";
  char gdesc[WORD_SIZE] = "";
  char *toks = NULL;
  Id id = NO_ID, space_id = NO_ID, dependency = NO_ID;
  Object *obj = NULL;
  Space *space = NULL;
  Status status = OK;
  Bool consumable, movable;
  int tam_format, health, attack, energy, speed, pos_x, pos_y;

  if (!game || !filename) return ERROR;

  tam_format = strlen(F_OBJE);
  file = fopen(filename, "r");
  if (file == NULL) return ERROR;

  while (fgets(line, WORD_SIZE, file))
  {
    if (strncmp(F_OBJE, line, tam_format) == 0)
    {
      toks = strtok(line + tam_format, "|");
      if (!toks) continue;
      id = atol(toks);

      toks = strtok(NULL, "|");
      if (!toks) continue;
      strcpy(name, toks);

      toks = strtok(NULL, "|");
      if (!toks) continue;
      space_id = atol(toks);

      toks = strtok(NULL, "|");
      if (!toks) continue;
      pos_x = atoi(toks);

      toks = strtok(NULL, "|");
      if (!toks) continue;
      pos_y = atoi(toks);

      toks = strtok(NULL, "|");
      if (toks) strcpy(description, toks);
      else      description[0] = '\0';

      toks = strtok(NULL, "|");
      if (!toks) continue;
      strcpy(gdesc, toks);

      toks = strtok(NULL, "|");
      if (!toks) continue;
      consumable = atoi(toks) ? TRUE : FALSE;

      toks = strtok(NULL, "|");
      if (!toks) continue;
      health = atoi(toks);

      toks = strtok(NULL, "|");
      if (!toks) continue;
      attack = atoi(toks);

      toks = strtok(NULL, "|");
      if (!toks) continue;
      energy = atoi(toks);

      toks = strtok(NULL, "|");
      if (!toks) continue;
      speed = atoi(toks);

      toks = strtok(NULL, "|");
      if (toks) dependency = atol(toks);
      else      dependency = NO_ID;

      toks = strtok(NULL, "|\n");
      if (!toks) continue;
      movable = atoi(toks) ? TRUE : FALSE;

#ifdef DEBUG
      printf("Leido object: o:%ld|%s|%ld|%s\n", id, name, space_id, description);
#endif

      obj = obj_create();
      if (obj != NULL)
      {
        obj_set_id(obj, id);
        obj_set_name(obj, name);
        if (description[0] != '\0') obj_set_description(obj, description);

        game_add_object(game, obj);

        /* Si el space_id no existe,
         * el objeto se ignora silenciosamente. La carga de objetos
         * en inventarios desde un savefile se reimplementara
         * */
        space = game_get_space(game, space_id);
        if (space) space_set_object(space, id);

        obj_set_health(obj, health);
        obj_set_attack(obj, attack);
        obj_set_energy(obj, energy);
        obj_set_speed(obj, speed);
        obj_set_consumable(obj, consumable);
        obj_set_movable(obj, movable);
        obj_set_dependency(obj, dependency);

        /* TODO Bloque 3: usar pos_x, pos_y para situar el objeto en el grid */
        (void)pos_x;
        (void)pos_y;
      }
    }
  }
  if (ferror(file)) status = ERROR;
  fclose(file);
  return status;
}

static Status game_load_characters(Game *game, char *filename)
{
  FILE *file = NULL;
  char line[WORD_SIZE] = "";
  char name[WORD_SIZE] = "";
  char gdesc[WORD_SIZE] = "";
  char message[WORD_SIZE] = "";
  char *toks = NULL;
  Character *ch = NULL;
  Space *space = NULL;
  Id id = NO_ID, space_id = NO_ID;
  int health = 0, friendly = 1;
  Status status = OK;
  int tam_format;

  if (!game || !filename) return ERROR;

  tam_format = strlen(F_CHAR);
  file = fopen(filename, "r");
  if (file == NULL) return ERROR;

  while (fgets(line, WORD_SIZE, file))
  {
    if (strncmp(F_CHAR, line, tam_format) == 0)
    {
      toks = strtok(line + tam_format, "|");
      if (!toks) continue;
      id = atol(toks);

      toks = strtok(NULL, "|");
      if (!toks) continue;
      strcpy(name, toks);

      toks = strtok(NULL, "|");
      if (!toks) continue;
      strcpy(gdesc, toks);

      toks = strtok(NULL, "|");
      if (!toks) continue;
      space_id = atol(toks);

      toks = strtok(NULL, "|");
      if (!toks) continue;
      health = atoi(toks);

      toks = strtok(NULL, "|");
      if (!toks) continue;
      friendly = atoi(toks);

      toks = strtok(NULL, "\n");
      if (toks) strcpy(message, toks);
      else      message[0] = '\0';

#ifdef DEBUG
      printf("Leido character: c:%ld|%s|%s|%ld|%d|%d|%s\n",
             id, name, gdesc, space_id, health, friendly, message);
#endif

      ch = character_create();
      if (ch != NULL)
      {
        character_set_id(ch, id);
        character_set_name(ch, name);
        character_set_gdesc(ch, gdesc);
        character_set_health(ch, health);
        character_set_friendly(ch, friendly ? TRUE : FALSE);
        character_set_message(ch, message);
        game_add_character(game, ch);

        space = game_get_space(game, space_id);
        if (space) space_set_character(space, id);
      }
    }
  }

  if (ferror(file)) status = ERROR;
  fclose(file);
  return status;
}

static Status game_load_players(Game *game, char *filename)
{
  FILE *file = NULL;
  char line[WORD_SIZE] = "";
  char name[WORD_SIZE] = "";
  char gdesc[WORD_SIZE] = "";
  char *toks = NULL;
  Player *player = NULL;
  Space *space = NULL;
  Id id = NO_ID;
  Id zone = NO_ID;
  int max_numens = 0;
  int max_bag = 0;
  Status status = OK;
  int tam_format, pos_x = 0, pos_y = 0;

  if (!game || !filename) return ERROR;

  tam_format = strlen(F_PLAY);
  file = fopen(filename, "r");
  if (!file) return ERROR;

  while (fgets(line, WORD_SIZE, file))
  {
    if (strncmp(F_PLAY, line, tam_format) == 0)
    {
      toks = strtok(line + tam_format, "|");
      if (!toks) continue;
      id = atol(toks);

      toks = strtok(NULL, "|");
      if (!toks) continue;
      strcpy(name, toks);

      toks = strtok(NULL, "|");
      if (!toks) continue;
      zone = atol(toks);

      toks = strtok(NULL, "|");
      if (!toks) continue;
      pos_x = atoi(toks);

      toks = strtok(NULL, "|");
      if (!toks) continue;
      pos_y = atoi(toks);

      toks = strtok(NULL, "|");
      if (!toks) continue;
      strcpy(gdesc, toks);

      toks = strtok(NULL, "|");
      if (!toks) continue;
      max_bag = atoi(toks);

      toks = strtok(NULL, "|\n");
      if (!toks) continue;
      max_numens = atoi(toks);

#ifdef DEBUG
      printf("Leido player: p:%ld|%s|%s|%ld|%d|%d\n",
             id, name, gdesc, zone, max_numens, max_bag);
#endif

      player = player_create();
      if (player != NULL)
      {
        player_set_id(player, id);
        player_set_name(player, name);
        player_set_gdesc(player, gdesc);
        player_set_zone(player, zone);
        player_set_pos_x(player, pos_x);
        player_set_pos_y(player, pos_y);
        player_set_max_numens(player, max_numens);
        player_set_max_objects(player, max_bag);

        game_add_player(game, player);

        space = game_get_space(game, zone);
        if (space) space_set_discovered(space, TRUE);
      }
    }
  }

  if (ferror(file)) status = ERROR;
  fclose(file);
  return status;
}

static Status game_load_numens(Game *game, char *filename)
{
  FILE *file = NULL;
  char line[WORD_SIZE] = "";
  char name[WORD_SIZE] = "";
  char gdesc[WORD_SIZE] = "";
  char *toks = NULL;
  Numen *numen = NULL;
  Space *space = NULL;
  Id id = NO_ID, space_id = NO_ID;
  int health = 0, attack = 0, energy = 0, speed = 0;
  int skills[4];               
  int pos_x = 0, pos_y = 0;
  Id following = NO_ID;
  Status status = OK;
  int tam_format;
  int i;                         

  if (!game || !filename) return ERROR;

  for (i = 0; i < 4; i++) skills[i] = NO_ID;

  tam_format = strlen(F_NUMEN);
  file = fopen(filename, "r");
  if (file == NULL) return ERROR;

  while (fgets(line, WORD_SIZE, file))
  {
    if (strncmp(F_NUMEN, line, tam_format) == 0)
    {
      toks = strtok(line + tam_format, "|");
      if (!toks) continue;
      id = atol(toks);

      toks = strtok(NULL, "|");
      if (!toks) continue;
      strcpy(name, toks);

      toks = strtok(NULL, "|");
      if (!toks) continue;
      space_id = atol(toks);

      toks = strtok(NULL, "|");
      if (!toks) continue;
      pos_x = atoi(toks);

      toks = strtok(NULL, "|");
      if (!toks) continue;
      pos_y = atoi(toks);

      toks = strtok(NULL, "|");
      if (!toks) continue;
      strcpy(gdesc, toks);

      toks = strtok(NULL, "|");
      if (!toks) continue;
      health = atoi(toks);

      toks = strtok(NULL, "|");
      if (!toks) continue;
      energy = atoi(toks);

      toks = strtok(NULL, "|");
      if (!toks) continue;
      attack = atoi(toks);

      toks = strtok(NULL, "|");
      if (!toks) continue;
      speed = atoi(toks);

      /* FIX Bloque 0: 'i' declarada arriba, no en el for */
      for (i = 0; i < 4; i++)
      {
        toks = strtok(NULL, "|");
        if (toks) skills[i] = atol(toks);
        else      skills[i] = NO_ID;
      }

      toks = strtok(NULL, "|\n");
      if (toks) following = atol(toks);
      else      following = NO_ID;

      numen = numen_create();
      if (numen != NULL)
      {
        numen_set_id(numen, id);
        numen_set_name(numen, name);
        numen_set_gdesc(numen, gdesc);
        numen_set_health(numen, health);
        numen_set_energy(numen, energy);
        numen_set_attack(numen, attack);
        numen_set_speed(numen, speed);

        /* FIX Bloque 0: 'i' declarada arriba */
        for (i = 0; i < 4; i++) numen_set_skill(numen, skills[i]);

        numen_set_following(numen, following);

        if (following != NO_ID)
          player_add_numen(game_get_player_by_id(game, following), id);

        game_add_numens(game, numen);

        space = game_get_space(game, space_id);
        if (space) space_set_numen(space, id);
      }
    }
  }

  if (ferror(file)) status = ERROR;
  fclose(file);
  return status;
}

static Status game_load_links(Game *game, char *filename)
{
  FILE *file = NULL;
  char line[WORD_SIZE] = "";
  char name[WORD_SIZE] = "";
  char *toks = NULL;
  Links *link = NULL;
  Id id = NO_ID;
  Id origin = NO_ID, dest = NO_ID;
  int dir_num = 0;
  int open_otd = 1;
  int open_dto = 1;
  Status status = OK;
  int tam_format;

  if (!game || !filename) return ERROR;

  tam_format = strlen(F_LINK);
  file = fopen(filename, "r");
  if (!file) return ERROR;

  while (fgets(line, WORD_SIZE, file))
  {
    if (strncmp(F_LINK, line, tam_format) == 0)
    {
      toks = strtok(line + tam_format, "|");
      if (!toks) continue;
      id = atol(toks);

      toks = strtok(NULL, "|");
      if (!toks) continue;
      strcpy(name, toks);

      toks = strtok(NULL, "|");
      if (!toks) continue;
      origin = atol(toks);

      toks = strtok(NULL, "|");
      if (!toks) continue;
      dest = atol(toks);

      toks = strtok(NULL, "|");
      if (!toks) continue;
      dir_num = atoi(toks);

      toks = strtok(NULL, "|");
      if (!toks) continue;
      open_otd = atoi(toks);

      toks = strtok(NULL, "|\n");
      if (toks) open_dto = atoi(toks);
      else      open_dto = open_otd;

#ifdef DEBUG
      printf("Leido link: l:%ld|%s|%ld|%ld|%d|%d|%d\n",
             id, name, origin, dest, dir_num, open_otd, open_dto);
#endif

      link = link_create();
      if (link != NULL)
      {
        link_set_id(link, id);
        link_set_name(link, name);
        link_set_origin_id(link, origin);
        link_set_destination_id(link, dest);
        link_set_direction(link, (Direction)dir_num);
        link_set_open_origin_to_dest(link, open_otd ? TRUE : FALSE);
        link_set_open_dest_to_origin(link, open_dto ? TRUE : FALSE);
        game_add_links(game, link);
      }
    }
  }

  if (ferror(file)) status = ERROR;
  fclose(file);
  return status;
}

Status game_management_create_from_file(Game **game, char *filename)
{
  if (!game || !filename) return ERROR;

  *game = game_create();
  if (*game == NULL) return ERROR;

  if (game_load_spaces(*game, filename) == ERROR)     { game_destroy(*game); *game = NULL; return ERROR; }
  if (game_load_players(*game, filename) == ERROR)    { game_destroy(*game); *game = NULL; return ERROR; }
  if (game_load_objects(*game, filename) == ERROR)    { game_destroy(*game); *game = NULL; return ERROR; }
  if (game_load_characters(*game, filename) == ERROR) { game_destroy(*game); *game = NULL; return ERROR; }
  if (game_load_links(*game, filename) == ERROR)      { game_destroy(*game); *game = NULL; return ERROR; }
  if (game_load_numens(*game, filename) == ERROR)     { game_destroy(*game); *game = NULL; return ERROR; }

  return OK;
}

/* ========================================================================= */
/*                            SAVE FILE                                      */
/* ========================================================================= */
/*
 * STUB del Bloque 0. La implementacion actual de game_management_save_file
 * tiene varios bugs estructurales que no son del Bloque 0:
 *
 *   - Pasa 'game' (Game **) donde las APIs esperan Game *.
 *   - Hace free() de strings que pueden ser punteros internos del TAD
 *     (depende de la convencion de space_get_name, player_get_name, etc.).
 *   - 'dependency' (Id) se imprime como string con format %s.
 *   - Usa el campo 'OST' de Space que aun no esta confirmado en API.
 *
 * Todo eso se reescribira cuando consolidemos:
 *   - convencion de propiedad de strings en getters
 *   - API de save coherente
 *   - tests de integracion para verificar el roundtrip dat -> save -> load
 *
 * Por ahora devolvemos ERROR para que el comando 'save' figure como no
 * implementado en el log, sin riesgo de corromper memoria.
 */
Status game_management_save_file(Game **game)
{
  (void)game;
  return ERROR;
}