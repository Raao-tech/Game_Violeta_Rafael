/**
 * @brief It defines the game loop
 *
 * @file game_loop.c
 * @author Profesores PPROG and Javier Jarque
 * @version 3
 * @date 08-04-2026
 * @copyright GNU Public License
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "command.h"
#include "game.h"
#include "game_reader.h"
#include "game_actions.h"
#include "graphic_engine.h"

int game_loop_init(Game **game, Graphic_engine **gengine, char *file_name);
void game_loop_cleanup(Game *game, Graphic_engine *gengine, FILE *file);

int main(int argc, char *argv[])
{
  FILE * file;
  Game *game = NULL;
  Graphic_engine *gengine = NULL;
  int result;
  Command *last_cmd;
  char* t_file;
  if (argv != NULL)
  {
  strcpy( t_file, argv[2]);
  if (argv[3] != NULL)
  {
  char* n_file;
  strcpy(n_file, argv[3]);
  memcpy;
  file = fopen(n_file, "w");
  }
  }
  /* Seed random for attack */
  srand(time(NULL));

  if (argc < 2)
  {
    fprintf(stderr, "Use: %s <game_data_file>\n", argv[0]);
    return 1;
  }

  result = game_loop_init(&game, &gengine, argv[1]);

  if (result == 1)
  {
    fprintf(stderr, "Error while initializing game.\n");
    return 1;
  }
  else if (result == 2)
  {
    fprintf(stderr, "Error while initializing graphic engine.\n");
    return 1;
  }

  last_cmd = game_get_last_command(game);

  while ((command_get_code(last_cmd) != EXIT) && (game_get_finished(game) == FALSE))
  {
    graphic_engine_paint_game(gengine, game);
    command_get_user_input(last_cmd);
    game_actions_update(game, last_cmd);
    /*implementation of LOG*/
    if (strcmp(t_file, "-l") == 0)
    {
    if (file != NULL)
    {
      Status status = game_get_last_cmd_status(game);
      char *obj = command_get_obj(last_cmd);
      CommandCode* comand_c = command_get_code(last_cmd);
      if (comand_c == EXIT || status == OK)
      {
        fprintf(file, "exit: OK\n");
      }
      else if (comand_c == EXIT || status == ERROR)
      {
        fprintf(file, "exit: ERROR\n");
      }
      else if (comand_c == TAKE || status == OK)
      {
        fprintf(file, "take %s: OK\n", obj);
      }
      else if (comand_c == TAKE || status == ERROR)
      {
        fprintf(file, "take %s: ERROR\n", obj);
      }
      else if (comand_c == DROP || status == OK)
      {
        fprintf(file, "drop %s: OK\n", obj);
      }
      else if (comand_c == DROP || status == ERROR)
      {
        fprintf(file, "drop %s: ERROR\n", obj);
      }
      else if (comand_c == ATTACK || status == OK)
      {
        fprintf(file, "attack %s: OK\n", obj);
      }
      else if (comand_c == ATTACK || status == ERROR)
      {
        fprintf(file, "attack %s: ERROR\n", obj);
      }
      else if (comand_c == CHAT || status == OK)
      {
        fprintf(file, "chat %s: OK\n", obj);
      }
      else if (comand_c == CHAT || status == ERROR)
      {
        fprintf(file, "chat %s: ERROR\n", obj);
      }
      else if (comand_c == INSPECT || status == OK)
      {
        fprintf(file, "inspect %s: OK\n", obj);
      }
      else if (comand_c == INSPECT || status == ERROR)
      {
        fprintf(file, "inspect %s: ERROR\n", obj);
      }
    }
  }
  }

  /*game_loop cleanup*/
  game_loop_cleanup(game, gengine, file);

  return 0;
}

int game_loop_init(Game **game, Graphic_engine **gengine, char *file_name)
{
  if (game_create_from_file(game, file_name) == ERROR)
  {
    return 1;
  }

  if ((*gengine = graphic_engine_create()) == NULL)
  {
    game_destroy(*game);
    return 2;
  }

  return 0;
}

void game_loop_cleanup(Game *game, Graphic_engine *gengine, FILE *file)
{
  game_destroy(game);
  graphic_engine_destroy(gengine);
  fclose(file);
}
