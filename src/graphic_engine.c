/**
 * @brief It implements a textual graphic engine
 *
 * @file graphic_engine.c
 * @author Profesores PPROG, Javier Jarque and Rafael
 * @version 6
 * @date 11-04-2026
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

#define WIDTH_MAP  70
#define WIDTH_DES  48
#define WIDTH_BAN  50
#define HEIGHT_MAP 25
#define HEIGHT_BAN 1
#define HEIGHT_HLP 2
#define HEIGHT_FDB 3

#define N_PLAYER_COLORS 6
#define MAX_HP_BAR      5

/* Side box internal width ("+-------+" = 9 total, 7 internal) */
#define SIDE_TOTAL 9
#define SIDE_INT   7

struct _Graphic_engine {
  Area *map, *descript, *banner, *help, *feedback;
};

static const Frame_color player_colors[N_PLAYER_COLORS] = {
  BLUE, GREEN, RED, YELLOW, PURPLE, CYAN
};

/* ========== PRIVATE PROTOTYPES ========== */
static const char *ge_status_str(Status result);
static void ge_build_char_str(Game *game, Space *space, char *buf, int sz);
static void ge_build_obj_str(Game *game, Space *space, char *buf, int sz);
static void ge_build_player_str(Game *game, Id space_id, char *buf, int sz);
static void ge_hp_bar(int hp, char *buf, int sz);
static const char *ge_hp_warn(int hp);

/* ========================================================================= */
/*                          CREATE / DESTROY                                 */
/* ========================================================================= */

Graphic_engine *graphic_engine_create() {
  static Graphic_engine *ge = NULL;
  int r = HEIGHT_MAP + HEIGHT_BAN + HEIGHT_HLP + HEIGHT_FDB + 4;
  int c = WIDTH_MAP + WIDTH_DES + 3;

  if (ge) return ge;
  screen_init(r, c);

  ge = (Graphic_engine *)calloc(1, sizeof(Graphic_engine));
  if (!ge) return NULL;

  ge->map      = screen_area_init(1, 1, WIDTH_MAP, HEIGHT_MAP);
  ge->descript = screen_area_init(WIDTH_MAP + 2, 1, WIDTH_DES, HEIGHT_MAP);
  ge->banner   = screen_area_init(
                   (WIDTH_MAP + WIDTH_DES + 1 - WIDTH_BAN) / 2,
                   HEIGHT_MAP + 2, WIDTH_BAN, HEIGHT_BAN);
  ge->help     = screen_area_init(1, HEIGHT_MAP + HEIGHT_BAN + 2,
                   WIDTH_MAP + WIDTH_DES + 1, HEIGHT_HLP);
  ge->feedback = screen_area_init(1, HEIGHT_MAP + HEIGHT_BAN + HEIGHT_HLP + 3,
                   WIDTH_MAP + WIDTH_DES + 1, HEIGHT_FDB);
  return ge;
}

void graphic_engine_destroy(Graphic_engine *ge) {
  if (!ge) return;
  screen_area_destroy(ge->map);
  screen_area_destroy(ge->descript);
  screen_area_destroy(ge->banner);
  screen_area_destroy(ge->help);
  screen_area_destroy(ge->feedback);
  screen_destroy();
  free(ge);
}

/* ========================================================================= */
/*                           PAINT GAME                                      */
/* ========================================================================= */

