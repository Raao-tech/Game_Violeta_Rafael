/**
 * @brief It implements the game rules
 *
 * @file game_rules.c
 * @author Salvador
 * @version 1.5
 * @date 01-05-2026
 * @copyright GNU Public License
 */

#include "game_rules.h"
#include "skills.h"
#include "game_actions.h"
#include "raymath.h"

Status
game_rule_attack_enemy (Game* game, Id id_enemy)
{
	Player* player = NULL;
	Space* space   = NULL;
	Numen *num = NULL, *enemy_numen = NULL;
	Skills_id skill     = NO_SKILL;
	Id space_id, num_id;
	int distance, radio, skill_indx = 0, active_pos_x, active_pos_y, enemy_pos_x, enemy_pos_y, i;

	if (!game || id_enemy == NO_ID) return ERROR_enemy_attack;

	player = game_get_player_at (game, PLAYER);
	if (!player) return ERROR_enemy_attack;

	num_id = player_get_active_numen (player);
	if (num_id == NO_ID) return ERROR_enemy_attack;

	space_id = player_get_zone (player);
	if (space_id == NO_ID) return ERROR_enemy_attack;

	space = game_get_space (game, space_id);
	if (!space) return ERROR_enemy_attack;

	if (space_contains_numen (space, id_enemy) == FALSE) return ERROR_enemy_attack;

	enemy_numen = game_get_numen_by_id (game, id_enemy);
	if (!enemy_numen || numen_get_health (enemy_numen) <= 0) return ERROR_enemy_attack;

	/* El enemigo elige una skill aleatoria entre LAS SUYAS */
	skill_indx = rand () % NUM_SKILLS;
	for (i = 0; i < NUM_SKILLS; i++)
	{
		skill = numen_get_skill_by_index (enemy_numen, skill_indx);
		if (skill != NO_SKILL) break;             /* la encontre, salgo */
		skill_indx = (skill_indx + 1) % NUM_SKILLS;
	}
	if (skill == NO_SKILL) return ERROR_enemy_attack;

	num = game_get_numen_by_id (game, num_id);    /* el del jugador, va a recibir */
	if (!num) return ERROR_enemy_attack;
	if (skill == NO_SKILL) return ERROR_enemy_attack; /* Enemy has no valid skills to attack */

	num = game_get_numen_by_id (game, num_id);
	if (!space || !num) return ERROR_enemy_attack;

	active_pos_x = numen_get_pos_x (num);
	active_pos_y = numen_get_pos_y (num);

	enemy_pos_x  = numen_get_pos_x (enemy_numen);
	enemy_pos_y  = numen_get_pos_y (enemy_numen);

	if (numen_get_health (num) <= 0 && numen_get_health (enemy_numen) <= 0) return ERROR_enemy_attack;

	radio    = skill_get_radio (skill); /*por implementar*/

	distance = sqrt (pow (active_pos_x - enemy_pos_x, 2) + pow (active_pos_y - enemy_pos_y, 2));
	if(radio>=distance)
	{
	if (skill_active (enemy_numen, num, skill, distance) == ERROR) /*por implementar status skill_apply_effect*/
		return ERROR_enemy_attack;
	}
	return OK;
}

/*Dete*/
Bool
_game_rule_numen_in_area (Numen* numen_center, Numen* numen_goal, int scale)
{
    Position pos_center, pos_goal;

    if (!numen_center || !numen_goal || scale <= 0) return FALSE;

    pos_center = numen_get_position (numen_center);
    pos_goal   = numen_get_position (numen_goal);

    if (pos_center.pos_x == NO_POS || pos_center.pos_y == NO_POS) return FALSE;
    if (pos_goal.pos_x   == NO_POS || pos_goal.pos_y   == NO_POS) return FALSE;

    if (pos_goal.pos_x <= (pos_center.pos_x + scale) &&
        pos_goal.pos_x >= (pos_center.pos_x - scale) &&
        pos_goal.pos_y <= (pos_center.pos_y + scale) &&
        pos_goal.pos_y >= (pos_center.pos_y - scale))
        return TRUE;

    return FALSE;
}

