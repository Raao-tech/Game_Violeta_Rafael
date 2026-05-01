/**
 * @brief It defines the game loop
 *
 * @file game_loop.c
 * @author Profesores PPROG, Rafael, Salvador and Javier Jarque
 * @version 3
 * @date 08-04-2026
 * @copyright GNU Public License
 */

#include "command.h"
#include "game.h"
#include "game_actions.h"
#include "game_management.h"
#include "graphic_engine.h"
#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define FLAG_DET "-d"
#define FLAG_LOG "-l"
#define FLAG_TEST "-t"

/* ------------------------------------------------------------------------- */
/* Macro: print                                                              */
/*                                                                           */
/* Imprime una linea de log al fichero indicado. Si Skill != NO_ID se        */
/* incluye el id de la skill usada por el numen activo.                      */
/* IMPORTANTE: la macro NO termina en ';'. El ';' lo pone quien la usa.      */
/* ------------------------------------------------------------------------- */
#define print(file, cmd, targ, Skill, res)                                                                                                           \
	do                                                                                                                                               \
		{                                                                                                                                            \
			if (file)                                                                                                                                \
				{                                                                                                                                    \
					if ((Skill) != NO_ID)                                                                                                            \
						{                                                                                                                            \
							fprintf ((file), "%s: %s    Used_Skill: %ld  Result: %s\n", (cmd) ? (cmd) : "Unknown",                                   \
									 (targ) ? (targ) : "Target_unknown", (long)(Skill), (res) ? (res) : "Strange_Answer");                           \
						}                                                                                                                            \
					else                                                                                                                             \
						{                                                                                                                            \
							fprintf ((file), "%s: %s Result: %s\n", (cmd) ? (cmd) : "Unknown", (targ) ? (targ) : "Target_unknown",                   \
									 (res) ? (res) : "Strange_Answer");                                                                              \
						}                                                                                                                            \
					fflush ((file));                                                                                                                 \
				}                                                                                                                                    \
		}                                                                                                                                            \
	while (0)

#define print_error(mssg, md_test)                                                                                                                   \
	do                                                                                                                                               \
		{                                                                                                                                            \
			if (mssg) fprintf (stderr, "%s   Modo: %s\n", (mssg), ((md_test) == TRUE) ? "Test" : "User");                                            \
		}                                                                                                                                            \
	while (0)

/* ------------------------------------------------------------------------- */
/* Status_init                                                               */
/*                                                                           */
/* Codigos de retorno de las funciones de inicializacion del game_loop.      */
/* INIT_OK debe ser el primero (valor 0) por convencion: "exito = 0".        */
/* Todos los nombres tienen prefijo INIT_ para no chocar con CommandCode     */
/* (EXIT) ni con Status (OK, ERROR).                                         */
/* ------------------------------------------------------------------------- */
typedef enum
{
	INIT_OK,
	INIT_EXIT,      /*<! El usuario cerro el menu sin elegir partida */
	INIT_ERR_GLOOP, /*<! Puntero game_loop nulo */
	INIT_ERR_GAME,  /*<! Fallo en game_management_create_from_file */
	INIT_ERR_FILE,  /*<! Nombre de fichero invalido o no existe */
	INIT_ERR_CMD,   /*<! No se pudo recuperar el last_cmd del Game */
	INIT_ERR_GRAPH, /*<! Fallo creando el Graphic_engine */
	INIT_ERR_UNKNOW
} Status_init;

typedef struct _Game_loop
{
	Graphic_engine* gp_raylib;
	Game* game;
	Command* last_cmd;
} GameLoop;

/* ========================================================================= */
/*                          PROTOTIPOS DEL MODULO                            */
/* ========================================================================= */

GameLoop* game_loop_create (void);
Status_init game_loop_init_user (GameLoop* game_loop);
Status_init game_loop_init_test (GameLoop* game_loop, char* file_name);
void game_loop_cleanup (GameLoop* gameloop, FILE* log_file);
void game_loop_print_log (Game* game, Command* last_cmd, FILE* log_file);

/* ========================================================================= */
/*                                 MAIN                                      */
/* ========================================================================= */