void graphic_engine_paint_game(Graphic_engine *ge, Game *game) {
  Id           id_act, id_back, id_next, id_left, id_right;
  Space       *space_act = NULL;
  Player      *player    = NULL;
  Frame_color  color;
  CommandCode  last_cmd;
  Status       type_error;
  char         str[255];
  char         obj_str[100];
  char         char_str[50];
  char         plyr_str[50];
  char         hpb[30];
  int          i, turn;
  char        *pname = NULL;

  /* Side box info */
  Bool    has_w = FALSE, has_e = FALSE;
  Bool    w_open = FALSE, e_open = FALSE;
  Bool    w_disc = FALSE, e_disc = FALSE;
  Space  *sp_w = NULL, *sp_e = NULL;
  char    w_obj[20], e_obj[20];

  /* Pre-built side box lines (4 lines each, 10 chars + null) */
  char wL[4][12];
  char eL[4][12];
  char wG[4][5];   /* gap between west and center */
  char eG[4][5];   /* gap between center and east */

  extern char *cmd_to_str[N_CMD][N_CMDT];

  if (!ge || !game) return;

  player = game_get_player_by_turn(game);
  if (!player) return;

  turn     = game_get_turn(game);
  color    = player_colors[turn % N_PLAYER_COLORS];
  last_cmd = command_get_code(game_get_last_command(game));

  /* ========================== MAP ========================== */
  screen_area_clear(ge->map);
  id_act = player_get_location(player);

  if (id_act != NO_ID) {
    space_act = game_get_space(game, id_act);
    id_back  = game_get_connection(game, id_act, N);
    id_next  = game_get_connection(game, id_act, S);
    id_left  = game_get_connection(game, id_act, W);
    id_right = game_get_connection(game, id_act, E);

    /* ---- Prepare WEST side box ---- */
    has_w = (id_left != NO_ID);
    if (has_w) {
      sp_w = game_get_space(game, id_left);
      w_open = game_connection_is_open(game, id_act, W);
      w_disc = space_get_discovered(sp_w);
      w_obj[0] = '\0';
      if (w_disc) ge_build_obj_str(game, sp_w, w_obj, sizeof(w_obj));

      sprintf(wL[0], "+-------+");
      sprintf(wL[1], "|   %3ld |", id_left);
      if (w_disc)
        sprintf(wL[2], "|%-7.7s|", w_obj[0] ? w_obj : "       ");
      else
        sprintf(wL[2], "| ????? |");
      sprintf(wL[3], "+-------+");

      sprintf(wG[0], "   ");
      sprintf(wG[1], w_open ? " < " : " x ");
      sprintf(wG[2], "   ");
      sprintf(wG[3], "   ");
    } else {
      for (i = 0; i < 4; i++) {
        sprintf(wL[i], "         ");
        sprintf(wG[i], "   ");
      }
    }

    /* ---- Prepare EAST side box ---- */
    has_e = (id_right != NO_ID);
    if (has_e) {
      sp_e = game_get_space(game, id_right);
      e_open = game_connection_is_open(game, id_act, E);
      e_disc = space_get_discovered(sp_e);
      e_obj[0] = '\0';
      if (e_disc) ge_build_obj_str(game, sp_e, e_obj, sizeof(e_obj));

      sprintf(eL[0], "+-------+");
      sprintf(eL[1], "|   %3ld |", id_right);
      if (e_disc)
        sprintf(eL[2], "|%-7.7s|", e_obj[0] ? e_obj : "       ");
      else
        sprintf(eL[2], "| ????? |");
      sprintf(eL[3], "+-------+");

      sprintf(eG[0], "   ");
      sprintf(eG[1], e_open ? " > " : " x ");
      sprintf(eG[2], "   ");
      sprintf(eG[3], "   ");
    } else {
      for (i = 0; i < 4; i++) {
        sprintf(eL[i], "         ");
        sprintf(eG[i], "   ");
      }
    }

    /* ---- NORTH box ---- */
    if (id_back != NO_ID) {
      Space *sp   = game_get_space(game, id_back);
      Bool   disc = space_get_discovered(sp);
      Bool   open = game_connection_is_open(game, id_act, N);

      sprintf(str, "             +---------------+");
      screen_area_puts(ge->map, str);
      sprintf(str, "             |           %c%3ld|",
              open ? ' ' : '*', id_back);
      screen_area_puts(ge->map, str);

      if (disc) {
        ge_build_obj_str(game, sp, obj_str, sizeof(obj_str));
        sprintf(str, "             |%-15s|", obj_str);
        screen_area_puts(ge->map, str);
      } else {
        sprintf(str, "             |   ? ? ? ? ?   |");
        screen_area_puts(ge->map, str);
      }

      sprintf(str, "             +---------------+");
      screen_area_puts(ge->map, str);
      sprintf(str, "                   %c", open ? '^' : 'x');
      screen_area_puts(ge->map, str);
    }

    /* ---- CURRENT space with SIDE BOXES ---- */
    {
      ge_build_obj_str(game, space_act, obj_str, sizeof(obj_str));
      ge_build_char_str(game, space_act, char_str, sizeof(char_str));
      ge_build_player_str(game, id_act, plyr_str, sizeof(plyr_str));

      /* Combine NPCs + players for header */
      char combined[101] = "";
      if (char_str[0] && plyr_str[0])
        snprintf(combined, sizeof(combined), "%s %s", char_str, plyr_str);
      else if (char_str[0])
        snprintf(combined, sizeof(combined), "%s", char_str);
      else if (plyr_str[0])
        snprintf(combined, sizeof(combined), "%s", plyr_str);

      /* Line 0: top borders */
      sprintf(str, " %s%s+---------------+%s%s",
              wL[0], wG[0], eG[0], eL[0]);
      screen_area_puts(ge->map, str);

      /* Line 1: west ID / center header / east ID */
      {
        char ctr[20];
        sprintf(ctr, "| %-10.10s %3ld|", combined, id_act);
        sprintf(str, " %s%s%s%s%s", wL[1], wG[1], ctr, eG[1], eL[1]);
        screen_area_puts(ge->map, str);
      }

      /* Lines 2-6: west content+bottom / center gdesc / east content+bottom */
      for (i = 0; i < MAX_LINE; i++) {
        char *gdl = space_get_gdesc(space_act, i);
        char ctr[20];
        int  si = i + 2;  /* side box line index */
        const char *wl = (si < 4) ? wL[si] : "         ";
        const char *wg = (si < 4) ? wG[si] : "   ";
        const char *eg = (si < 4) ? eG[si] : "   ";
        const char *el = (si < 4) ? eL[si] : "         ";

        sprintf(ctr, "|%-15s|", gdl ? gdl : "");
        sprintf(str, " %s%s%s%s%s", wl, wg, ctr, eg, el);
        screen_area_puts(ge->map, str);
      }

      /* Line 7: objects in current space */
      sprintf(str, " %s%s|%-15s|%s%s",
              "         ", "   ", obj_str, "   ", "         ");
      screen_area_puts(ge->map, str);

      /* Line 8: bottom border */
      sprintf(str, "             +---------------+");
      screen_area_puts(ge->map, str);
    }

    /* ---- SOUTH box ---- */
    if (id_next != NO_ID) {
      Space *sp   = game_get_space(game, id_next);
      Bool   disc = space_get_discovered(sp);
      Bool   open = game_connection_is_open(game, id_act, S);

      sprintf(str, "                   %c", open ? 'v' : 'x');
      screen_area_puts(ge->map, str);
      sprintf(str, "             +---------------+");
      screen_area_puts(ge->map, str);
      sprintf(str, "             |           %c%3ld|",
              open ? ' ' : '*', id_next);
      screen_area_puts(ge->map, str);

      if (disc) {
        ge_build_obj_str(game, sp, obj_str, sizeof(obj_str));
        sprintf(str, "             |%-15s|", obj_str);
        screen_area_puts(ge->map, str);
      } else {
        sprintf(str, "             |   ? ? ? ? ?   |");
        screen_area_puts(ge->map, str);
      }

      sprintf(str, "             +---------------+");
      screen_area_puts(ge->map, str);
    }
  }

  /* ======================= DESCRIPTION ======================= */
  screen_area_clear(ge->descript);

  /* Objects */
  sprintf(str, " Objects:");
  screen_area_puts(ge->descript, str);
  for (i = 0; i < game_get_n_objects(game); i++) {
    Object *obj = game_get_object_at(game, i);
    Id loc = game_get_object_location(game, obj_get_id(obj));
    if (loc != NO_ID)
      sprintf(str, "  %s : %ld", obj_get_name(obj), loc);
    else
      sprintf(str, "  %s : player", obj_get_name(obj));
    screen_area_puts(ge->descript, str);
  }
  screen_area_puts(ge->descript, " ");

  /* Characters */
  sprintf(str, " Characters:");
  screen_area_puts(ge->descript, str);
  for (i = 0; i < game_get_n_characters(game); i++) {
    Character *ch  = game_get_character_at(game, i);
    char      *cn  = character_get_name(ch);
    char      *cg  = character_get_gdesc(ch);
    Id         loc = game_get_character_location(game, character_get_id(ch));
    int        hp  = character_get_health(ch);
    Bool       fri = character_get_friendly(ch);

    ge_hp_bar(hp, hpb, sizeof(hpb));
    sprintf(str, "  %s %-10s %s",
            fri ? "[ALLY] " : "[ENEMY]", cn ? cn : "?", cg ? cg : "?");
    screen_area_puts(ge->descript, str);
    sprintf(str, "    HP: %s %-6s loc:%ld", hpb, ge_hp_warn(hp), loc);
    screen_area_puts(ge->descript, str);

    if (cn) free(cn);
    if (cg) free(cg);
  }
  screen_area_puts(ge->descript, " ");

  /* Players */
  sprintf(str, " Players:");
  screen_area_puts(ge->descript, str);
  for (i = 0; i < game_get_n_players(game); i++) {
    Player *p  = game_get_player_at(game, i);
    int     hp = player_get_health(p);

    pname = player_get_name(p);
    ge_hp_bar(hp, hpb, sizeof(hpb));

    if (i == turn)
      sprintf(str, "  >> %s  <-- TURN", pname ? pname : "?");
    else
      sprintf(str, "     %s", pname ? pname : "?");
    screen_area_puts(ge->descript, str);
    sprintf(str, "     HP: %s %-6s loc:%ld",
            hpb, ge_hp_warn(hp), player_get_location(p));
    screen_area_puts(ge->descript, str);

    if (pname) free(pname);
  }
  screen_area_puts(ge->descript, " ");

  /* Inventory */
  if (player_get_n_objects(player) > 0)
    sprintf(str, " Inventory: %d objects", player_get_n_objects(player));
  else
    sprintf(str, " Inventory: empty");
  screen_area_puts(ge->descript, str);

  /* CHAT */
  if (last_cmd == CHAT && game_get_last_cmd_status(game) == OK) {
    char *cn = command_get_obj(game_get_last_command(game));
    if (cn) {
      Character *ch = game_get_character_by_name(game, cn);
      if (ch) {
        char *msg = character_get_message(ch);
        if (msg) {
          screen_area_puts(ge->descript, " ");
          sprintf(str, " \"%s\"", msg);
          screen_area_puts(ge->descript, str);
          free(msg);
        }
      }
    }
  }

  /* INSPECT */
  if (last_cmd == INSPECT && game_get_last_cmd_status(game) == OK) {
    char *iname = command_get_obj(game_get_last_command(game));
    if (iname) {
      Object *obj = game_get_object_by_name(game, iname);
      if (obj) {
        char *dsc = obj_get_description(obj);
        if (dsc) {
          screen_area_puts(ge->descript, " ");
          sprintf(str, " [%s]: %s", iname, dsc);
          screen_area_puts(ge->descript, str);
        }
      }
    }
  }

  /* ======================== BANNER ======================== */
  pname = player_get_name(player);
  sprintf(str, "  Player %d: %s", turn + 1, pname ? pname : "???");
  screen_area_puts(ge->banner, str);
  if (pname) free(pname);

  /* ========================= HELP ========================= */
  screen_area_clear(ge->help);
  sprintf(str, " The commands you can use are:");
  screen_area_puts(ge->help, str);
  sprintf(str, " take or t, drop or d, attack or a,"
               " chat or c, move or m, inspect or i, exit or e");
  screen_area_puts(ge->help, str);

  /* ======================= FEEDBACK ======================= */
  screen_area_clear(ge->feedback);
  type_error = game_get_last_cmd_status(game);
  if (last_cmd >= 0 && last_cmd < N_CMD) {
    int idx = last_cmd - NO_CMD;
    if (idx >= 0 && idx < N_CMD) {
      char       *oa = command_get_obj(game_get_last_command(game));
      const char *rs = ge_status_str(type_error);
      if (oa)
        sprintf(str, " %s (%s) %s: %s",
                cmd_to_str[idx][CMDL], cmd_to_str[idx][CMDS], oa, rs);
      else
        sprintf(str, " %s (%s): %s",
                cmd_to_str[idx][CMDL], cmd_to_str[idx][CMDS], rs);
      screen_area_puts(ge->feedback, str);
    }
  }

  /* ======================== RENDER ======================== */
  screen_paint(color);
  printf("prompt:> ");
}


