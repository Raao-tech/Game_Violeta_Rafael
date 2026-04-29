/**
 * @brief It implements the skills struct
 *
 * @file skills.c
 * @author Rafael and Javier Jarque
 * @version 1.1
 * @date 20-04-2026
 * @copyright GNU Public License
*/

#include "skills.h"
#define UNKNOWN -1
/* NOTA:
    *JAVIER AGREGA MÁS SKILLS:  llamarada, chorro de agua, 
    *puño hielo, lo que sea, pero que hayan aunque sea 18 habilidades distitntas, 
    *4 habilidades por tipo de numen*
*/
static const Skills skill_table[NUM_SKILLS] = {
    /* [NO_SKILL]       */ { NO_SKILL,       "Ninguna",        0, 0, 0 },
    /* [TAKLE]          */ { TAKLE,          "Tackle",         3, 1, 2 },
    /* [QUICK_ATTACK]   */ { QUICK_ATTACK,   "Quick Attack",   2, 1, 1 },
    /* [STEAM_ERUPTION] */ { STEAM_ERUPTION, "Steam Eruption", 5, 2, 4 },
    /* [STAND_PLATINUM] */ { STAND_PLATINUM, "Stand Platinum", 4, 4, 5 },
    /* [THUNDER_SHOCK]  */ { THUNDER_SHOCK,  "Thunder Shock",  6, 2, 4 },
    /* [FIRE_BALL]      */ { FIRE_BALL,      "Fire Ball",      4, 3, 4 },
    /* [FIRE_BALL_A]    */ { FIRE_BALL_A,    "Fire Ball Area",  2, 1,0 }
};
    /*INSTRUCTIONS:          ID,              NAME,            radio, damage, coste*/



/*============== PUBLIC FUNCTION ======================*/
Status skill_active(Numen * sender, Numen* receiver, Skills_id id_skill, int distance, Space* space, Game* game)
{
    if(!sender || !receiver || id_skill == NO_SKILL || distance < 0) return ERROR;

    return skill_apply_effect (id_skill, sender, receiver, distance, space, game);
    /*
    Se hace con 2 funciones separadas por si en un futuro se desea añadir nuevas funciones, como efectos tipo relantizar o daño en area
    */

}



int skill_get_radio (Skills_id id)
{
    if (id < 0 || id > NUM_SKILLS) return UNKNOWN;
    return skill_table[id].radio;
}

int skill_get_damage(Skills_id id)
{
    if (id < 0 || id > NUM_SKILLS) return UNKNOWN;
    return skill_table[id].damage;
}

int skill_get_coste(Skills_id id)
{
    if (id < 0 || id > NUM_SKILLS) return UNKNOWN;
    return skill_table[id].coste;
}


char *skill_get_name(Skills_id id)
{
    if (id < 0 || id > NUM_SKILLS) return UNKNOWN;
    return skill_table[id].name;
}



Status skill_apply_effect (Skills_id id, Numen *sender, Numen *reciver, int distance, Space *space, Game *game)
{
    if (id < 0 || id > NUM_SKILLS || !sender || !reciver) return ERROR;
    int energy = numen_get_energy(sender);
    int coste = skill_get_coste(id);

    if (energy < coste) return ERROR;
    energy = energy - coste;
    numen_set_energy (sender, energy);
    switch (id)
    {
		case TAKLE:			skill_active_takle (sender, reciver, id, distance);	break;
		case QUICK_ATTACK:	skill_active_quick_attack(sender, reciver, id, distance);	break;
		case STEAM_ERUPTION:
			skill_active_steam_eruptiom(sender, reciver, id, distance);
			break;
		case STAND_PLATINUM:
			skill_active_stand_platinum(sender, reciver, id, distance);
			break;
		case THUNDER_SHOCK:
			skill_active_thunder_shock(sender, reciver, id, distance);
			break;
        case FIRE_BALL:
            skill_active_fire_ball(sender, reciver, id, distance, space, game);
            break;
        case FIRE_BALL_A:
            skill_active_fire_ball_a (sender, reciver, id, distance);
		default:
			break;
    }
    return OK;
}


