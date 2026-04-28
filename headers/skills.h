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

#include "numen.h"
#include "types.h"

/*Hay que definir más ataques, aunque sea 4 distintos ataques por 5 numens, osea 20 distintos*/
typedef enum
{
    NO_SKILL,
    TAKLE,
    QUICK_ATTACK,
    STEAM_ERUPTION,
    STAND_PLATINUM,
    THUNDER_SHOCK
} Skills_id;

struct _Skills
{
    Skills_id id;
    char* name;
    int radio;
    int damage;
    int coste;
};

typedef struct _Skills Skills;

/*
 *    command
 *    attack name_numen 2 1
 */

/**
 * @brief It active the skill
 * @author Javier Jarque and Rafael
 *
 * @param sender a pointer with the sender of the skill
 * @param reciver a pointer with the reciver of the skill
 * @param id_skill the id of the skill
 * @param distance the distance of the skill
 */

Status skill_active (Numen* sender, Numen* receiver, Skills_id id_skill, int distance);
/**
 * @brief It gets the name of the skill
 * @author Javier Jarque
 *
 * @param id the id of the skill
 * @return the name of the skill
 */
char* skill_get_name (Skills_id id);

#endif