Direction
_game_rule_enemy_dir (Numen* numen_center, Numen* numen_goal)
{
    Position pos_center, pos_goal;
    int dx, dy;

    if (!numen_center || !numen_goal) return U;

    pos_center = numen_get_position (numen_center);
    pos_goal   = numen_get_position (numen_goal);

    if (pos_center.pos_x == NO_POS || pos_goal.pos_x == NO_POS) return U;

    dx = pos_goal.pos_x - pos_center.pos_x;   /* >0 goal a la derecha */
    dy = pos_goal.pos_y - pos_center.pos_y;   /* >0 goal abajo        */

    /* Nos movemos en el eje con mayor diferencia absoluta. */
    if (abs (dx) >= abs (dy))
    {
        if (dx > 0) return E;
        if (dx < 0) return W;
    }
    else
    {
        if (dy > 0) return S;
        if (dy < 0) return N;
    }
    return U; 
}

Status
game_rule_walk_enemy (Game* game)
{
    Player*  player;
    Numen*   active_numen = NULL;
    Numen*   walker;
    Space*   space;
    Set*     set_ids;
    Id       active_id, walker_id;
    Direction dir;
    Position pos_now, pos_orig;
    int      *grid_line;
    int      n_numens, i, cell_x, cell_y;
    int      hp_max, hp_now;
    Bool     hides;        /* TRUE si el numen quiere huir */
    Bool     sees_active;

    if (!game) return ERROR;

    player = game_get_player_at (game, PLAYER);
    if (!player) return ERROR;

    /* El numen activo es el "objetivo" del que perseguir o huir.
     * Si no hay activo, los enemigos solo patrullan. */
    active_id = player_get_active_numen (player);
    if (active_id != NO_ID)
        active_numen = game_get_numen_by_id (game, active_id);

    space = game_get_space (game, player_get_zone (player));
    if (!space) return ERROR;

    set_ids  = space_get_numens (space);
    n_numens = space_get_n_numens (space);
    if (n_numens == 0 || !set_ids) return OK;

    for (i = 0; i < n_numens; i++)
    {
        walker_id = set_get_id_at (set_ids, i);
        if (walker_id == NO_ID || walker_id == active_id) continue;

        walker = game_get_numen_by_id (game, walker_id);
        if (!walker) continue;
        if (numen_get_health (walker) <= 0) continue;

        pos_now = numen_get_position (walker);
        if (pos_now.pos_x == NO_POS || pos_now.pos_y == NO_POS) continue;
        pos_orig = numen_get_position (walker);
        /* === DECIDIR DIRECCION === */
        sees_active = (active_numen != NULL) &&
                      (_game_rule_numen_in_area (walker, active_numen, SCALE * 4) == TRUE);

        hp_now = numen_get_health (walker);
        hp_max = (numen_get_corrupt (walker) == TRUE) ? MAX_LIFE_CORRUPT : MAX_LIFE;
        hides  = (numen_get_corrupt (walker) == TRUE) &&
                 (hp_now > 0) &&
                 (hp_now <= hp_max * 30 / 100);

        if (hides == TRUE && active_numen != NULL)
        {
            /* HUIR: dirección opuesta a perseguir */
            dir = _game_rule_enemy_dir (walker, active_numen);
            switch (dir)
            {
                case N: dir = S; break;
                case S: dir = N; break;
                case E: dir = W; break;
                case W: dir = E; break;
                default: dir = U; break;
            }
        }
        else if (sees_active == TRUE)
        {
            /* PERSEGUIR */
            dir = _game_rule_enemy_dir (walker, active_numen);
        }
        else
        {
            /* PATRULLAR: aleatorio.  rand()%4 → 0..3 → N/S/E/W */
            switch (rand () % 4)
            {
                case 0:  dir = N; break;
                case 1:  dir = S; break;
                case 2:  dir = E; break;
                default: dir = W; break;
            }
        }

        /* === CALCULAR CELDA DESTINO === */
        switch (dir)
        {
            case N: pos_now.pos_y -= SCALE; break;
            case S: pos_now.pos_y += SCALE; break;
            case E: pos_now.pos_x += SCALE; break;
            case W: pos_now.pos_x -= SCALE; break;
            default: continue;   /* sin dirección, no se mueve */
        }

        cell_x = pos_now.pos_x / SCALE;
        cell_y = pos_now.pos_y / SCALE;

        /* Fuera del grid: no se sale del space */
        if (cell_x < 0 || cell_x >= WIDHT ||
            cell_y < 0 || cell_y >= HIGHT) continue;

        /* Pared: skip turno */
        grid_line = space_get_grid_by_line (space, cell_y);
        if (!grid_line || grid_line[cell_x] == 0 || grid_line[cell_x] == 2) continue;


        if(sees_active==TRUE) game_rule_attack_enemy(game, walker_id);
        

        /* MOVER */
        numen_set_pos_x (walker, pos_now.pos_x);
        numen_set_pos_y (walker, pos_now.pos_y);

        space_set_grid_by_position(space, pos_orig, 1);
        space_set_grid_by_position(space, pos_now, 2);
    }

    return OK;
}


