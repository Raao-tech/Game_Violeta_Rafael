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

/**
 * @brief Prefix tags used to identify each entity type in the .dat file.
 *
 * Every line in the .dat file begins with one of these prefixes,
 * followed by the fields of that entity separated by '|'.
 */
#define F_PLAY "#p:"
#define F_SPAC "#s:"
#define F_OBJE "#o:"
#define F_CHAR "#c:"
#define F_LINK "#l:"
#define F_NUMEN "#n:"

/**
 * Private function prototypes — one loader per entity type.
 * Each function opens the file, scans for its prefix, parses the
 * fields, builds the TAD, and registers it in the Game.
 */
static Status game_load_spaces(Game *game, char *filename);
static Status game_load_objects(Game *game, char *filename);
static Status game_load_characters(Game *game, char *filename);
static Status game_load_players(Game *game, char *filename);
static Status game_load_links(Game *game, char *filename);

static Status game_load_spaces(Game *game, char *filename)
{
  FILE *file = NULL;
  char line[WORD_SIZE] = "";
  char name[WORD_SIZE] = "";
  char *toks = NULL;
  Id id = NO_ID;
  Space *space = NULL;
  Status status = OK;
  int tam_format;
  int i;

  if (!game || !filename)
    return ERROR;

  file = fopen(filename, "r");
  if (file == NULL)
    return ERROR;

  tam_format = strlen(F_SPAC);

  while (fgets(line, WORD_SIZE, file))
  {
    if (strncmp(F_SPAC, line, tam_format) == 0)
    {

      /* --- id --- */
      toks = strtok(line + tam_format, "|");
      if (!toks)
        continue;
      id = atol(toks);

      /* --- name --- */
      toks = strtok(NULL, "|");
      if (!toks)
        continue;
      strcpy(name, toks);

#ifdef DEBUG
      printf("Leido space: s:%ld|%s\n", id, name);
#endif

      space = space_create();
      if (space != NULL)
      {
        space_set_id(space, id);
        space_set_name(space, name);

        /* Try to read up to MAX_LINE gdesc lines (optional) */
        for (i = 0; i < MAX_LINE; i++)
        {
          toks = strtok(NULL, "|");
          if (toks && toks[0] != '\n' && toks[0] != '\0')
          {
            space_set_gdesc_line(space, i, toks);
          }
        }

        game_add_space(game, space);
      }
    }
  }

  if (ferror(file))
    status = ERROR;

  fclose(file);
  return status;
}

