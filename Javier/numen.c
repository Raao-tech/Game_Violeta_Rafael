/**
 * @brief It implements the numen module
 *
 * @file numen.c
 * @author Rafael, Violeta y Javier Jarque
 * @version 2.0
 * @date 02-05-2026
 * @copyright GNU Public License
 */

#include "numen.h"

struct _Numen
{
    Character* c_numen;
    Skills_id  skills[NUM_SKILLS];
};

/* ====================================================================== */
/*                          CREATE / DESTROY                               */
/* ====================================================================== */

Numen*
numen_create (void)
{
    Numen* new_numen = (Numen*)malloc (sizeof (Numen));
    int    i;

    if (!new_numen) return NULL;

    new_numen->c_numen = character_create ();
    if (!new_numen->c_numen)    {free (new_numen);  return NULL;}

    for (i = 0; i < NUM_SKILLS; i++) new_numen->skills[i] = NO_SKILL;

    return new_numen;
}

Status
numen_destroy (Numen* numen)
{
    if (!numen) return ERROR;
    if (numen->c_numen) character_destroy (numen->c_numen);
    free (numen);
    return OK;
}

/* ====================================================================== */
/*                                  ID                                     */
/* ====================================================================== */

Status
numen_set_id (Numen* numen, Id new_id)
{
    if (!numen) return ERROR;
    return character_set_id (numen->c_numen, new_id);
}

Id
numen_get_id (Numen* numen)
{
    if (!numen) return NO_ID;
    return character_get_id (numen->c_numen);
}

/* ====================================================================== */
/*                                 NAME                                    */
/* ====================================================================== */

Status
numen_set_name (Numen* numen, char* name)
{
    if (!numen || !name) return ERROR;
    return character_set_name (numen->c_numen, name);
}

char*
numen_get_name (Numen* numen)
{
    if (!numen) return NULL;
    return character_get_name (numen->c_numen);
}

Bool
numen_has_name (Numen* numen, char* name)
{
    if (!numen || !name) return FALSE;
    return character_has_name (numen->c_numen, name);
}

/* ====================================================================== */
/*                               POSITION                                  */
/*                                                                         */
/*  set_position valida limites de pantalla .                               */
/*  Si la coordenada esta fuera de [0..WIDHT_SCREEN] x [0..HIGHT_SCREEN]   */
/*  devolvemos ERROR sin tocar el estado                                    */
/* ====================================================================== */

Status
numen_set_position (Numen* numen, int x, int y)
{
    if (!numen) return ERROR;
    if (x < 0 || x >= WIDHT_SCREEN || y < 0 || y >= HIGHT_SCREEN) return ERROR;
    return character_set_position (numen->c_numen, x, y);
}

Position
numen_get_position (Numen* numen)
{
    Position pos;
    pos.pos_x = NO_POS;
    pos.pos_y = NO_POS;

    if (!numen) return pos;
    return character_get_position (numen->c_numen);
}

Status
numen_set_pos_x (Numen* numen, int pos_x)
{
    if (!numen) return ERROR;
    return character_set_pos_x (numen->c_numen, pos_x);
}

int
numen_get_pos_x (Numen* numen)
{
    if (!numen) return NO_POS;
    return character_get_pos_x (numen->c_numen);
}

Status
numen_set_pos_y (Numen* numen, int pos_y)
{
    if (!numen) return ERROR;
    return character_set_pos_y (numen->c_numen, pos_y); 
}

int
numen_get_pos_y (Numen* numen)
{
    if (!numen) return NO_POS;
    return character_get_pos_y (numen->c_numen);
}

/* ====================================================================== */
/*                                GDESC                                    */
/* ====================================================================== */

Status
numen_set_gdesc (Numen* numen, char* gdesc)
{
    if (!numen || !gdesc) return ERROR;
    return character_set_gdesc (numen->c_numen, gdesc);
}

char*
numen_get_gdesc (Numen* numen)
{
    if (!numen) return NULL;
    return character_get_gdesc (numen->c_numen);
}