/* ========================================================================= */
/*                          PRIVATE HELPERS                                  */
/* ========================================================================= */

static void ge_hp_bar(int hp, char *buf, int sz) {
  int filled, empty, i, pos;
  if (!buf || sz < 2) return;
  if (hp < 0) hp = 0;
  filled = (hp > MAX_HP_BAR) ? MAX_HP_BAR : hp;
  empty  = MAX_HP_BAR - filled;
  pos = 0;
  buf[pos++] = '[';
  for (i = 0; i < filled && pos < sz - 5; i++) buf[pos++] = '#';
  for (i = 0; i < empty  && pos < sz - 5; i++) buf[pos++] = '-';
  pos += snprintf(buf + pos, sz - pos, "] %d", hp);
  buf[pos] = '\0';
}

static const char *ge_hp_warn(int hp) {
  if (hp <= 2) return "!!!";
  if (hp <= 4) return "!";
  return "";
}

static void ge_build_char_str(Game *game, Space *space, char *buf, int sz) {
  int i, total;
  Character *ch;
  char *gdsc;
  buf[0] = '\0';
  if (!space || !game || !buf || sz <= 0) return;
  total = game_get_n_characters(game);
  for (i = 0; i < total; i++) {
    ch = game_get_character_at(game, i);
    if (ch && space_contains_character(space, character_get_id(ch)) == TRUE) {
      gdsc = character_get_gdesc(ch);
      if (gdsc) {
        if (buf[0] != '\0') strncat(buf, " ", sz - (int)strlen(buf) - 1);
        strncat(buf, gdsc, sz - (int)strlen(buf) - 1);
        free(gdsc);
      }
    }
  }
}