static Status game_load_objects(Game *game, char *filename)
{
  FILE *file = NULL;
  char line[WORD_SIZE] = "";
  char name[WORD_SIZE] = "";
  char description[WORD_SIZE] = "";
  char *toks = NULL;
  Id id = NO_ID, space_id = NO_ID;
  Object *obj = NULL;
  Space *space = NULL;
  Status status = OK;
  int tam_format;

  if (!game || !filename)
    return ERROR;

  tam_format = strlen(F_OBJE);
  file = fopen(filename, "r");
  if (file == NULL)
    return ERROR;

  while (fgets(line, WORD_SIZE, file))
  {
    if (strncmp(F_OBJE, line, tam_format) == 0)
    {

      /* --- id --- */
      toks = strtok(line + tam_format, "|");
      if (!toks)
        continue;
      id = atol(toks);

      /* --- name --- */
      toks = strtok(NULL, "|");
      if (!toks)
        continue;
      strcpy(name, toks);

      /* --- space_id --- */
      toks = strtok(NULL, "|");
      if (!toks)
        continue;
      space_id = atol(toks);

      /* --- description (rest of the line, optional) --- */
      toks = strtok(NULL, "\n");
      if (toks)
      {
        strcpy(description, toks);
      }
      else
      {
        description[0] = '\0';
      }

#ifdef DEBUG
      printf("Leido object: o:%ld|%s|%ld|%s\n", id, name, space_id, description);
#endif

      obj = obj_create();
      if (obj != NULL)
      {
        obj_set_id(obj, id);
        obj_set_name(obj, name);
        if (description[0] != '\0')
        {
          obj_set_description(obj, description);
        }
        game_add_object(game, obj);

        /* Place object in its space */
        space = game_get_space(game, space_id);
        if (space)
          space_set_object(space, id);
      }
    }
  }

  if (ferror(file))
    status = ERROR;

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

  if (!game || !filename)
    return ERROR;

  tam_format = strlen(F_CHAR);
  file = fopen(filename, "r");
  if (file == NULL)
    return ERROR;

  while (fgets(line, WORD_SIZE, file))
  {
    if (strncmp(F_CHAR, line, tam_format) == 0)
    {

      /* --- id --- */
      toks = strtok(line + tam_format, "|");
      if (!toks)
        continue;
      id = atol(toks);

      /* --- name --- */
      toks = strtok(NULL, "|");
      if (!toks)
        continue;
      strcpy(name, toks);

      /* --- gdesc --- */
      toks = strtok(NULL, "|");
      if (!toks)
        continue;
      strcpy(gdesc, toks);

      /* --- space_id (position) --- I3 ORDER: before health */
      toks = strtok(NULL, "|");
      if (!toks)
        continue;
      space_id = atol(toks);

      /* --- health --- */
      toks = strtok(NULL, "|");
      if (!toks)
        continue;
      health = atoi(toks);

      /* --- friendly (0 or 1) --- */
      toks = strtok(NULL, "|");
      if (!toks)
        continue;
      friendly = atoi(toks);

      /* --- message (rest of line, may contain spaces) --- */
      toks = strtok(NULL, "\n");
      if (toks)
      {
        strcpy(message, toks);
      }
      else
      {
        message[0] = '\0';
      }

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

        /* Place character in its space */
        space = game_get_space(game, space_id);
        if (space)
          space_set_character(space, id);
      }
    }
  }

  if (ferror(file))
    status = ERROR;

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
  Id location = NO_ID;
  int health = 0;
  int max_bag = 0;
  Status status = OK;
  int tam_format;

  if (!game || !filename)
    return ERROR;

  tam_format = strlen(F_PLAY);
  file = fopen(filename, "r");
  if (!file)
    return ERROR;

  while (fgets(line, WORD_SIZE, file))
  {
    if (strncmp(F_PLAY, line, tam_format) == 0)
    {

      /* --- id --- */
      toks = strtok(line + tam_format, "|");
      if (!toks)
        continue;
      id = atol(toks);

      /* --- name --- */
      toks = strtok(NULL, "|");
      if (!toks)
        continue;
      strcpy(name, toks);

      /* --- gdesc --- */
      toks = strtok(NULL, "|");
      if (!toks)
        continue;
      strcpy(gdesc, toks);

      /* --- location --- */
      toks = strtok(NULL, "|");
      if (!toks)
        continue;
      location = atol(toks);

      /* --- health --- */
      toks = strtok(NULL, "|");
      if (!toks)
        continue;
      health = atoi(toks);

      /* --- max_bag --- */
      toks = strtok(NULL, "|\n");
      if (!toks)
        continue;
      max_bag = atoi(toks);

#ifdef DEBUG
      printf("Leido player: p:%ld|%s|%s|%ld|%d|%d\n",
             id, name, gdesc, location, health, max_bag);
#endif

      player = player_create();
      if (player != NULL)
      {
        player_set_id(player, id);
        player_set_name(player, name);
        player_set_gdesc(player, gdesc);
        player_set_location(player, location);
        player_set_health(player, health);

        player_set_max_objects(player, max_bag);

        game_add_player(game, player);

        space = game_get_space(game, location);
        if (space)
          space_set_discovered(space, TRUE);
      }
    }
  }

  if (ferror(file))
    status = ERROR;

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
  int open_otd = 1; /* open origin-to-dest */
  int open_dto = 1; /* open dest-to-origin */
  Status status = OK;
  int tam_format;

  if (!game || !filename)
    return ERROR;

  tam_format = strlen(F_LINK);
  file = fopen(filename, "r");
  if (!file)
    return ERROR;

  while (fgets(line, WORD_SIZE, file))
  {
    if (strncmp(F_LINK, line, tam_format) == 0)
    {

      /* --- id --- */
      toks = strtok(line + tam_format, "|");
      if (!toks)
        continue;
      id = atol(toks);

      /* --- name --- */
      toks = strtok(NULL, "|");
      if (!toks)
        continue;
      strcpy(name, toks);

      /* --- origin id --- */
      toks = strtok(NULL, "|");
      if (!toks)
        continue;
      origin = atol(toks);

      /* --- destination id --- */
      toks = strtok(NULL, "|");
      if (!toks)
        continue;
      dest = atol(toks);

      /* --- direction (numeric: N=0 S=1 E=2 W=3) --- */
      toks = strtok(NULL, "|");
      if (!toks)
        continue;
      dir_num = atoi(toks);

      /* --- open_orig_to_dest --- */
      toks = strtok(NULL, "|");
      if (!toks)
        continue;
      open_otd = atoi(toks);

      /* --- open_dest_to_orig (bidirectional extension) --- */
      toks = strtok(NULL, "|\n");
      if (toks)
      {
        open_dto = atoi(toks);
      }
      else
      {
        /* if only 6 fields are present (standard enunciado format),
        use the same value for both directions, making the link symmetric.*/
        open_dto = open_otd;
      }

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

        /*
         * Cast the integer to Direction.  The enum values are:
         *   N=0, S=1, E=2, W=3, U=4
         * If dir_num is out of range, link_set_direction will
         * reject U and return ERROR, but we continue anyway
         * (the link simply won't work in-game).
         */
        link_set_direction(link, (Direction)dir_num);

        link_set_open_origin_to_dest(link, open_otd ? TRUE : FALSE);
        link_set_open_dest_to_origin(link, open_dto ? TRUE : FALSE);

        game_add_links(game, link);
      }
    }
  }

  if (ferror(file))
    status = ERROR;

  fclose(file);
  return status;
}

