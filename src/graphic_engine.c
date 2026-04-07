/**
 * @brief It implements a textual graphic engine
 *
 * @file graphic_engine.c
 * @author Profesores PPROG
 * @version 2
 * @date 24-01-2026
 * @copyright GNU Public License
 */

#include "graphic_engine.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "command.h"
#include "libscreen.h"
#include "space.h"
#include "types.h"

#define WIDTH_MAP 70
#define WIDTH_DES 48
#define WIDTH_BAN 50
#define HEIGHT_MAP 25
#define HEIGHT_BAN 1
#define HEIGHT_HLP 2
#define HEIGHT_FDB 3

struct _Graphic_engine {
  Area *map, *descript, *banner, *help, *feedback;
};


/**
 * @brief It evaluates the status result of the last command executed
 * @author Violeta y Rafa
 *
 *
 * @param result OK, ERROR_attack, ERROR_chat, ERROR_dir, ERROR_take, ERROR_drop,  ERROR
 * @return  A pointer to string with feedback
 */
static char *ge_evaluated_error(Status result);






Graphic_engine *graphic_engine_create(){
  static Graphic_engine *ge = NULL;

  if (ge) return ge;

  screen_init(HEIGHT_MAP + HEIGHT_BAN + HEIGHT_HLP + HEIGHT_FDB + 4, WIDTH_MAP + WIDTH_DES + 3);
  ge = (Graphic_engine *)calloc(1, sizeof(Graphic_engine));
  if (ge == NULL) return NULL;

  ge->map = screen_area_init(1, 1, WIDTH_MAP, HEIGHT_MAP);
  ge->descript = screen_area_init(WIDTH_MAP + 2, 1, WIDTH_DES, HEIGHT_MAP);
  ge->banner = screen_area_init((int)((WIDTH_MAP + WIDTH_DES + 1 - WIDTH_BAN) / 2), HEIGHT_MAP + 2, WIDTH_BAN, HEIGHT_BAN);
  ge->help = screen_area_init(1, HEIGHT_MAP + HEIGHT_BAN + 2, WIDTH_MAP + WIDTH_DES + 1, HEIGHT_HLP);
  ge->feedback = screen_area_init(1, HEIGHT_MAP + HEIGHT_BAN + HEIGHT_HLP + 3, WIDTH_MAP + WIDTH_DES + 1, HEIGHT_FDB);

  return ge;
}

void graphic_engine_destroy(Graphic_engine *ge){
  if (!ge) return;

  screen_area_destroy(ge->map);
  screen_area_destroy(ge->descript);
  screen_area_destroy(ge->banner);
  screen_area_destroy(ge->help);
  screen_area_destroy(ge->feedback);

  screen_destroy();
  free(ge);
}

/* Helper: build a string with object names in a space, comma-separated */
static void ge_build_obj_str(Game *game, Space *space, char *buf, int bufsize){
  int i;
  Object *obj;

  buf[0] = '\0';
  if (!space) return;

  /* Iterate game objects and check which are in this space */
  {
    int total = game_get_n_objects(game);
    int first = 1;
    for (i = 0; i < total; i++){
      obj = game_get_object_at(game, i);
      if (obj && space_contains_object(space, obj_get_id(obj)) == TRUE){
        if (!first) strncat(buf, ",", bufsize - strlen(buf) - 1);
        strncat(buf, obj_get_name(obj), bufsize - strlen(buf) - 1);
        first = 0;
      }
    }
  }
}

/* Helper: build character gdesc string for a space */
static void ge_build_char_str(Game *game, Space *space, char *buf, int bufsize){
  int i, total;
  Character *ch;
  char *gdesc;

  buf[0] = '\0';
  if (!space) return;

  total = game_get_n_characters(game);
  for (i = 0; i < total; i++){
    ch = game_get_character_at(game, i);
    if (ch && space_contains_character(space, character_get_id(ch)) == TRUE){
      gdesc = character_get_gdesc(ch);
      if (gdesc){
        if (buf[0] != '\0') strncat(buf, " ", bufsize - strlen(buf) - 1);
        strncat(buf, gdesc, bufsize - strlen(buf) - 1);
        free(gdesc);
      }
    }
  }
}

