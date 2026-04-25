/**
 * @brief It implements the skills struct
 *
 * @file skills.c
 * @author Rafael
 * @version 0
 * @date 20-04-2026
 * @copyright GNU Public License
 */

#include "skills.h"
#define NUM_SKILLS 4

/* NOTA:
    *JAVIER AGREGA MÁS SKILLS:  llamarada, chorro de agua, 
    *puño hielo, lo que sea, pero que hayan aunque sea 18 habilidades distitntas, 
    *4 habilidades por tipo de numen*
*/
static const Skills skill_table[NUM_SKILLS] = {
    /* [NO_SKILL]       */ { NO_SKILL,      "Ninguna",        0, 0,  0,  0 },
    /* [TAKLE]          */ { TAKLE,          "Tackle",         3, 1,  5,  2 },
    /* [QUICK_ATTACK]   */ { QUICK_ATTACK,   "Quick Attack",   2, 1,  3,  1 },
    /* [STEAM_ERUPTION] */ { STEAM_ERUPTION, "Steam Eruption", 5, 2, 10,  4 },
};



/*============== PUBLIC FUNCTION ======================*/
void skill_active(Numen * sender, Numen* receiver, Skills id_skill)
{
    if(!sender || !receiver || id_skill == NO_ID) return ERROR;

    switch (id_skill)
    {
    case TAKLE : skill_active_takle(sender, receiver, id_skill);
        /* code */
        break;
    
    default:
        break;
    }

}



skill_active_takle(Numen * sender, Numen* receiver, Skills id_skill)
{

}