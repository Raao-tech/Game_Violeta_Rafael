/**
 * @brief It implements a textual graphic engine
 *
 * @file graphic_engine.c
 * @author Profesores PPROG
 * @version 0
 * @date 24-01-2026
 * @copyright GNU Public License
 */

#include "graphic_engine.h"

#include <stdio.h>
#include <stdlib.h>

#include "command.h"
#include "libscreen.h"
#include "space.h"
#include "types.h"

#define WIDTH_MAP 85
#define WIDTH_DES 50
#define WIDTH_BAN 50
#define HEIGHT_MAP 25
#define HEIGHT_BAN 1
#define HEIGHT_HLP 2
#define HEIGHT_FDB 3

struct _Graphic_engine
{
  Area *map, *descript, *banner, *help, *feedback;
};

Graphic_engine *graphic_engine_create()
{
  static Graphic_engine *ge = NULL;

  if (ge)
  {
    return ge;
  }

  screen_init(HEIGHT_MAP + HEIGHT_BAN + HEIGHT_HLP + HEIGHT_FDB + 4, WIDTH_MAP + WIDTH_DES + 3);
  ge = (Graphic_engine *)calloc(1, sizeof(Graphic_engine));
  if (ge == NULL)
  {
    return NULL;
  }

  ge->map = screen_area_init(1, 1, WIDTH_MAP, HEIGHT_MAP);
  ge->descript = screen_area_init(WIDTH_MAP + 2, 1, WIDTH_DES, HEIGHT_MAP);
  ge->banner = screen_area_init((int)((WIDTH_MAP + WIDTH_DES + 1 - WIDTH_BAN) / 2), HEIGHT_MAP + 2, WIDTH_BAN, HEIGHT_BAN);
  ge->help = screen_area_init(1, HEIGHT_MAP + HEIGHT_BAN + 2, WIDTH_MAP + WIDTH_DES + 1, HEIGHT_HLP);
  ge->feedback = screen_area_init(1, HEIGHT_MAP + HEIGHT_BAN + HEIGHT_HLP + 3, WIDTH_MAP + WIDTH_DES + 1, HEIGHT_FDB);

  return ge;
}

void graphic_engine_destroy(Graphic_engine *ge)
{
  if (!ge)
    return;

  screen_area_destroy(ge->map);
  screen_area_destroy(ge->descript);
  screen_area_destroy(ge->banner);
  screen_area_destroy(ge->help);
  screen_area_destroy(ge->feedback);

  screen_destroy();
  free(ge);
}

