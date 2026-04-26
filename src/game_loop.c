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
#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h" /*Esto es una extension de raylib para hacer la interfaz grafica*/
#include "command.h"
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
} while(0);
#define print_error(mssg, md_test) do { \
    if(mssg) fprintf(stderr, "%s   Modo: %s\n", mssg, \
        (md_test == TRUE) ? "Test" : "User"); \
} while(0);

typedef enum{
	OK,
	EXIT,
	ERR_gloop,
	ERR_game,
	ERR_cmd,
	ERR_graph,
	ERR_unknow
}Status_init;

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
Status_init game_loop_init_user(GameLoop *game_loop);


/**
 * @brief Initializes the game and the graphic engine from a data file on mode Test to other devs
 * @author Rafael
 *
 * @param game pointer to pointer to the game (will be allocated)
 * @param gengine pointer to pointer to the graphic engine (will be allocated)
 * @param file_name path to the .dat file
 * @return 0 on success, 1 if game init fails, 2 if graphic engine fails
 */
Status_init game_loop_init_test(GameLoop* game_loop, char *file_name);

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
	GameLoop*		game_loop =		 		NULL;
	FILE           *log_file    = 	 		NULL;		/*<! puntero al file, desactivado en test mode*/
	Bool            log_enabled = 	 		FALSE;
	Bool			test_enabled = 	 		FALSE;
	Bool			is_determinist = 		FALSE;
	Status_init		result = 		 		ERR_unknow;
	MenuResult		menu_result;
	char			name_file_data[WORD_SIZE+1]= ""; 		/*<! Espacio para guardar la direccion del file con info de la partida*/
	int             i_flag = 		 		0;
	unsigned int	seed = 			 		0;

	/*=============== COMPROBACION DE recursos Minimos (programa y .dat) =========================*/
	/*Posible meora, podemos dejar como opcional el que s eingrese un .dat, al fin y al cabo, el jeugo se creara cuando se haya dado un menu que diga si leer el newgameo no*/
	if (argc < 1) {
		fprintf(stderr, "Use: %s  [-l <log_file>][-d <seed>][-t <game_data_file>]\n", argv[0]);
		return 1;
	}


	/*============= FLAGS -l -d -t =================================*/
	/*Aca se hará la pregunta de las flags -l , -d  o -t*/
	if(argc >= 2)
	{
		for (i_flag = 1; i_flag < argc; i_flag++)
		{
			/*Si existe -l debe de existir el namefile_log*/
			if((i_flag+1) < argc && argv[i_flag] && strcmp(FLAG_LOG, argv[i_flag]) == 0)
			{
				if(argv[i_flag+1] != NULL)
				{
					log_file = fopen(argv[++i_flag], "w");
					if(!log_file)
					{
						print_error("Error: Cann't open the file pass", test_enabled);
						return 1;
					}
					log_enabled = TRUE;
				}
			}
			/*Si existe -d debe existir el <seed> para hacer determinista el juego*/
			if(((i_flag+1) < argc) && argv[i_flag] && strcmp(FLAG_DET, argv[i_flag]) == 0)
			{
				is_determinist = TRUE;
				seed = (unsigned int) atoi(argv[++i_flag]);
			}
			/*Si existe -t  debe existir <game_data_file> para que se ejecute el modo test*/
			if(((i_flag+1) < argc) && argv[i_flag] && strcmp(FLAG_TEST, argv[i_flag]) == 0)
			{
				strncpy(name_file_data,argv[++i_flag],WORD_SIZE );
				name_file_data[WORD_SIZE] = '\0';
				test_enabled = TRUE;
			}
		}
	}	
	
	/*Existe -d ---->  Activa el Modo Determinista*/
	if(is_determinist == TRUE)  {srand(seed);}
	else {srand(time(NULL));}
	/*==========================================================*/

	/*============== Inicialización del JUEGO (LLAMADA A GAME_MANAGMENT  y Graphic_engine ) =======================*/
	
	game_loop = game_loop_create();
	if(!game_loop)
	{
		print_error("ERROR: Cann't create game_loop", test_enabled); 
		return 1;
	}

	if(test_enabled == TRUE)	{result = game_loop_init_test(game_loop, name_file_data);} /*Modo Test*/
	else						{result = game_loop_init_user(game_loop);}				   /*Modo User*/
	

	switch (result)
	{
		case ERR_unknow: print_error("ERROR: Unknow this error, please, contact to we for we can help you", test_enabled) game_loop_cleanup(game_loop,log_file); 	return 1;
		case ERR_game: 	 print_error("ERROR: Cann't init game", 				test_enabled); 	game_loop_cleanup(game_loop,log_file);	return 1;
		case ERR_file: 	 print_error("ERROR: Anysomething wrong whit file", 	test_enabled); 	game_loop_cleanup(game_loop,log_file);  return 1;
		case ERR_graph:  print_error("ERROR: Cann't init graph", 				test_enabled); 	game_loop_cleanup(game_loop,log_file);	return 1;
		default:																														break;
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
	/*==========================================================*/

	return 0;
}


/* ========================================================================= */
/*                            CREATE/ INIT / CLEANUP                                 */
/* ========================================================================= */
GameLoop* 	game_loop_create()
{
	GameLoop* new_game_Loop = (GameLoop*) malloc(sizeof(GameLoop));
	if(!new_game_Loop)			return NULL;

	new_game_Loop->game =		NULL;
	new_game_Loop->last_cmd = 	NULL;
	new_game_Loop->gp_raylib = 	NULL;

	return new_game_Loop;
}
Status_init game_loop_init_test(GameLoop* game_loop, char *file_name)/*Función para el modo test*/
{
	if(!game_loop) return ERR_gloop; 																/*ERROR 1 : NO existe GameLoop con el que trabajar*/
	if (game_management_create_from_file(game_loop->game, file_name) == ERROR) return ERR_game;		/*ERROR 2 : No se pudo guardar info del .dat en game*/
}

Status_init game_loop_init_user(GameLoop *game_loop)
{
	MenuResult	result_ge;
	Command		*command = NULL;


	if(!game_loop)	return ERR_gloop;


	game_loop->gp_raylib = graphic_engine_create();
	if (game_loop->gp_raylib == NULL)
	{
		command_destroy(command);
		return ERR_graph;
	}

	result_ge = graphic_engine_init(game_loop->gp_raylib);

	


	/*Si por lo que sea el usuario ha terminado la sesión en el menú, salimos y notificamos que se tiene que salir (EXIT)*/
	if(result_ge.menu_out == OUT_ERR)
	{
		command_destroy(command);
		graphic_engine_destroy(game_loop->gp_raylib);
		return EXIT;
	}
	if(game_management_create_from_file(&game_loop->game, result_ge.data_name) == ERROR)
	{
		command_destroy(command);
		graphic_engine_destroy(game_loop->gp_raylib);
		return ERR_game;
	}
	if(command_raylib_get_user_input(game_get_last_command(game_loop->game)) == ERROR)
	{
		command_destroy(command);
		graphic_engine_destroy(game_loop->gp_raylib);
		return ERR_game;
	}
	
	return OK;
}

void game_loop_cleanup(GameLoop* gameloop, FILE *log_file)
{
	if(gameloop)
	{
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

			switch (cmd_code)
			{
				case EXIT: 		print(log_file, "Exit", "Generic",   NO_ID,    result_str);		break;
				case MOVE: 		print(log_file, "Move", target_name, skill_id, result_str);		break;
				case WALK: 		print(log_file, "Walk", target_name, skill_id, result_str);		break;
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