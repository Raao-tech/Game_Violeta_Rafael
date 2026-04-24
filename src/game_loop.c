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

#define	FLAG_DET 	"-d"
#define	FLAG_LOG 	"-l"
#define	FLAG_TEST	"-t"  /*<! Print permite imprimir el mesaje de log en el file indicado*/
#define print(file, cmd, targ, Skill, res) do { \ 
    if (file) { \
        if (Skill != NO_ID) { \
            fprintf(file, "%s: %s    Used_Skill: %ld  Result: %s\n", \
                (cmd)   ? (cmd)   : "Unknown", \
                (targ)  ? (targ)  : "Target_unknown", \
                (long)(Skill), \
                (res)   ? (res)   : "Strange_Answer"); \
        } else { \
            fprintf(file, "%s: %s Result: %s\n", \
                (cmd)   ? (cmd)   : "Unknown", \
                (targ)  ? (targ)  : "Target_unknown", \
                (res)   ? (res)   : "Strange_Answer"); \
        } \
        fflush(file); \
    } \
} while(0)


typedef struct _Game_loop
{
	Graphic_engine* gp_raylib;
	Game*			game;
	Command*		last_cmd;
}GameLoop;


	/*Se podría pedir la hora y la ínea en la que se ejecutó el comando y mostrarlo*/

/**
 * @brief Initializes the game and the graphic engine from a data file on mode visual to commun user
 * @author Rafael
 *
 * @param game pointer to pointer to the game (will be allocated)
 * @param gengine pointer to pointer to the graphic engine (will be allocated)
 * @param file_name path to the .dat file
 * @return 0 on success, 1 if game init fails, 2 if graphic engine fails
 */
int game_loop_init_user(GameLoop *game_loop, char *file_name);


/**
 * @brief Initializes the game and the graphic engine from a data file on mode Test to other devs
 * @author Rafael
 *
 * @param game pointer to pointer to the game (will be allocated)
 * @param gengine pointer to pointer to the graphic engine (will be allocated)
 * @param file_name path to the .dat file
 * @return 0 on success, 1 if game init fails, 2 if graphic engine fails
 */
int game_loop_init_test(GameLoop* game_loop, char *file_name);

/**
 * @brief Frees the game, graphic engine and closes the log file
 * @author Profesores PPROG y Rafael
 *
 * @param game pointer to the game
 * @param gengine pointer to the graphic engine
 * @param log_file pointer to the log FILE (may be NULL)
 */
void game_loop_cleanup(GameLoop* gameloop, FILE *log_file);


/**
 * @brief print on file log
 * @author Rafael
 *
 * @param game pointer to the game
 * @param last_cmd pointer to the last command on game
 * @param log_file pointer to the log FILE (may be NULL)
 */
void game_loop_print_log(Game *game, Command* last_cmd, FILE* log_file);


/* ========================================================================= */
/*                                 MAIN                                      */
/* ========================================================================= */

int main(int argc, char *argv[]) {
	GameLoop*		game_Loop =		 NULL;
	FILE           *log_file    = 	 NULL;		/*<! puntero al file, desactivado en test mode*/
	Bool            log_enabled = 	 FALSE;
	Bool			test_enabled = 	 FALSE;
	Bool			is_determinist = FALSE;
	int             result;
	int             i_flag;
	unsigned int	seed = 			 0;

	/*=============== COMPROBACION DE recursos Minimos (programa y .dat) =========================*/
	if (argc < 2) {
		fprintf(stderr, "Use: %s <game_data_file> [-l <log_file>]\n", argv[0]);
		return 1;
	}

	/*============= FLAGS -l -d -t =================================*/
	/*Aca se hará la pregunta de las flags -l , -d  o -t*/
	if(argc >= 3)
	{
		for (i_flag = 2; i_flag < argc; i_flag++)
		{
			/*Si existe -l debe de existir el namefile_log*/
			if((i_flag+1) < argc && argv[i_flag] && strcmp(FLAG_LOG, argv[i_flag]) == 0)
			{
				if(argv[i_flag+1] != NULL)
				{
					log_file = fopen(argv[++i_flag], "w");
					if(!log_file)
					{
						fprintf(stderr, "ERROR: Cann't open the log file\n");
						return 1;
					}
					log_enabled = TRUE;
				}
			}
			/*Si existe -d se hará determinista el juego*/
			if(((i_flag+1) < argc) && argv[i_flag] && strcmp(FLAG_DET, argv[i_flag]) == 0)
			{
				is_determinist = TRUE;
				seed = atoi(argv[++i_flag]);
			}
			/*Si existe -t se ejecutará el modo test*/
			if(argv[i_flag] && strcmp(FLAG_TEST, argv[i_flag]) == 0)
			{
				test_enabled = TRUE;
			}
		}
	}	
	
	/*Existe -d ---->  Activa el Modo Determinista*/
	if(is_determinist == TRUE)  {srand(seed);}
	else {srand(time(NULL));}


	/*==========================================================*/

	/*============== Inicialización del JUEGO (LLAMADA A GAME_MANAGMENT  y Graphic_engine ) =======================*/

	result = game_loop_init(&game, &gengine, argv[1], test_enabled);
	if (result != 0) 
	{
		fprintf(stderr, "Error inizialiting game (%d)\n", result);
		if (log_file) fclose(log_file); /*Si hemos abierto el archvio log, (flag -l), cierralo*/
		return 1;
	}
	/*==========================================================*/

	/*============== OBTENCIÓN DEL INPUT DEL USUARIO =======================*/

	/** 
	 * Mi propuesta consiste en bifurcar el tipo de input, gracia a una tercera flag[ -t ]
	 *  que active el modo test (por terminal .cmd) o en su defecto el modo deploy (por Keyboard)
	 * 
	 * Modo Visual:
	 *            IF    (no existe -t  )  THEN
	 *                command_raylib_user_input();
	 *
	*/
	last_cmd = game_get_last_command(game_loop->last);
	if (!last_cmd)
	{
		game_loop_cleanup(game, gengine, log_file);
		return 1;
	}
	/*==========================================================*/


	/* ── Abrir la ventana UNA SOLA VEZ ── */
	InitWindow(WIDHT_SCREEN, HIGHT_SCREEN, "Atlantic Quest");
	SetTargetFPS(60);

	/* ── Menú (dibuja dentro de la ventana ya abierta) ── */
	graphic_engine_menu_init(gengine, game);

	/* ── Bucle principal: UN SOLO while ── */
	while (!WindowShouldClose() && 
			command_get_code(last_cmd) != EXIT && 
			game_get_finished(game)    == FALSE) 
	{
		graphic_engine_paint_game(gengine, game);  /* 1. Render  */
		command_get_user_input(last_cmd);           /* 2. Input   */
		game_actions_update(game, last_cmd);     
		/* 4. LOG — if enabled, write the command and its result */
		if (log_enabled && log_file) game_loop_print_log(game, last_cmd, log_file);

			/* 5. TURN — advance to the next player (F11 multiplayer) */
			game_turn_update(game);
	}

	/* ── Cerrar la ventana UNA SOLA VEZ ── */
	CloseWindow();
	game_loop_cleanup(game, gengine, log_file);
	return 0;
}