Status skill_active_takle(Numen * sender, Numen* receiver, Skills_id id_skill, int distance)
{
    int radio = skill_get_radio(id_skill);
    int damage = skill_get_damage(id_skill);
    int health = 0;
    
    if (distance < radio) return ERROR;
    health = health - damage;
    numen_set_health (receiver, health);

    return OK;
}

Status skill_active_quick_attack(Numen * sender, Numen* receiver, Skills_id id_skill, int distance)
{
    int radio = skill_get_radio(id_skill);
    int damage = skill_get_damage(id_skill);
    int health = 0;
    
    if (distance < radio) return ERROR;
    health = health - damage;
    numen_set_health (receiver, health);

    return OK;
}

Status skill_active_steam_eruptiom(Numen * sender, Numen* receiver, Skills_id id_skill, int distance)
{
    int radio = skill_get_radio(id_skill);
    int damage = skill_get_damage(id_skill);
    int health = 0;
    
    if (distance < radio) return ERROR;
    health = health - damage;
    numen_set_health (receiver, health);

    return OK;
}

Status skill_active_stand_platinum(Numen * sender, Numen* receiver, Skills_id id_skill, int distance)
{
    int radio = skill_get_radio(id_skill);
    int damage = skill_get_damage(id_skill);
    int health = 0;
    
    if (distance < radio) return ERROR;
    health = health - damage;
    numen_set_health (receiver, health);

    return OK;
}

Status skill_active_thunder_shock(Numen * sender, Numen* receiver, Skills_id id_skill, int distance)
{
    int radio = skill_get_radio(id_skill);
    int damage = skill_get_damage(id_skill);
    int health = 0;
    
    if (distance < radio) return ERROR;
    health = health - damage;
    numen_set_health (receiver, health);

    return OK;
}

Status skill_active_fire_ball(Numen * sender, Numen* receiver, Skills_id id_skill, int distance, Space *space, Game *game)
{
    int radio = skill_get_radio(id_skill);
    int damage = skill_get_damage(id_skill);
    int health = 0;
    int active_pos_x = 0, active_pos_y = 0, enemy_pos_x = 0, enemy_pos_y = 0;
    Set *space_numens = NULL;
    Id num_id;



    if (distance < radio) return ERROR;
    health = health - damage;
    numen_set_health (receiver, health);

    sender = receiver;
    id_skill = FIRE_BALL_A;

    int num_enemies = space_get_n_numens(space);
    active_pos_x = numen_get_pos_x(sender); 
    active_pos_y = numen_get_pos_y(sender);
    space_numens = space_get_numens(space); 
    num_id = numen_get_id(sender);

  for (int i = 0; i < num_enemies; i++)
  {
    receiver = game_get_numen_by_id(game, set_get_id_at(space_numens, i)); 
    if (receiver && receiver && numen_get_id(receiver) != num_id)
    {
      enemy_pos_x = numen_get_pos_x(receiver);
      enemy_pos_y = numen_get_pos_y(receiver);
      distance = sqrt(pow(active_pos_x - enemy_pos_x, 2) + pow(active_pos_y - enemy_pos_y, 2));
      if (distance <= radio)
      {
        if(skill_active(sender, receiver, id_skill, distance, space_numens, game) == ERROR)
        {
          game_set_last_cmd_status(game, ERROR_Attack);
          return;
        }
      }
    }
  }
    return OK;
}

Status skill_active_fire_ball_a(Numen * sender, Numen* receiver, Skills_id id_skill, int distance)
{
    int radio = skill_get_radio(id_skill);
    int damage = skill_get_damage(id_skill);
    int health = 0;
    
    if (distance < radio) return ERROR;
    health = health - damage;
    numen_set_health (receiver, health);

    return OK;
}