/* ====================================================================== */
/*                                STATS                                    */
/* ====================================================================== */

Status
numen_set_health (Numen* numen, int health)
{
    if (!numen) return ERROR;
    return character_set_health (numen->c_numen, health);
}

int
numen_get_health (Numen* numen)
{
    if (!numen) return ERROR_LIFE;
    return character_get_health (numen->c_numen);
}

Status
numen_set_attack (Numen* numen, int attack)
{
    if (!numen) return ERROR;
    return character_set_attack (numen->c_numen, attack);
}

int
numen_get_attack (Numen* numen)
{
    if (!numen) return ERROR_ATTACK;
    return character_get_attack (numen->c_numen);
}

Status
numen_set_energy (Numen* numen, int energy)
{
    if (!numen) return ERROR;
    return character_set_energy (numen->c_numen, energy);
}

int
numen_get_energy (Numen* numen)
{
    if (!numen) return ERROR_ENGY;
    return character_get_energy (numen->c_numen);
}

Status
numen_set_speed (Numen* numen, int speed)
{
    if (!numen) return ERROR;
    return character_set_speed (numen->c_numen, speed);
}

int
numen_get_speed (Numen* numen)
{
    if (!numen) return 0;
    return character_get_speed (numen->c_numen);
}

/* ====================================================================== */
/*                                SKILLS                                   */
/* ====================================================================== */

Status
numen_add_skill (Numen* numen, Id skill_id)
{
    int i;
    if (!numen || skill_id < 0) return ERROR;

    for (i = 0; i < NUM_SKILLS; i++)
    {
        if (numen->skills[i] == NO_SKILL)
        {
            numen->skills[i] = skill_id;
            return OK;
        }
    }
    return ERROR;   /* Sin slots libres */
}

Id
numen_get_skill_by_id (Numen* numen, Id skill_id)
{
    int i;
    if (!numen || skill_id < 0) return NO_ID;

    for (i = 0; i < NUM_SKILLS; i++)
    {
        if (numen->skills[i] == skill_id) return skill_id;
    }
    return NO_ID;
}

Skills_id
numen_get_skill_by_index (Numen* numen, int skill_indx)
{
    if (!numen || skill_indx < 0 || skill_indx >= NUM_SKILLS) return NO_SKILL;
    return numen->skills[skill_indx];
}

/* ====================================================================== */
/*                              FOLLOWING                                  */
/* ====================================================================== */

Status
numen_set_following (Numen* numen, Id following)
{
    if (!numen) return ERROR;
    return character_set_following (numen->c_numen, following);
}

Id
numen_get_following (Numen* numen)
{
    if (!numen) return NO_ID;
    return character_get_following (numen->c_numen);
}

Bool
numen_is_errant (Numen* numen)
{
    if (!numen) return FALSE;
    return (character_get_following (numen->c_numen) == NO_ID) ? TRUE : FALSE;
}

/* ====================================================================== */
/*                               CORRUPT                                   */
/* ====================================================================== */

Status
numen_set_corrupt (Numen* numen, Bool corrupt)
{
    if (!numen) return ERROR;
    /* corrupt = TRUE → no friendly. La inversion vive aqui para que
     * el codigo cliente piense en terminos de "es enemigo" sin tener
     * que recordar que internamente Character usa "friendly". */
    return character_set_friendly (numen->c_numen, corrupt == TRUE ? FALSE : TRUE);
}

Bool
numen_get_corrupt (Numen* numen)
{
    if (!numen) return FALSE;
    return (character_get_friendly (numen->c_numen) == TRUE) ? FALSE : TRUE;
}

/* ====================================================================== */
/*                               PRINT                                    */
/* ====================================================================== */

Status numen_print (Numen* numen)
{
    if (!numen) return ERROR;
    Status status;

    printf (stdout, "\n--- Numen ---\n");
    status = skill_print (numen->skills);
    if (status == ERROR) return ERROR;
    else status = character_print (numen->c_numen);
    return status;
}