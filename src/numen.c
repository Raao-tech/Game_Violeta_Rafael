/**
 * @brief It implements the object struct
 *
 * @file numen.c
 * @author Rafael
 * @version 0
 * @date 18-04-2026
 * @copyright GNU Public License
 */

#include "numen.h"
struct _Numen
{
    Character* c_numen;
    Skills_id skills[NUM_SKILLS];
};

/*========= (Create/Destroy) Numen ================*/
Numen*
numen_create ()
{
    Numen* new_numen = (Numen*)malloc (sizeof (Numen));
    if (!new_numen) return NULL;
    new_numen->c_numen = character_create ();
    if (!new_numen->c_numen)
        {
            free (new_numen);
            return NULL;
        }
    return new_numen;
}
Status
numen_destroy (Numen* numen)
{
    if (numen)
        {
            if (numen->c_numen) character_destroy (numen->c_numen);
            free (numen);
        }
    return OK;
}

/*======== (Set/Get) Id ===========================*/
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

/*======== (Set/Get/Has) Name ===========================*/
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

/*======== (set/get) skills ================*/
Status
numen_add_skill (Numen* numen, Id skill_id)
{
    if (!numen || skill_id < 0) return ERROR;
    for (int i = 0; i < NUM_SKILLS; i++)
        {
            if (numen->skills[i] == NO_SKILL)
                {
                    numen->skills[i] = skill_id;
                    return OK;
                }
        }
    return ERROR;
}
Id
numen_get_skill_by_id (Numen* numen, Id skill_id)
{
    if (!numen || skill_id < 0) return NO_ID;
    for (int i = 0; i < NUM_SKILLS; i++)
        {
            if (numen->skills[i] == skill_id)
                {
                    return skill_id;
                }
        }
    return NO_ID;
}
Skills_id
numen_get_skill_by_index (Numen* numen, int skill_indx)
{
    if (!numen || skill_indx < 0 || skill_indx >= NUM_SKILLS) return NO_SKILL;
    return numen->skills[skill_indx];
}

/*======== (Set/Get/ is errant?) following ===========================*/
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
    return ((character_get_following (numen->c_numen) == NO_ID) ? TRUE : FALSE);
}

Status
numen_set_corrupt (Numen* numen, Bool corrupt)
{
    if (!numen) return ERROR;
    return character_set_friendly (numen->c_numen, corrupt);
}
Bool
numen_get_corrupt (Numen* numen)
{
    if (!numen) return FALSE;
    return (character_get_friendly (numen->c_numen) == TRUE ? FALSE : TRUE);
}

/*======== (set/get) position ================*/
Status
numen_set_position (Numen* numen, int x, int y)
{
    if (!numen || x < 0 || x > WIDHT_SCREEN || y < 0 || y > HIGHT_SCREEN)
        {
            return ERROR;
        }
    return entity_set_position (numen->c_numen, x, y);
}

int
numen_get_pos_x (Numen* numen)
{
    if (!numen)
        {
            return -1;
        }
    return entity_get_pos_x (numen->c_numen);
}
int
numen_get_pos_y (Numen* numen)
{
    if (!numen)
        {
            return -1;
        }
    return entity_get_pos_y (numen->c_numen);
}

/*======== (set/get) gdesc() ================*/
Status
numen_set_gdesc (Numen* numen, char* gdesc)
{
    if (!numen || !gdesc) return ERROR;
    return entity_set_gdesc (numen->c_numen, gdesc);
}
char*
numen_get_gdesc (Numen* numen)
{
    if (!numen) return NULL;
    return entity_get_gdesc (numen->c_numen);
}

/* health */
Status
numen_set_health (Numen* numen, int life)
{
    if (!numen) return ERROR;
    return entity_set_health (numen->c_numen, life);
}
int
numen_get_health (Numen* numen)
{
    if (!numen) return ERROR_LIFE;
    return entity_get_health (numen->c_numen);
}

/* attack */
Status
numen_set_attack (Numen* numen, int attack)
{
    if (!numen) return ERROR;
    return entity_set_attack (numen->c_numen, attack);
}
int
numen_get_attack (Numen* numen)
{
    if (!numen) return ERROR_ATTACK;
    return entity_get_attack (numen->c_numen);
}

/* energy */
Status
numen_set_energy (Numen* numen, int energy)
{
    if (!numen) return ERROR;
    return entity_set_energy (numen->c_numen, energy);
}
int
numen_get_energy (Numen* numen)
{
    if (!numen) return ERROR_ENGY;
    return entity_get_energy (numen->c_numen);
}

/* Speed */
Status
numen_set_speed (Numen* numen, int speed)
{
    if (!numen) return ERROR;
    return entity_set_speed (numen->c_numen, speed);
}
int
numen_get_speed (Numen* numen)
{
    if (!numen) return 0;
    return entity_get_speed (numen->c_numen);
}

Status
numen_print (Numen* numen)
{
    if (!numen) return ERROR;
    return character_print (numen->c_numen);
}