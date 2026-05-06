/**
 * @brief It defines the skills struct
 *
 * @file skills.h
 * @author Javier Jarque and Rafael
 * @version 1.2
 * @date 20-04-2026
 * @copyright GNU Public License
 */

#ifndef SKILLS_H
#define SKILLS_H

#include "types.h"

/* lugar de #include "numen.h":
 * skills.h NO necesita ver la struct interna de Numen, solo
 * recibirlo como puntero opaco.*/
typedef struct _Numen Numen;

typedef enum
{
    NO_SKILL = 0,
    TAKLE,
    QUICK_ATTACK,
    STEAM_ERUPTION,
    STAND_PLATINUM,
    THUNDER_SHOCK,
    FIRE_BALL,        /* Ataque base con efecto de propagacion en area */
    FIRE_BALL_A,      /* Variante, sería como una salpicadura del fire ball principal*/
    NUM_SKILLS        
} Skills_id;

typedef struct _Skills
{
    Skills_id id;
    char*     name;
    int       radio;
    int       damage;
    int       coste;
} Skills;

/* ====================================================================== */
/*                           PUBLIC FUNTIONS                              */
/* ====================================================================== */

/**
 * @brief Aplica una skill de sender a receiver si esta en rango y hay energia
 * @author Javier Jarque y Rafael
 *
 * @param sender    quien usa la skill (paga el coste de energia)
 * @param receiver  quien la recibe (pierde HP si esta en rango)
 * @param id_skill  identificador de la skill
 * @param distance  distancia en celdas entre sender y receiver
 * @return OK si se aplico, ERROR si fuera de rango / sin energia / NULL
 */
Status skill_active (Numen* sender, Numen* receiver, Skills_id id_skill, int distance);

/* Lpropiedades en la tabla — devuelven -1 / NULL en error */
char* skill_get_name   (Skills_id id);
int   skill_get_radio  (Skills_id id);
int   skill_get_damage (Skills_id id);
int   skill_get_coste  (Skills_id id);

#endif