void graphic_engine_paint_game(Graphic_engine *ge, Game *game)
{
  Id id_act = NO_ID, id_back = NO_ID, id_next = NO_ID, id_left = NO_ID, id_rigth = NO_ID;
  Space *space_act = NULL;
  char obj_back = '\0';
  char obj_next = '\0';
  char obj_act = '\0';
  char obj_right = '\0';
  char obj_left = '\0';
  char str[255];
  CommandCode last_cmd = UNKNOWN;
  extern char *cmd_to_str[N_CMD][N_CMDT];

  /* Paint the in the map area */
  screen_area_clear(ge->map);
  if ((id_act = game_get_player_location(game)) != NO_ID)
  {
    space_act = game_get_space(game, id_act);
    id_back = space_get_north(space_act);
    id_next = space_get_south(space_act);
    id_left = space_get_east(space_act);
    id_rigth = space_get_west(space_act);

    if (game_get_object_location(game) == id_back)
      obj_back = '+';
    else
      obj_back = ' ';

    if (id_back != NO_ID)
    {
      sprintf(str, "+++++++++++++++++++++++++++++------------++++++++++++++++++++++++++++");
      screen_area_puts(ge->map, str);
      sprintf(str, "++++++++++++++++++++++++++++|         %2d|++++++++++++++++++++++++++++", (int)id_back);
      screen_area_puts(ge->map, str);
      sprintf(str, "++++++++++++++++++++++++++++|            |++++++++++++++++++++++++++++");
      screen_area_puts(ge->map, str);
      sprintf(str, "++++++++++++++++++++++++++++|     %c     |++++++++++++++++++++++++++++", obj_back);
      screen_area_puts(ge->map, str);
      sprintf(str, "+++++++++++++++++++++++++++++-----------+++++++++++++++++++++++++++++");
      screen_area_puts(ge->map, str);
      sprintf(str, "                                  ^                                ");
      screen_area_puts(ge->map, str);
    }

    if (game_get_object_location(game) == id_act)
      obj_act = '+';
    else
      obj_act = ' ';

      
    if (game_get_object_location(game) == id_left)
      obj_left = '+';
    else
      obj_left = ' ';
 
    if (game_get_object_location(game) == id_rigth)
      obj_left = '+';
    else
      obj_left = ' ';


        //CASO IZQUEIRDA DERECHA Y ACTUAL
    if (id_act != NO_ID && id_act < 99 &&  id_left != NO_ID &&  id_rigth != NO_ID )
    {
      sprintf(str, " +------------+            +------------+             +------------+ ");
      screen_area_puts(ge->map, str);
      sprintf(str, " |         %2d|       <    | >8D     %2d|    >        |         %2d| ", (int)id_act, (int)id_left,  (int)id_rigth);
      screen_area_puts(ge->map, str);
      sprintf(str, " |      %c    |       <    |     %c     |     >       |      %c    | ", obj_right, obj_act, obj_left);
      screen_area_puts(ge->map, str);
      sprintf(str, " |             |      <    |            |     >       |            | ");
      sprintf(str, " +------------+            +------------+             +------------+ ");
      screen_area_puts(ge->map, str);
      //CASO IZQUIERDA Y ACTUAL
    }else if(id_act != NO_ID  && id_left != NO_ID){
      
      sprintf(str, " +------------+            +------------+++++++++++++++++++++++++++++");
      screen_area_puts(ge->map, str);
      sprintf(str, " |         %2d|       <    | >8D     %2d|++++++++++++++++++++++++++++",  (int)id_left, (int)id_act);
      screen_area_puts(ge->map, str);
      sprintf(str, "|             |        <   |            |++++++++++++++++++++++++++++");
      screen_area_puts(ge->map, str);
      sprintf(str, " |      %c    |       <    |     %c     |++++++++++++++++++++++++++++", obj_left, obj_act);
      sprintf(str, " +------------+            +------------+++++++++++++++++++++++++++++");
      screen_area_puts(ge->map, str);
      //CASO DERECHA Y ACTUAL
    }else if(id_act != NO_ID  && id_rigth != NO_ID){
      sprintf(str, "++++++++++++++++++++++++++++------------+              +------------+");
      screen_area_puts(ge->map, str);
      sprintf(str, "+++++++++++++++++++++++++++| >8D     %2d|       >       |         %2d |", (int)id_act,  (int)id_rigth);
      screen_area_puts(ge->map, str);
      sprintf(str, "+++++++++++++++++++++++++++|     %c     |       >      |      %c      |", obj_act, obj_right);
      screen_area_puts(ge->map, str);
      sprintf(str, "+++++++++++++++++++++++++++|            |       >      |              |");
      screen_area_puts(ge->map, str);
      sprintf(str, "++++++++++++++++++++++++++++------------+              +-------------+");
      screen_area_puts(ge->map, str);
    }else if(id_act != NO_ID){
      sprintf(str, "++++++++++++++++++++++++++++-------------++++++++++++++++++++++++++++");
      screen_area_puts(ge->map, str);
      sprintf(str, "+++++++++++++++++++++++++++| >8D       %2d|+++++++++++++++++++++++++++", (int)id_act);
      screen_area_puts(ge->map, str);
      sprintf(str, "+++++++++++++++++++++++++++|     %c       |+++++++++++++++++++++++++++", obj_act);
      screen_area_puts(ge->map, str);
      sprintf(str, "+++++++++++++++++++++++++++|             |+++++++++++++++++++++++++++");
      screen_area_puts(ge->map, str);
      sprintf(str, "+++++++++++++++++++++++++++|             |+++++++++++++++++++++++++++");
      screen_area_puts(ge->map, str);
      sprintf(str, "++++++++++++++++++++++++++++-------------++++++++++++++++++++++++++++");
      screen_area_puts(ge->map, str);
      
      
    }

    if (game_get_object_location(game) == id_next)
      obj_next = '+';
    else
      obj_next = ' ';

    if (id_next != NO_ID)
    {
      sprintf(str, "        v");
      screen_area_puts(ge->map, str);
      sprintf(str, "+++++++++++++++++++++++++++++------------+++++++++++++++++++++++++++++");
      screen_area_puts(ge->map, str);
      sprintf(str, "++++++++++++++++++++++++++++|         %2d|++++++++++++++++++++++++++++", (int)id_next);
      screen_area_puts(ge->map, str);
      sprintf(str, "++++++++++++++++++++++++++++|     %c     |++++++++++++++++++++++++++++", obj_next);
      screen_area_puts(ge->map, str);
      sprintf(str, "+++++++++++++++++++++++++++++-----------+++++++++++++++++++++++++++++");
      screen_area_puts(ge->map, str);
    }


  }

  /* Paint in the description area */
  screen_area_clear(ge->descript);

  Id player_loc = game_get_player_location(game);
  Id object_loc = game_get_object_location(game);

  sprintf(str, " Player in: %ld", (long)player_loc);
  screen_area_puts(ge->descript, str);

  sprintf(str, " Object in: %ld", (long)object_loc);
  screen_area_puts(ge->descript, str);

  screen_area_puts(ge->descript, " -------------------------");

  /* List all spaces loaded */
  int n = game_get_n_spaces(game);
  for (int i = 0; i < n; i++) {
    Id sid = game_get_space_id_at(game, i);
    Space *sp = game_get_space(game, sid);

    if (!sp) continue;

    sprintf(str, "[%ld]%s%s",
            (long)sid,
            (sid == player_loc) ? " <P>" : "",
            (sid == object_loc) ? " <O>" : "");
    screen_area_puts(ge->descript, str);

    sprintf(str, " N:%ld S:%ld E:%ld W:%ld",
            (long)space_get_north(sp),
            (long)space_get_south(sp),
            (long)space_get_east(sp),
            (long)space_get_west(sp));

    screen_area_puts(ge->descript, str);
  }

  /* Paint in the banner area */
  screen_area_puts(ge->banner, " The haunted castle game ");

  /* Paint in the help area */
  screen_area_clear(ge->help);
  sprintf(str, " The commands you can use are:");
  screen_area_puts(ge->help, str);
  sprintf(str, "     next(n) back(b) left(l) right(r) take(t) drop(d) exit(e)");
  screen_area_puts(ge->help, str);

  /* Paint in the feedback area */
  last_cmd = command_get_code(game_get_last_command(game));
  sprintf(str, " %s (%s)", cmd_to_str[last_cmd - NO_CMD][CMDL], cmd_to_str[last_cmd - NO_CMD][CMDS]);
  screen_area_puts(ge->feedback, str);

  /* Dump to the terminal */
  screen_paint();
  printf("prompt:> ");
}
