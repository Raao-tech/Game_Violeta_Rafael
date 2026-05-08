/**
 * @brief It defines common types for the whole project
 *
 * @file types.h
 * @author Rafael
 * @version 0.0
 * @date 7-5-2026
 * @copyright GNU Public License
 */


#include "types.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>


Direction
types_parse_direction (const char* str)
{
	if (!str) return U;
	if (strcasecmp (str, "north") == 0 || strcasecmp (str, "n") == 0) return N;
	if (strcasecmp (str, "south") == 0 || strcasecmp (str, "s") == 0) return S;
	if (strcasecmp (str, "east") == 0 || strcasecmp (str, "e") == 0) return E;
	if (strcasecmp (str, "west") == 0 || strcasecmp (str, "w") == 0) return W;
	return U;
}

Bool    types_is_watching_it 
(
	void*  watchful, void*  target,
	Position (*get_vision) (void*), 
	Position (*get_position) (void*)
)
{
	Position    watchful_vision;
	Position    target_position;
	if (!watchful || !target || !get_position) return FALSE;
	
	/*Valores en Píxeles*/
	watchful_vision = get_vision (watchful);
	target_position = get_position (target);

	/*Si son validas las coordenadas, en pixeles*/
	if(types_position_is_valid (watchful, get_position,0, 0, WIDHT_MAP, HIGHT_MAP) == FALSE) 	return FALSE; 
	if(types_position_is_valid (target, get_vision,0, 0, WIDHT_MAP, HIGHT_MAP) == FALSE) 		return FALSE; 

	/*Si son iguales*/
	if (watchful_vision.pos_x == target_position.pos_x && 
		watchful_vision.pos_y == target_position.pos_y ) return TRUE;
	return FALSE;
}

Bool    types_position_is_valid
(
	void*  generic,
	Position (*get_position) (void*),
	int min_x, int min_y,
	int max_x, int max_y
)
{
	Position    position;
	if (!generic || !get_position) return FALSE;
	
	/*Valores en Píxeles*/
	position = get_position( generic );

	if ( 
			position.pos_x >= min_x && position.pos_y >= min_y &&
			position.pos_y <  max_x && position.pos_y < max_y
		) return TRUE;

	return FALSE;
}