Status
game_rule_walk_active (Game* game)
{
	Player* player      = NULL;
	Numen* active_num   = NULL;
	Space* space        = NULL;
	int*    grid[HIGHT];
	Direction direction = U;
	Position pos_current, pos_player, pos_orig;
	Id id_active     = NO_ID;
	int  i, radio = SCALE , distance, speed;

	for (i = 0; i < HIGHT; i++) grid[i] = NULL;

	if (!game) { return ERROR; }
	player = game_get_player_at (game, PLAYER);
	if (!player) { return ERROR; }
	space = game_get_space (game, player_get_zone (player));
	if (!space) { return ERROR; }
	for (i = 0; i < HIGHT; i++) { grid[i] = space_get_grid_by_line (game_get_space (game, player_get_zone (player)), i); }

	id_active = player_get_active_numen (player);
	if (id_active == NO_ID) { return ERROR; /* Skip if the ID is NO_ID*/ }
	active_num = game_get_numen_by_id (game, id_active);

	if (!active_num || numen_get_health (active_num) <= 0)
		{
			/*¿debería cambiar al active numen en este caso?*/
			return ERROR;
		}

    pos_orig.pos_x=numen_get_pos_x(active_num);
    pos_orig.pos_y=numen_get_pos_y(active_num);

	pos_current.pos_x = numen_get_pos_x (active_num);
	pos_current.pos_y = numen_get_pos_y (active_num);

	pos_player        = player_get_position (player);
	speed             = numen_get_speed (active_num);
	
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Dividiendo distance entre SCALE, se activa el movimiento aleatorio del numen. Este es un tipo de movimiento que actualmente está defectuoso, no usar.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	distance          = sqrt (pow (pos_current.pos_x - pos_player.pos_x, 2) + pow (pos_current.pos_y - pos_player.pos_y, 2));///SCALE

	if (distance > radio) /*The active numen is too far from player, it tries to get back to the area*/
		{
			if (pos_current.pos_x < pos_player.pos_x) { direction = W; }
			else if (pos_current.pos_x > pos_player.pos_x) { direction = E; }
			else if (pos_current.pos_y < pos_player.pos_y) { direction = S; }
			else if (pos_current.pos_y > pos_player.pos_y) { direction = N; }

			/////////////////////////////////////////////////////////
			switch (direction)
				{
					case N: pos_current.pos_y -= SCALE*speed; break;
					case S: pos_current.pos_y += SCALE*speed; break;
					case W: pos_current.pos_x -= SCALE*speed; break;
					case E: pos_current.pos_x += SCALE*speed; break;
					default: break;
				}
            
			/////////////////////////////////////////////////////////
            if(pos_current.pos_x>=0 && pos_current.pos_x<=WIDHT_MAP)
            {
			if (grid[pos_current.pos_y/SCALE][pos_current.pos_x/SCALE] != 0 && grid[pos_current.pos_y/SCALE][pos_current.pos_x/SCALE] != 2)
				{
					if (numen_set_pos_x (active_num, pos_current.pos_x) == ERROR || numen_set_pos_y (active_num, pos_current.pos_y) == ERROR)
						{
							return ERROR;
						}
                    space_set_grid_by_position(space, pos_orig, 1);
                    space_set_grid_by_position(space, pos_current, 2);
					return OK;
				}
            }
			switch (direction) /*checks whether the direction that failed is on the x coordinate and restores it. If it isn't, then it must've been on
														the y coordinate and there's nothing left to check*/
				{
					case W: pos_current.pos_x += SCALE*speed; break;
					case E: pos_current.pos_x -= SCALE*speed; break;
					default: return ERROR;
				}
			/*If the error was on the x coordinate, try the y coordinate*/

			if (pos_current.pos_y < pos_player.pos_y) { direction = S; }
			else if (pos_current.pos_y > pos_player.pos_y) { direction = N; }
            
			switch (direction) /*Falta definir cuánto se mueve*/
				{
					case N: pos_current.pos_y -= SCALE*speed; break;
					case S: pos_current.pos_y += SCALE*speed; break;

					default: break;
				}
            
            if(pos_current.pos_y>=0 && pos_current.pos_y<=HIGHT_MAP)
            {
			if (grid[pos_current.pos_y/SCALE][pos_current.pos_x/SCALE] != 0 && grid[pos_current.pos_y/SCALE][pos_current.pos_x/SCALE] != 2)
				{
					if (numen_set_pos_x (active_num, pos_current.pos_x) == ERROR || numen_set_pos_y (active_num, pos_current.pos_y) == ERROR)
						{
							return ERROR;
						}
                    space_set_grid_by_position(space, pos_orig, 1);
                    space_set_grid_by_position(space, pos_current, 2);
					return OK;
				}
			return ERROR; /*Unable to move the active numen any closer to the player*/
            }
		}
	else /*The numen is close enough to the player, for now, it moves at any random position inside the area or 1 square out of it, it decides it
			randomly*/
		{
			direction = (Direction)rand () % 4; /*The 4 possible directions besides unknown*/

			switch (direction) /*Falta definir cuánto se mueve*/
				{
					case N: pos_current.pos_y -= SCALE*speed; break;
					case S: pos_current.pos_y += SCALE*speed; break;
					case W: pos_current.pos_x -= SCALE*speed; break;
					case E: pos_current.pos_x += SCALE*speed; break;
					default: break;
				}

			if (pos_current.pos_x<0 || pos_current.pos_x>=WIDHT_MAP||pos_current.pos_y<0 || pos_current.pos_y>=HIGHT_MAP||grid[pos_current.pos_y/SCALE][pos_current.pos_x/SCALE] == 0 || grid[pos_current.pos_y/SCALE][pos_current.pos_x/SCALE] == 2)
				{
					direction = (direction + 1) % 4;
					for (i = 0; (pos_current.pos_x<0|| pos_current.pos_x>=WIDHT_MAP||pos_current.pos_y<0 || pos_current.pos_y>=HIGHT_MAP||grid[pos_current.pos_y/SCALE][pos_current.pos_x/SCALE] == 0) && i < 3; direction = (direction + 1) % 4, i++)
						{

							switch ((direction + 3) % 4) /*restores the previous coordinates*/
								{
									case N: pos_current.pos_y += SCALE*speed; break;
									case S: pos_current.pos_y -= SCALE*speed; break;
									case W: pos_current.pos_x += SCALE*speed; break;
									case E: pos_current.pos_x -= SCALE*speed; break;
									default: break;
								}
							switch (direction) /*gets the new coordinates to move to*/
								{
									case N: pos_current.pos_y -= SCALE*speed; break;
									case S: pos_current.pos_y += SCALE*speed; break;
									case W: pos_current.pos_x -= SCALE*speed; break;
									case E: pos_current.pos_x += SCALE*speed; break;
									default: break;
								}
						}
				}
			if (grid[pos_current.pos_y/SCALE][pos_current.pos_x/SCALE] == 0||grid[pos_current.pos_y/SCALE][pos_current.pos_x/SCALE] == 2) return ERROR; /*The active Numen can't move*/
			if (numen_set_pos_x (active_num, pos_current.pos_x) == ERROR || numen_set_pos_y (active_num, pos_current.pos_y) == ERROR)
				{
					return ERROR; /*Unable to move the active numen*/
				}
                 space_set_grid_by_position(space, pos_orig, 1);
                 space_set_grid_by_position(space, pos_current, 2);
			return OK;
		}
}
Status
game_rule_move (Game* game)
{   
    Numen*    numen;
	Player*   player;
	Space*    dest_sp;
	char*     dir_str;
	Direction dir;
	Id        origin, dest, active_id;
	int       pos_x, pos_y;

	if (!game) return ERROR;
	player = game_get_player_at (game, PLAYER);
	if (!player) return ERROR;

    active_id=player_get_active_numen(player);
    if(active_id!=NO_ID)
    numen=game_get_numen_by_id(game, active_id);

	origin = player_get_zone (player);
	if (origin == NO_ID) return ERROR;

	dir_str = command_get_target (game_get_last_command (game));
	if (!dir_str) return ERROR;

	dir = ge_parse_direction (dir_str);
	if (dir == U) return ERROR;

	dest = game_get_connection (game, origin, dir);
	if (dest == NO_ID) return ERROR;

	if (game_connection_is_open (game, origin, dir) == FALSE) return ERROR;

	pos_x = player_get_pos_x (player);
	pos_y = player_get_pos_y (player);

	/* Reposicionamos en el borde OPUESTO al de salida.
	 * Si sales por el norte, entras por el sur del nuevo space. */
	switch (dir)
	{
		case N: 
        player_set_position (player, pos_x, (HIGHT - 1) * SCALE); 
        if(numen)
        {
        if(pos_x+SCALE<WIDHT_MAP)
        numen_set_position(numen, pos_x+SCALE, (HIGHT - 1) * SCALE);

        else
         numen_set_position(numen, pos_x-SCALE, (HIGHT - 1) * SCALE);
        }
        break;

		case S: 
        player_set_position (player, pos_x, 0); 
          if(numen)
        {
        if(pos_x+SCALE<WIDHT_MAP)
        numen_set_position(numen, pos_x+SCALE, 0);

        else
         numen_set_position(numen, pos_x-SCALE, 0);
        }                  
         break;

		case E: 
        player_set_position (player, 0, pos_y);
          if(numen)
        {
        if(pos_y+SCALE<WIDHT_MAP)
        numen_set_position(numen, 0, pos_y+SCALE);

        else
         numen_set_position(numen, 0, pos_y-SCALE);
        }         
        break;

		case W: player_set_position (player, (WIDHT - 1) * SCALE, pos_y);
            if(numen)
        {
        if(pos_y+SCALE<WIDHT_MAP)
        numen_set_position(numen, (WIDHT - 1) * SCALE, pos_y+SCALE);

        else
         numen_set_position(numen, (WIDHT - 1) * SCALE, pos_y-SCALE);
        }    
        break;

		default: 
        return ERROR;
	}

	player_set_zone (player, dest);

	dest_sp = game_get_space (game, dest);
	if (dest_sp) space_set_discovered (dest_sp, TRUE);

	return OK;
}