Status game_create_from_file(Game **game, char *filename)
{
  if (!game || !filename)
    return ERROR;

  *game = game_create();
  if (*game == NULL)
    return ERROR;

  if (game_load_spaces(*game, filename) == ERROR)
  {
    game_destroy(*game);
    *game = NULL;
    return ERROR;
  }

  if (game_load_objects(*game, filename) == ERROR)
  {
    game_destroy(*game);
    *game = NULL;
    return ERROR;
  }

  if (game_load_characters(*game, filename) == ERROR)
  {
    game_destroy(*game);
    *game = NULL;
    return ERROR;
  }

  if (game_load_links(*game, filename) == ERROR)
  {
    game_destroy(*game);
    *game = NULL;
    return ERROR;
  }

  if (game_load_players(*game, filename) == ERROR)
  {
    game_destroy(*game);
    *game = NULL;
    return ERROR;
  }

  return OK;
}

Status game_save_file(Game **game)
{
  Space *space = NULL;
  Object *object = NULL;
  Player *player = NULL;
  Numen *numen = NULL;
  Links *link = NULL;
  Id id = NO_ID, location = NO_ID, origin = NO_ID, dest = NO_ID, skills[MAX_HELD_SKILLS], dependency, following;
  int check = 0, bucle, pos_x = 0, pos_y = 0, health, attack, energy, speed;
  Bool friendly, open_otd, open_dto, consumable;
  char *name = NULL, *gdesc = NULL, *OST = NULL, *message = NULL;
  FILE *new_sfile = NULL;
  if (!game)
    return ERROR;
  new_sfile = fopen("../savefile/savefile.dat", "w");
  if (!new_sfile)
    return ERROR;
  check = game_get_n_spaces(*game);
  for (bucle = 0; bucle < check; bucle++)
  {
    id = game_get_space_id_at(game, bucle);
    space = game_get_space(*game, id);
    name = space_get_name(space);
    gdesc = space_get_gdesc(space); /*Hay que cambialo por una ruta string*/
    OST = space_get_ost(space);     /*Hay que implementarlo*/

    fprintf(new_sfile, "#s:%d|%s|%s|%s|\n", (int)id, name,
            gdesc == NULL ? "" : gdesc, OST == NULL ? "" : OST);
    free(name);
    free(gdesc);
    free(OST);
  }

  check = game_get_n_objects(*game);
  for (bucle = 0; bucle < check; bucle++)
  {
    object = game_get_object_at(game, bucle);
    id = obj_get_id(object);
    name = obj_get_name(object);
    location = obj_get_location(object);
    pos_x = obj_get_pos_x(object);
    pos_y = obj_get_pos_y(object);
    message = obj_get_description(object);
    gdesc = obj_get_gdesc(object); /*Hay que cambialo por una ruta string*/
    health = obj_get_health(object);
    attack = obj_get_attack(object);
    energy = obj_get_energy(object);
    speed = obj_get_speed(object);
    consumable = obj_get_consumable(object);

    fprintf(new_sfile, "#o:%d|%s|%d|%d|%d|%s|%s|%d|%d|%d|%d|%d|%d|\n", (int)id, name, (int)location, pos_x, pos_y, message, gdesc == NULL ? "" : gdesc, consumable == TRUE ? 1 : 0, health, energy, attack, speed, dependency == NO_ID ? "" : (int)dependency);
    free(name);
    free(gdesc);
    free(message);
  }

  check = game_get_n_numens(*game);
  for (bucle = 0; bucle < check; bucle++)
  {
    numen = game_get_numen_at(game, bucle);
    id = numen_get_id(numen);
    name = numen_get_name(numen);
    location = numen_get_location(numen);
    pos_x = numen_get_pos_x(numen);
    pos_y = numen_get_pos_y(numen);
    gdesc = numen_get_gdesc(numen); /*Hay que cambialo por una ruta string*/
    health = numen_get_health(numen);
    attack = numen_get_attack(numen);
    energy = numen_get_energy(numen);
    speed = numen_get_speed(numen);
    skills[0] = numen_get_skill(numen, 0);
    skills[1] = numen_get_skill(numen, 1);
    skills[2] = numen_get_skill(numen, 2);
    skills[3] = numen_get_skill(numen, 3);
    following = numen_get_following(numen);

    fprintf(new_sfile, "#n:%d|%s|%d|%d|%d|%s|%d|%d|%d|%d|%d|%d|%d|%d|%d|\n", (int)id, name, (int)location, pos_x, pos_y, gdesc == NULL ? "" : gdesc, health, energy, attack, speed, skills[0], skills[1], skills[2], skills[3], following == NO_ID ? "" : (int)following);
    free(name);
    free(gdesc);
  }

  check = game_get_n_players(*game);
  for (bucle = 0; bucle < check; bucle++)
  {
    player = game_get_player_at(game, bucle);
    id = player_get_id(player);
    name = player_get_name(player);
    location = player_get_location(player);
    pos_x = player_get_pos_x(player);
    pos_y = player_get_pos_y(player);
    gdesc = player_get_gdesc(player); /*Hay que cambialo por una ruta string*/

    fprintf(new_sfile, "#n:%d|%s|%d|%d|%d|%s|\n", (int)id, name, (int)location, pos_x, pos_y, gdesc == NULL ? "" : gdesc, health, energy, attack, speed, skills[0], skills[1], skills[2], skills[3], following == NO_ID ? "" : (int)following);
    free(name);
    free(gdesc);
    /*Para guardar el inventario de player, el id location que se guarda en el object tiene que ser el id del player, de otra forma, hay que hacer un load inventory*/
  }
  fclose(new_sfile);
  return OK;
}