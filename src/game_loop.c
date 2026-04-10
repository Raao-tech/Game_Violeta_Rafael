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
	FILE*       		file = 		NULL;
	Game*				game = 		NULL;
	Graphic_engine*	gengine = 	NULL;
	Command*			last_cmd = 	NULL;
	char* 			t_file = 	NULL;
	int 				result;

	if (argc < 2){
	fprintf(stderr, "Use: %s <game_data_file>\n", argv[0]);
	return 1;
	}
	//si hay argumentos de entrada abr
	if (argv != NULL){
	strcpy( t_file, argv[2]);
	if (argv[3] != NULL){
		char* n_file = NULL;
		strcpy(n_file, argv[3]);
		file = fopen(n_file, "w");
	}
	}


	/* Seed random for attack */
	srand(time(NULL));


	result = game_loop_init(&game, &gengine, argv[1]);

	switch (result)
	{
		case 0 :	break; //succesfully 
		case 1 : 	fprintf(stderr, "Error while initializing game.\n");			return 1;
		case 2 : 	fprintf(stderr, "Error while initializing graphic engine.\n");	return 1;
		default:	fprintf(stderr, "Error Unknow initialization loop.\n");			break;
	}	


	last_cmd = game_get_last_command(game);
	if(!last_cmd){
		
	}

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
				CommandCode comand_c = command_get_code(last_cmd);

				switch (comand_c)
				{
					case EXIT : 	fprintf(file, "exit: %s\n", (status == OK) ? "OK" : "ERROR" ); 		break;
					case MOVE : 	fprintf(file, "move: %s\n", (status == OK) ? "OK" : "ERROR" ); 		break;
					case TAKE : 	fprintf(file, "take: %s\n", (status == OK) ? "OK" : "ERROR" ); 		break;
					case DROP : 	fprintf(file, "drop: %s\n", (status == OK) ? "OK" : "ERROR" ); 		break;
					case ATTACK : 	fprintf(file, "attack: %s\n", (status == OK) ? "OK" : "ERROR" ); 	break;
					case CHAT : 	fprintf(file, "chat: %s\n", (status == OK) ? "OK" : "ERROR" ); 		break;
					case INSPECT : 	fprintf(file, "inspect: %s\n", (status == OK) ? "OK" : "ERROR" ); 	break;
				
					default: fprintf(file,"command: UNKNOW.  Sorry, could you repeat that?\n");			break;
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
	if (game_create_from_file(game, file_name) == ERROR)				   return 1;
	if ((*gengine = graphic_engine_create()) == NULL){game_destroy(*game); return 2; }

	return 0;
}

void game_loop_cleanup(Game *game, Graphic_engine *gengine, FILE *file)
{
game_destroy(game);
graphic_engine_destroy(gengine);
fclose(file);
}