/* ========================================================================= */
/*                            CREATE/ INIT / CLEANUP                                 */
/* ========================================================================= */
GameLoop* 	game_loop_create()
{
	GameLoop* new_game_Loop = (GameLoop*) malloc(sizeof(GameLoop));
	if(!new_game_Loop) return NULL;

	new_game_Loop->game = 		game_create();
	if(!new_game_Loop->game)
	{
		free(new_game_Loop);
		return NULL;
	}
	new_game_Loop->last_cmd = 	NULL;
	new_game_Loop->gp_raylib = 	NULL;

	return new_game_Loop;
}
int game_loop_init_test(GameLoop* game_loop, char *file_name)
{
	if(!game_loop) return 1; 														/*ERROR 1 : NO existe GameLoop con el que trabajar*/
	if (game_management_create_from_file(game_loop->game, file_name) == ERROR) return 2;		/*ERROR 2 : NO existe GameLoop con el que trabajar*/
}

int game_loop_init_user(GameLoop *game_loop, char *file_name)
{
	Graphic_engine *gengine     = 	 NULL;		/*<! puntero al Graphic_engine_raylib, desactivado en test mode*/
	Game           *game        = 	 NULL;
	Command        *last_cmd    = 	 NULL;

	

	if (game_management_create_from_file(game, file_name) == ERROR) return 1;

	if()
	*gengine = graphic_engine_create();
	if (*gengine == NULL) {
		game_destroy(*game);
		return 2;
	}

	return 0;
}

void game_loop_cleanup(GameLoop* gameloop, FILE *log_file) {
	if(gameloop){
		if(gameloop->game) 		game_destroy(gameloop->game);
		if(gameloop->gp_raylib) graphic_engine_destroy(gameloop->gp_raylib);
		free(gameloop);
	}
	if (log_file) 			fclose(log_file);
}


/* ======================================================================== */
/*                               Print LOG                                  */
/* ======================================================================== */

void game_loop_print_log(Game *game, Command* last_cmd, FILE* log_file){
	if(!game || !last_cmd ) return;
	if (log_file) {
			Status      status   = game_get_last_cmd_status(game);
			CommandCode cmd_code = command_get_code(last_cmd);
			Id			skill_id = command_get_skill(last_cmd); /*Falta implementar esta función para saber que skill ha usado el numen del player*/
			char       *target_name = command_get_target(last_cmd);
			const char *result_str = (status == OK) ? "OK" : "ERROR";

			target_name = target_name ? target_name : "UNKNOW";

			switch (cmd_code) {
				case EXIT: 		print(log_file, "Exit", "Generic",   NO_ID,    result_str);		break;
				case MOVE: 		print(log_file, "Move", target_name, skill_id, result_str);		break;
				case TAKE: 		print(log_file, "Take", target_name, skill_id, result_str);		break;
				case DROP: 		print(log_file, "Drop", target_name, skill_id, result_str);		break;
				case ATTACK: 	print(log_file, "Attack", target_name, skill_id, result_str);	break;
				case CHAT: 		print(log_file, "Chat", target_name, skill_id, result_str);		break;
				case INSPECT: 	print(log_file, "Inspect", target_name, skill_id, result_str);	break;
				case USE: 		print(log_file, "Use", 	target_name, skill_id, result_str);		break;
				case OPEN: 		print(log_file, "Open", target_name, skill_id, result_str);		break;	
				case SAVE: 		print(log_file, "Save", target_name, skill_id, result_str);		break;
				default:		print(log_file, "UNKNOW", "???", NO_ID, "ERROR");				break;
			}
	}
	return;
}