int
main (int argc, char* argv[])
{
	GameLoop* game_loop                = NULL;
	FILE* log_file                     = NULL;
	Bool log_enabled                   = FALSE;
	Bool test_enabled                  = FALSE;
	Bool is_determinist                = FALSE;
	Status_init result                 = INIT_ERR_UNKNOW;
	char name_file_data[WORD_SIZE + 1] = "";
	int i_flag                         = 0;
	unsigned int seed                  = 0;

	/*=============== COMPROBACION DE recursos minimos =====================*/
	if (argc < 1)
		{
			fprintf (stderr, "Use: %s  [-l <log_file>][-d <seed>][-t <game_data_file>]\n", argv[0]);
			return 1;
		}

	/*=============== FLAGS -l -d -t =======================================*/
	if (argc >= 2)
		{
			for (i_flag = 1; i_flag < argc; i_flag++)
				{
					/* -l <log_file> */
					if ((i_flag + 1) < argc && argv[i_flag] && strcmp (FLAG_LOG, argv[i_flag]) == 0)
						{
							if (argv[i_flag + 1] != NULL)
								{
									log_file = fopen (argv[++i_flag], "w");
									if (!log_file)
										{
											print_error ("Error: Cann't open the file pass", test_enabled);
											return 1;
										}
									log_enabled = TRUE;
								}
						}
					/* -d <seed> */
					if (((i_flag + 1) < argc) && argv[i_flag] && strcmp (FLAG_DET, argv[i_flag]) == 0)
						{
							is_determinist = TRUE;
							seed           = (unsigned int)atoi (argv[++i_flag]);
						}
					/* -t <game_data_file> */
					if (((i_flag + 1) < argc) && argv[i_flag] && strcmp (FLAG_TEST, argv[i_flag]) == 0)
						{
							strncpy (name_file_data, argv[++i_flag], WORD_SIZE);
							name_file_data[WORD_SIZE] = '\0';
							test_enabled              = TRUE;
						}
				}
		}

	if (is_determinist == TRUE) { srand (seed); }
	else    {srand ((unsigned int)time (NULL));}

	/*=============== INICIALIZACION DEL JUEGO =============================*/

	game_loop = game_loop_create ();
	if (!game_loop)
		{
			print_error ("ERROR: Cann't create game_loop", test_enabled);
			if (log_file) fclose (log_file);
			return 1;
		}

	if (test_enabled == TRUE) { result = game_loop_init_test (game_loop, name_file_data); }
	else	{result = game_loop_init_user (game_loop);}

	switch (result)
		{
			case INIT_OK: break;
			case INIT_EXIT:
				game_loop_cleanup (game_loop, log_file); /* El usuario cerro el menu pulsando Exit. Salida limpia. */
				return 0;
			case INIT_ERR_GLOOP:
				print_error ("ERROR: game_loop is NULL", test_enabled);
				game_loop_cleanup (game_loop, log_file);
				return 1;
			case INIT_ERR_GAME:
				print_error ("ERROR: Cann't init game", test_enabled);
				game_loop_cleanup (game_loop, log_file);
				return 1;
			case INIT_ERR_FILE:
				print_error ("ERROR: Anysomething wrong whit file", test_enabled);
				game_loop_cleanup (game_loop, log_file);
				return 1;
			case INIT_ERR_CMD:
				print_error ("ERROR: Cann't recover last_cmd", test_enabled);
				game_loop_cleanup (game_loop, log_file);
				return 1;
			case INIT_ERR_GRAPH:
				print_error ("ERROR: Cann't init graph", test_enabled);
				game_loop_cleanup (game_loop, log_file);
				return 1;
			default:
				print_error ("ERROR: Unknow this error, please contact us", test_enabled);
				game_loop_cleanup (game_loop, log_file);
				return 1;
		}

	/*=============== BUCLE PRINCIPAL ======================================*/
	/*
	 * Bloque 2: aqui ira el while real.
	 *
	 * Modo test:
	 *   while (cmd != EXIT && !game_finished) {
	 *     command_get_user_input(last_cmd);
	 *     game_actions_update(game, last_cmd);
	 *     if (log_enabled) game_loop_print_log(game, last_cmd, log_file);
	 *   }
	 *
	 * Modo visual:
	 *   while (!WindowShouldClose() && cmd != EXIT && !game_finished) {
	 *     graphic_engine_paint_game(gp_raylib, game);
	 *     command_raylib_get_user_input(last_cmd);
	 *     if (command_has_input(last_cmd))
	 *         game_actions_update(game, last_cmd);
	 *   }
	 */

	game_loop_cleanup (game_loop, log_file);
	return 0;
}

/* ========================================================================= */
/*                       CREATE / INIT / CLEANUP                             */
/* ========================================================================= */

