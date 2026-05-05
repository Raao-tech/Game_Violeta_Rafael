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
#include "numen.h"
#include <stdio.h>

/* ====================================================================== */
/*                            TABLA DE SKILLS                              */
/*                                                                         */
/*  El indice del array DEBE coincidir con el valor del enum Skills_id     */
/*  (por eso NO_SKILL es 0). Si añades una skill nueva: enum primero,      */
/*  fila aqui despues, y el conteo NUM_SKILLS se actualiza solo.           */
/*                                                                         */
/*  Columnas:                  id              name             r  d  c    */
/* ====================================================================== */
static const Skills skill_table[NUM_SKILLS] = {
    [NO_SKILL]       = { NO_SKILL,       "Ninguna",        0, 0, 0 },
    [TAKLE]          = { TAKLE,          "Tackle",         3, 1, 2 },
    [QUICK_ATTACK]   = { QUICK_ATTACK,   "Quick Attack",   2, 1, 1 },
    [STEAM_ERUPTION] = { STEAM_ERUPTION, "Steam Eruption", 5, 2, 4 },
    [STAND_PLATINUM] = { STAND_PLATINUM, "Stand Platinum", 4, 4, 5 },
    [THUNDER_SHOCK]  = { THUNDER_SHOCK,  "Thunder Shock",  6, 2, 4 },
    [FIRE_BALL]      = { FIRE_BALL,      "Fire Ball",      4, 3, 4 },
    [FIRE_BALL_A]    = { FIRE_BALL_A,    "Fire Ball Area", 2, 1, 0 }
};

/* ====================================================================== */
/*                          PRIVATE HELPERS                                */
/* ====================================================================== */

static Bool
skill_id_is_valid (Skills_id id)
{
    return (id >= NO_SKILL && id < NUM_SKILLS) ? TRUE : FALSE;
}

/**
 * @brief Aplica daño plano del receptor en funcion del damage de la skill.
 *
 * Es la logica que repetian las 6 skill_active_takle/quick_attack/etc.
 * Tener una sola funcion evita que un bugfix futuro se aplique a 5
 * sitios y se olvide en el sexto.
 *
 * @return OK si golpea, ERROR si esta fuera de rango.
 */
static Status
apply_basic_damage (Numen* receiver, Skills_id id, int distance)
{
    int radio, damage, current_hp;

    if (!receiver || !skill_id_is_valid (id)) return ERROR;

    radio  = skill_table[id].radio;
    damage = skill_table[id].damage;

    /* Si el target esta MAS LEJOS que el radio, no le llegamos. */
    if (distance > radio) return ERROR;

    current_hp = numen_get_health (receiver);
    /* numen_set_health pasa por entity_set_health, que recorta
     * automaticamente a MIN_LIFE.*/
    return numen_set_health (receiver, current_hp - damage);
}

/* ====================================================================== */
/*                            PUBLIC FUNCTIONS                            */
/* ====================================================================== */

Status
skill_active (Numen* sender, Numen* receiver, Skills_id id_skill, int distance)
{
    int energy, coste;

    if (!sender || !receiver || !skill_id_is_valid (id_skill) || distance < 0)
        return ERROR;
    if (id_skill == NO_SKILL) return ERROR;

    /* 1. Comprobamos que el sender tiene energia para pagar el coste */
    energy = numen_get_energy (sender);
    coste  = skill_table[id_skill].coste;
    if (energy < coste) return ERROR;

    /* 2. Cobramos el coste antes de aplicar el efecto, para que un
     *    efecto que falla no devuelva la energia  GRATIS al sender. */
    if (numen_set_energy (sender, energy - coste) == ERROR) return ERROR;

    /* 3. Aplicamos el efecto. Para todas las skills basicas el efecto
     *    es daño plano; las que necesiten algo mas (FIRE_BALL en area)
     *    se manejan desde game_actions_attack. */
    return apply_basic_damage (receiver, id_skill, distance);
}

char*
skill_get_name (Skills_id id)
{
    if (!skill_id_is_valid (id)) return NULL;
    return skill_table[id].name;
}

int
skill_get_radio (Skills_id id)
{
    if (!skill_id_is_valid (id)) return -1;
    return skill_table[id].radio;
}

int
skill_get_damage (Skills_id id)
{
    if (!skill_id_is_valid (id)) return -1;
    return skill_table[id].damage;
}

int
skill_get_coste (Skills_id id)
{
    if (!skill_id_is_valid (id)) return -1;
    return skill_table[id].coste;
}

Status skill_print (Skills_id id)
{
    char* name;
    strcpy (name, skill_get_name(id));
    int radio = skill_get_radio(id);
    int damage = skill_get_damage(id);
    int coste = skill_get_coste(id);

    fprintf (stdout, " --> Skill (Id: %i; Name: %s; Radio: %i; Damage: %i; Coste: %i;)\n", id, name, radio, damage, coste);
    return OK;
}