void graphic_engine_paint_game(Graphic_engine *ge, Game *game){
  Id id_act = NO_ID, id_back = NO_ID, id_next = NO_ID, id_left = NO_ID, id_right = NO_ID;
  Space *space_act = NULL;
  Player *player = NULL;
  char str[255];
  char obj_str[100];
  char char_str[50];
  CommandCode last_cmd = UNKNOWN;
  Status  type_error;
  extern char *cmd_to_str[N_CMD][N_CMDT];
  int i;

  player = game_get_player(game);

  /* ===== MAP AREA ===== */
  screen_area_clear(ge->map);
  id_act = player_get_location(player);

  if (id_act != NO_ID){
    space_act = game_get_space(game, id_act);
    id_back = space_get_north(space_act);
    id_next = space_get_south(space_act);
    id_left = space_get_east(space_act);
    id_right = space_get_west(space_act);

    /* NORTH space */
    if (id_back != NO_ID){
      Space *sp_back = game_get_space(game, id_back);
      ge_build_obj_str(game, sp_back, obj_str, sizeof(obj_str));
      sprintf(str, "              +---------------+");
      screen_area_puts(ge->map, str);
      sprintf(str, "              |           %-4ld|", id_back);
      screen_area_puts(ge->map, str);
      for (i = 0; i < MAX_LINE; i++){
        char *gdl = space_get_gdesc(sp_back, i);
        sprintf(str, "              |%-15s|", gdl ? gdl : "");
        screen_area_puts(ge->map, str);
      }
      sprintf(str, "              |%-15s|", obj_str);
      screen_area_puts(ge->map, str);
      sprintf(str, "              +---------------+");
      screen_area_puts(ge->map, str);
      sprintf(str, "                    ^");
      screen_area_puts(ge->map, str);
    }

    /* CURRENT space (with left/right indicators) */
    {
      char *pgdesc = player_get_gdesc(player);
      ge_build_obj_str(game, space_act, obj_str, sizeof(obj_str));
      ge_build_char_str(game, space_act, char_str, sizeof(char_str));

      if (id_left != NO_ID) sprintf(str, "         <");
      else sprintf(str, "          ");

      screen_area_puts(ge->map, str);
      sprintf(str, "              +---------------+");
      screen_area_puts(ge->map, str);
      sprintf(str, "              | %-6s %-3s %-3ld|", char_str[0] ? char_str : "", pgdesc ? pgdesc : ">8D", id_act);
      screen_area_puts(ge->map, str);

      for (i = 0; i < MAX_LINE; i++){
        char *gdl = space_get_gdesc(space_act, i);
        sprintf(str, "              |%-15s|", gdl ? gdl : "");
        screen_area_puts(ge->map, str);
      }

      sprintf(str, "              |%-15s|", obj_str);
      screen_area_puts(ge->map, str);
      sprintf(str, "              +---------------+");
      screen_area_puts(ge->map, str);

      if (id_right != NO_ID){
        sprintf(str, "                            >");
        screen_area_puts(ge->map, str);
      }

      if (pgdesc) free(pgdesc);
    }

    /* SOUTH space */
    if (id_next != NO_ID){
      Space *sp_next = game_get_space(game, id_next);
      ge_build_obj_str(game, sp_next, obj_str, sizeof(obj_str));
      sprintf(str, "                    v");
      screen_area_puts(ge->map, str);
      sprintf(str, "              +---------------+");
      screen_area_puts(ge->map, str);
      sprintf(str, "              |           %-4ld|", id_next);
      screen_area_puts(ge->map, str);
      sprintf(str, "              |%-15s|", obj_str);
      screen_area_puts(ge->map, str);
      sprintf(str, "              +---------------+");
      screen_area_puts(ge->map, str);
    }
  }

  /* ===== DESCRIPTION AREA ===== */
  screen_area_clear(ge->descript);

  /* Objects list with locations */
  sprintf(str, " Objects:");
  screen_area_puts(ge->descript, str);
  for (i = 0; i < game_get_n_objects(game); i++){
    Object *obj = game_get_object_at(game, i);
    Id loc = game_get_object_location(game, obj_get_id(obj));
    if (loc != NO_ID)
      sprintf(str, "  %s : %ld", obj_get_name(obj), loc);
    else
      sprintf(str, "  %s : player", obj_get_name(obj));
    screen_area_puts(ge->descript, str);
  }

  screen_area_puts(ge->descript, " ");

  /* Characters list with locations and health */
  sprintf(str, " Characters:");
  screen_area_puts(ge->descript, str);
  for (i = 0; i < game_get_n_characters(game); i++){
    Character *ch = game_get_character_at(game, i);
    char *chname = character_get_name(ch);
    char *chgdesc = character_get_gdesc(ch);
    Id loc = game_get_character_location(game, character_get_id(ch));
    sprintf(str, "  %s %s : %ld (%d)", chname ? chname : "?", chgdesc ? chgdesc : "?", loc, character_get_health(ch));
    screen_area_puts(ge->descript, str);
    if (chname) free(chname);
    if (chgdesc) free(chgdesc);
  }

  screen_area_puts(ge->descript, " ");

  /* Player info */
  sprintf(str, " Player : %ld (%d)", player_get_location(player), player_get_health(player));
  screen_area_puts(ge->descript, str);

  if (player_get_n_objects(player) > 0){
    sprintf(str, " Player objects: %d", player_get_n_objects(player));
    screen_area_puts(ge->descript, str);
  } else {
    sprintf(str, " Player has no objects");
    screen_area_puts(ge->descript, str);
  }

  /* Chat message: if last command was CHAT and OK, show character message */
  last_cmd = command_get_code(game_get_last_command(game));
  if (last_cmd == CHAT && game_get_last_cmd_status(game) == OK){
    char *char_name = command_get_obj(game_get_last_command(game));
    if (char_name){
      Character *ch = game_get_character_by_name(game, char_name);
      if (ch){
        char *msg = character_get_message(ch);
        if (msg){
          screen_area_puts(ge->descript, " ");
          sprintf(str, " \"%s\"", msg);
          screen_area_puts(ge->descript, str);
          free(msg);
        }
      }
    }
  }

  /* ===== BANNER ===== */
  screen_area_puts(ge->banner, "              The haunted castle game ");

  /* ===== HELP ===== */
  screen_area_clear(ge->help);
  sprintf(str, " The commands you can use are:");
  screen_area_puts(ge->help, str);
  sprintf(str, "  next(n) back(b) left(l) right(r) take(t) drop(d) attack(a) chat(c) exit(e)");
  screen_area_puts(ge->help, str);

  /* ===== FEEDBACK (F15j: last commands with OK/ERROR) ===== */
  screen_area_clear(ge->feedback);
  last_cmd = command_get_code(game_get_last_command(game));
  type_error = game_get_last_cmd_status(game);
  if (last_cmd >= 0 && last_cmd < N_CMD + 1){
    int idx = last_cmd - NO_CMD;
    if (idx >= 0 && idx < N_CMD){
      sprintf(str, " %s (%s): %s",
        cmd_to_str[idx][CMDL],
        cmd_to_str[idx][CMDS],
        ge_evaluated_error(type_error));
      screen_area_puts(ge->feedback, str);
    }
  }

  /* Dump to terminal */
  screen_paint();
  printf("prompt:> ");
}





/* =========PRIVATED FUNCTION================= */

static char *ge_evaluated_error(Status result){

  if(result == OK)            return "OK";
  if(result == ERROR_Attack)  return "ERROR: (Attack/a)  (name monster)";
  if(result == ERROR_Chat)    return "ERROR: (chat/c)  (name character friend)";
  if(result == ERROR_dir)     return "ERROR: It's not possible go to there. Can't you see there's a wall?. A little of please";
  if(result == ERROR_take)    return "ERROR: (take/t) (name of  object)";
  if(result == ERROR_drop)    return "ERROR: (drop/d) (name of  object)";

  return  "ERROR";
}