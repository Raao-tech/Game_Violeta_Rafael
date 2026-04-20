/**
 * @brief It defines the game loop
 *
 * @file game_loop.c
 * @author Profesores PPROG, Rafael, Salvador and Javier Jarque
 * @version 3
 * @date 08-04-2026
 * @copyright GNU Public License
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "command.h"
#include "raylib.h"
#include "game.h"
#include "game_management.h"
#include "game_actions.h"
#include "graphic_engine.h"

/**
 * @brief Initializes the game and the graphic engine from a data file
 * @author Profesores PPROG
 *
 * @param game pointer to pointer to the game (will be allocated)
 * @param gengine pointer to pointer to the graphic engine (will be allocated)
 * @param file_name path to the .dat file
 * @return 0 on success, 1 if game init fails, 2 if graphic engine fails
 */
int game_loop_init(Game **game, Graphic_engine **gengine, char *file_name);

/**
 * @brief Frees the game, graphic engine and closes the log file
 * @author Profesores PPROG
 *
 * @param game pointer to the game
 * @param gengine pointer to the graphic engine
 * @param log_file pointer to the log FILE (may be NULL)
 */
void game_loop_cleanup(Game *game, Graphic_engine *gengine, FILE *log_file);


/* ========================================================================= */
/*                                 MAIN                                      */
/* ========================================================================= */

int main(int argc, char *argv[]) {
  FILE           *log_file = NULL;
  Game           *game     = NULL;
  Graphic_engine *gengine  = NULL;
  Command        *last_cmd = NULL;
  int             result;
  Bool            log_enabled = FALSE;

  /* ---- Validate minimum arguments ---- */
  if (argc < 2) {
    fprintf(stderr, "Use: %s <game_data_file> [-l <log_file>]\n", argv[0]);
    return 1;
  }

  /* ---- Parse optional LOG arguments ---- */
  /* We check argc >= 4 AND argv[2] == "-l" before opening the log file.*/
  if (argc >= 4 && strcmp(argv[2], "-l") == 0) {
    log_file = fopen(argv[3], "w");
    if (!log_file) {
      fprintf(stderr, "Error: cannot open log file '%s' for writing.\n", argv[3]);
      return 1;
    }
    log_enabled = TRUE;
  }

  /* Seed random number generator for attack rolls */
  srand(time(NULL));

  /* ---- Initialize game and graphic engine ---- */
  result = game_loop_init(&game, &gengine, argv[1]);

  switch (result) {
    case 0:  break;
    case 1:  fprintf(stderr, "Error while initializing game.\n");
             if (log_file) fclose(log_file);
             return 1;
    case 2:  fprintf(stderr, "Error while initializing graphic engine.\n");
             if (log_file) fclose(log_file);
             return 1;
    default: fprintf(stderr, "Error: unknown initialization failure.\n");
             if (log_file) fclose(log_file);
             return 1;
  }

  /* ---- Get the command struct (reused every turn) ---- */
  last_cmd = game_get_last_command(game);
  if (!last_cmd) {
    fprintf(stderr, "Error: could not get command struct.\n");
    game_loop_cleanup(game, gengine, log_file);
    return 1;
  }

  /* ========== GAME LOOP ========== */
  while (command_get_code(last_cmd) != EXIT &&
         game_get_finished(game) == FALSE) {

    /* 1. RENDER — paint the current state */
    graphic_engine_paint_game(gengine, game);

    /* 2. INPUT — read the next command from stdin */
    command_get_user_input(last_cmd);

    /* 3. UPDATE — apply the command to the game state */
    game_actions_update(game, last_cmd);

    /* 4. LOG — if enabled, write the command and its result */
    if (log_enabled && log_file) {
      Status      status   = game_get_last_cmd_status(game);
      CommandCode cmd_code = command_get_code(last_cmd);
      char       *obj_name = command_get_obj(last_cmd);
      const char *result_str = (status == OK) ? "OK" : "ERROR";


      switch (cmd_code) {
        case EXIT:
          fprintf(log_file, "exit: %s\n", result_str);
          break;
        case MOVE:
          fprintf(log_file, "move %s: %s\n",
                  obj_name ? obj_name : "", result_str);
          break;
        case TAKE:
          fprintf(log_file, "take %s: %s\n",
                  obj_name ? obj_name : "", result_str);
          break;
        case DROP:
          fprintf(log_file, "drop %s: %s\n",
                  obj_name ? obj_name : "", result_str);
          break;
        case ATTACK:
          fprintf(log_file, "attack %s: %s\n",
                  obj_name ? obj_name : "", result_str);
          break;
        case CHAT:
          fprintf(log_file, "chat %s: %s\n",
                  obj_name ? obj_name : "", result_str);
          break;
        case INSPECT:
          fprintf(log_file, "inspect %s: %s\n",
                  obj_name ? obj_name : "", result_str);
          break;
        case USE:
          fprintf(log_file, "use %s: %s\n",
                  obj_name ? obj_name : "", result_str);
          break;
        case OPEN:
          fprintf(log_file, "open %s: %s\n",
                  obj_name ? obj_name : "", result_str);
          break;
        case SAVE:
          fprintf(log_file, "save: %s\n", result_str);
          break;
        default:
          break;
      }
    }

    /* 5. TURN — advance to the next player (F11 multiplayer) */
    game_turn_update(game);
  }

  /* ---- Cleanup ---- */
  game_loop_cleanup(game, gengine, log_file);

  return 0;
}


/* ========================================================================= */
/*                            INIT / CLEANUP                                 */
/* ========================================================================= */

int game_loop_init(Game **game, Graphic_engine **gengine, char *file_name) {
  if (game_create_from_file(game, file_name) == ERROR)
    return 1;

  *gengine = graphic_engine_create();
  if (*gengine == NULL) {
    game_destroy(*game);
    return 2;
  }

  return 0;
}

void game_loop_cleanup(Game *game, Graphic_engine *gengine, FILE *log_file) {
  game_destroy(game);
  graphic_engine_destroy(gengine);

  if (log_file) fclose(log_file);
}