static void ge_build_player_str(Game *game, Id space_id, char *buf, int sz) {
  int i, total;
  Player *p;
  char *gdsc;
  buf[0] = '\0';
  if (!game || !buf || sz <= 0 || space_id == NO_ID) return;
  total = game_get_n_players(game);
  for (i = 0; i < total; i++) {
    p = game_get_player_at(game, i);
    if (p && player_get_location(p) == space_id) {
      gdsc = player_get_gdesc(p);
      if (gdsc) {
        if (buf[0] != '\0') strncat(buf, " ", sz - (int)strlen(buf) - 1);
        strncat(buf, gdsc, sz - (int)strlen(buf) - 1);
        free(gdsc);
      }
    }
  }
}

static void ge_build_obj_str(Game *game, Space *space, char *buf, int sz) {
  int i, total, first = 1;
  Object *obj;
  buf[0] = '\0';
  if (!space || !game || !buf || sz <= 0) return;
  total = game_get_n_objects(game);
  for (i = 0; i < total; i++) {
    obj = game_get_object_at(game, i);
    if (obj && space_contains_object(space, obj_get_id(obj)) == TRUE) {
      if (!first) strncat(buf, ",", sz - (int)strlen(buf) - 1);
      strncat(buf, obj_get_name(obj), sz - (int)strlen(buf) - 1);
      first = 0;
    }
  }
}

static const char *ge_status_str(Status result) {
  if (result == OK)            return "OK";
  if (result == ERROR_Attack)  return "ERROR: attack failed";
  if (result == ERROR_Chat)    return "ERROR: chat failed";
  if (result == ERROR_dir)     return "ERROR: can't go there";
  if (result == ERROR_take)    return "ERROR: can't take that";
  if (result == ERROR_drop)    return "ERROR: can't drop that";
  if (result == ERROR_inspect) return "ERROR: can't inspect that";
  return "ERROR";
}