/**
 * @brief	Esta función inicializa el objeto gameLoop que permite getionar los comanods, la grafica y el estado del jueg
 */
GameLoop*
game_loop_create (void)
{
	GameLoop* new_game_loop = (GameLoop*)malloc (sizeof (GameLoop));
	if (!new_game_loop) return NULL;

	new_game_loop->game      = NULL;
	new_game_loop->last_cmd  = NULL;
	new_game_loop->gp_raylib = NULL;

	return new_game_loop;
}

Status_init
game_loop_init_test (GameLoop* game_loop, char* file_name)
{
	if (!game_loop) return INIT_ERR_GLOOP;
	if (!file_name) return INIT_ERR_FILE;

	if (game_management_create_from_file (&game_loop->game, file_name) == ERROR) return INIT_ERR_GAME;

	game_loop->last_cmd = game_get_last_command (game_loop->game);
	if (!game_loop->last_cmd) return INIT_ERR_CMD;

	return INIT_OK;
}

Status_init
game_loop_init_user (GameLoop* game_loop)
{
	MenuResult result_ge;

	if (!game_loop) return INIT_ERR_GLOOP;

	/* 1. Crear el motor grafico */
	game_loop->gp_raylib = graphic_engine_create ();
	if (!game_loop->gp_raylib) return INIT_ERR_GRAPH;

	/* 2. Mostrar el menu y recoger la decision del usuario */
	result_ge = graphic_engine_init (game_loop->gp_raylib);

	/* Si el usuario salio del menu (cerro ventana o pulso Exit) */
	if (result_ge.menu_out == OUT_ERR || result_ge.menu_out == EXIT_Q) { return INIT_EXIT; }

	/* 3. Cargar la partida desde el .dat elegido en el menu */
	if (game_management_create_from_file (&game_loop->game, result_ge.data_name) == ERROR) { return INIT_ERR_GAME; }

	/* 4. Recuperar el last_cmd que vive dentro del Game */
	game_loop->last_cmd = game_get_last_command (game_loop->game);
	if (!game_loop->last_cmd) return INIT_ERR_CMD;

	/* 5. Establecer Numen inicial como active si entro por NEW_GAME*/
	if (result_ge.menu_out == NEW_GAME && result_ge.init_numen != NO_ID)
		player_set_active_numen (game_get_player (game_loop->game), result_ge.init_numen);

	return INIT_OK;
}

void
game_loop_cleanup (GameLoop* gameloop, FILE* log_file)
{
	if (gameloop)
		{
			if (gameloop->game) game_destroy (gameloop->game);
			if (gameloop->gp_raylib) graphic_engine_destroy (gameloop->gp_raylib);
			free (gameloop);
		}
	if (log_file) fclose (log_file);
}

/* ========================================================================= */
/*                               PRINT LOG                                   */
/* ========================================================================= */

void
game_loop_print_log (Game* game, Command* last_cmd, FILE* log_file)
{
	if (!game || !last_cmd) return;
	if (log_file)
		{
			Status status          = game_get_last_cmd_status (game);
			CommandCode cmd_code   = command_get_code (last_cmd);
			Id skill_id            = NO_ID; /* TODO Bloque 1: command_get_skill(last_cmd) */
			char* target_name      = command_get_target (last_cmd);
			const char* result_str = (status == OK) ? "OK" : "ERROR";

			target_name            = target_name ? target_name : "UNKNOW";

			switch (cmd_code)
				{
					case EXIT: print (log_file, "Exit", "Generic", NO_ID, result_str); break;
					case MOVE: print (log_file, "Move", target_name, skill_id, result_str); break;
					case WALK: print (log_file, "Walk", target_name, skill_id, result_str); break;
					case TAKE: print (log_file, "Take", target_name, skill_id, result_str); break;
					case DROP: print (log_file, "Drop", target_name, skill_id, result_str); break;
					case ATTACK: print (log_file, "Attack", target_name, skill_id, result_str); break;
					case CHAT: print (log_file, "Chat", target_name, skill_id, result_str); break;
					case INSPECT: print (log_file, "Inspect", target_name, skill_id, result_str); break;
					case USE: print (log_file, "Use", target_name, skill_id, result_str); break;
					case OPEN: print (log_file, "Open", target_name, skill_id, result_str); break;
					case SAVE: print (log_file, "Save", target_name, skill_id, result_str); break;
					default: print (log_file, "UNKNOW", "???", NO_ID, "ERROR"); break;
				}
		}
}